#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"
#include <string>
using namespace tinyxml2;

Vector3f Scene::prodEveryElem(Vector3f a, Vector3f b){
	float val1 = a[0]*b[0];
	float val2 = a[1]*b[1];
	float val3 = a[2]*b[2];
	return Vector3f(val1, val2, val3);
}

Vector3f Scene::generateRGBValue(const ReturnVal &hitResult, Camera &currentCamera, const Ray &ray, int recursionDepth){

    Vector3f rgbValue(0,0,0);

    if(hitResult.isHit){
    	int matIndex = hitResult.matIndex;

		// Apply ambient light to each object:
		rgbValue = this->prodEveryElem(this->materials[matIndex - 1]->ambientRef, this->ambientLight);
		// Shadow:

		for(int lightNumber = 0; lightNumber < this->lights.size(); lightNumber++){
		    bool isShadow = false;

		    PointLight* currentLight = this->lights[lightNumber];

			Ray shadowRay(hitResult.hitPoint + hitResult.normalAtHitPoint*this->shadowRayEps, 
			(currentLight->position - hitResult.hitPoint + hitResult.normalAtHitPoint*this->shadowRayEps).normalized());

			ReturnVal shadowHit;

			float tLight = (currentLight->position - shadowRay.origin)[0] / shadowRay.direction[0];

        	for(int objectNumber = 0; objectNumber < this->objects.size(); objectNumber++){
          		shadowHit = this->objects[objectNumber]->intersect(shadowRay);

          		if(shadowHit.isHit){
            		if(tLight > shadowHit.hitTime && shadowHit.hitTime >= 0){
              			isShadow = true;
            		}
          		}
        	}

	        if(!isShadow){

				Vector3f diffuse;

				Vector3f irradiance = currentLight->computeLightContribution(hitResult.hitPoint);

				Vector3f l = currentLight->position - hitResult.hitPoint;
				l = l.normalized();

				float dotPro = l.dot(hitResult.normalAtHitPoint);
				if(dotPro < 0){
					dotPro = 0;
				}

				diffuse = this->prodEveryElem(this->materials[matIndex - 1]->diffuseRef, dotPro * irradiance);

				Vector3f specular;

				Material* material = this->materials[hitResult.matIndex - 1];

				Vector3f wi = currentLight->position - hitResult.hitPoint;
				wi = wi.normalized();

				Vector3f h = wi - ray.direction;
				h = h.normalized();

				dotPro = hitResult.normalAtHitPoint.dot(h);
				if(dotPro < 0){
					dotPro = 0;
				}

				specular = this->prodEveryElem(material->specularRef,pow(dotPro, material->phongExp) * irradiance);

				rgbValue += diffuse + specular;
	        }
    	}


		// mirror:

		Material* material = this->materials[hitResult.matIndex - 1];

		if ( ( (material->mirrorRef[0] != 0) && (material->mirrorRef[1] != 0) && (material->mirrorRef[2] != 0) ) && (recursionDepth > 0) ) {
			float wi = -2 * (ray.direction.dot(hitResult.normalAtHitPoint));
			Vector3f normal_wi;

			normal_wi = (hitResult.normalAtHitPoint * wi + ray.direction).normalized();

			Vector3f wi_epsilon;
			wi_epsilon = normal_wi * this->shadowRayEps;
			

			Ray reflectionRay(hitResult.hitPoint + wi_epsilon, normal_wi);


			vector<ReturnVal> hitVector;
			for (int objectNumber = 0; objectNumber < this->objects.size(); objectNumber++) {
				ReturnVal hit = this->objects[objectNumber]->intersect(reflectionRay);

				if (hit.isHit) {
					hitVector.push_back(hit);
				}
			}
			ReturnVal mirrorHitResult;
			mirrorHitResult.isHit = false;
			for (int i = 0; i < hitVector.size(); i++) {
				if (mirrorHitResult.isHit == false)
					mirrorHitResult = hitVector[i];
				if (mirrorHitResult.hitTime > hitVector[i].hitTime)
					mirrorHitResult = hitVector[i];
			}


			if (!(mirrorHitResult.objectId == hitResult.objectId && mirrorHitResult.objectShape == hitResult.objectShape))
			{
				Vector3f reflection = generateRGBValue(mirrorHitResult, currentCamera, reflectionRay, (recursionDepth - 1));

				rgbValue += this->prodEveryElem(reflection, this->materials[hitResult.matIndex - 1]->mirrorRef);
			}

		}

  	}
  	else{
		rgbValue = this->backgroundColor;
  	}

  	return rgbValue;
}

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void){

	int numberOfCameras = this->cameras.size();
    for(int cameraNumber = 0; cameraNumber < numberOfCameras; cameraNumber++){
        Camera *currentCamera = this->cameras[cameraNumber];
        int imgWidth = currentCamera->imgPlane.nx;
        int imgHeight = currentCamera->imgPlane.ny;

		Image image(imgWidth, imgHeight);

        for(int i = 0; i < imgHeight; i++){
            for(int j = 0; j < imgWidth; j++){

				Ray ray = currentCamera->getPrimaryRay(i, j);
				vector<ReturnVal> hitVector;

				for(int objectNumber = 0; objectNumber < this->objects.size(); objectNumber++){
					ReturnVal retVal = this->objects[objectNumber]->intersect(ray);

					if(retVal.isHit){
						hitVector.push_back(retVal);
					}
				}

				ReturnVal hitResult;
				hitResult.isHit = false;
				for(int i=0 ; i<hitVector.size() ; i++){
					if(hitResult.isHit==false)
						hitResult = hitVector[i];
					if(hitResult.hitTime > hitVector[i].hitTime)
						hitResult = hitVector[i];	
				}

             	Vector3f rgbValue = this->generateRGBValue(hitResult, *currentCamera, ray, this->maxRecursionDepth);
				Color color;
				if(rgbValue[0] < 0){
					color.red = 0;
				}
				if(rgbValue[1] < 0){
					color.grn = 0;
				}
				if(rgbValue[2] < 0){
					color.blu = 0;
				}
            	if(rgbValue[0] > 255)
            		color.red = 255;
              	else
                	color.red = round(rgbValue[0]);

            	if(rgbValue[1] > 255)
            		color.grn = 255;
             	else
                	color.grn = round(rgbValue[1]);

            	if(rgbValue[2] > 255)
            		color.blu = 255;
              	else
                	color.blu = round(rgbValue[2]);

				image.setPixelValue(i, j, color);
            }
        }

		image.saveImage(currentCamera->imageName);
    }
}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor[0], &backgroundColor[1], &backgroundColor[2]);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos[0], &pos[1], &pos[2]);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze[0], &gaze[1], &gaze[2]);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up[0], &up[1], &up[2]);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef[0], &materials[curr]->ambientRef[1], &materials[curr]->ambientRef[2]);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef[0], &materials[curr]->diffuseRef[1], &materials[curr]->diffuseRef[2]);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef[0], &materials[curr]->specularRef[1], &materials[curr]->specularRef[2]);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef[0], &materials[curr]->mirrorRef[1], &materials[curr]->mirrorRef[2]);
		}
				else
		{
			materials[curr]->mirrorRef[0] = 0.0;
			materials[curr]->mirrorRef[1] = 0.0;
			materials[curr]->mirrorRef[2] = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint[0] = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint[1] = atof(str + cursor);
			else
				tmpPoint[2] = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight[0], &ambientLight[1], &ambientLight[2]);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position[0], &position[1], &position[2]);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

