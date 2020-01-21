#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/

vector<TransformedModel> Models_transformed;


Vec3 Scene::multiply3x4MatrixWithVec4(double m[3][4], Vec4 v){
    double total;
	vector<double> vec;
	Vec3 vector3;

    for (int i = 0; i < 3; i++)
    {
        total = 0;
        for (int j = 0; j < 4; j++)
        {
            total += m[i][j] * v.getElementAt(j);
        }
        vec.push_back(total);
    }
	vector3.x = vec[0];
	vector3.y = vec[1];
	vector3.z = vec[2];
	vector3.colorId = v.colorId;

    return vector3;
}

Color Scene::subtractColor(Color a, Color b){
    Color result;
    result.r = a.r - b.r;
    result.g = a.g - b.g;
    result.b = a.b - b.b;

    return result;
}

Color Scene::divideColorwithInt(Color a, int b){
	Color result;
	result.r = a.r / b;
	result.g = a.g / b;
	result.b = a.b / b;

	return result;
}

void Scene::drawPixel(Camera& cam, int x, int y, Color c){

	if(x >= 0 && x < cam.horRes){
		if(y >= 0 && y < cam.verRes){
			image[x][y].r = c.r > 255 ? 255 : c.r;
			image[x][y].r = image[x][y].r < 0 ? 0 : image[x][y].r;
			image[x][y].g = c.g > 255 ? 255 : c.g;
			image[x][y].g = image[x][y].g < 0 ? 0 : image[x][y].g;
			image[x][y].b = c.b > 255 ? 255 : c.b;
			image[x][y].b = image[x][y].b < 0 ? 0 : image[x][y].b;
		}
	}
}

Matrix4 Scene::translation(Translation* translation){
	Matrix4 translation_matrix = getIdentityMatrix();

	translation_matrix.val[0][3] = translation->tx;
	translation_matrix.val[1][3] = translation->ty;
	translation_matrix.val[2][3] = translation->tz;
	
	return translation_matrix;
}

Matrix4 Scene::scaling(Scaling* scaling){
	Matrix4 scaling_matrix = getIdentityMatrix();

	scaling_matrix.val[0][0] = scaling->sx;
	scaling_matrix.val[1][1] = scaling->sy;
	scaling_matrix.val[2][2] = scaling->sz;

	return scaling_matrix;
}

Matrix4 Scene::rotation(Rotation* rotation){
	Vec3 u, v, w;
	Matrix4 M = getIdentityMatrix(), M_inverse = getIdentityMatrix(), Rotate_x_theta = getIdentityMatrix();
	double theta = (rotation->angle * M_PI) / 180.0; //degree to radian

	u.x = rotation->ux;
	u.y = rotation->uy;
	u.z = rotation->uz;

	double abs_x = ABS(u.x);
	double abs_y = ABS(u.y);
	double abs_z = ABS(u.z);

	char min;
	/*find min element*/
	min = ((abs_x < abs_y) ? (abs_x < abs_z ? 'x' : 'z') : (abs_y < abs_z ? 'y' : 'z')); 

	switch (min){
		case 'x':{
			v.x = 0.0;
			v.y = -1.0 * u.z;
			v.z = u.y;
		}break;

		case 'y':{
			v.x = -1.0 * u.z;
			v.y = 0.0;
			v.z = u.x;
		}break;

		case 'z':{
			v.x = -1.0 * u.y;
			v.y = u.x;
			v.z = 0.0;
		}break;
	}

	u = normalizeVec3(u);
	v = normalizeVec3(v);

	w = crossProductVec3(u,v);
	w = normalizeVec3(w);

	/*M transformation matrix*/
	M.val[0][0] = u.x;
	M.val[0][1] = u.y;
	M.val[0][2] = u.z;

	M.val[1][0] = v.x;
	M.val[1][1] = v.y;
	M.val[1][2] = v.z;

	M.val[2][0] = w.x;
	M.val[2][1] = w.y;
	M.val[2][2] = w.z;


	/*M_inverse transformation matrix*/
	M_inverse.val[0][0] = u.x;
	M_inverse.val[0][1] = v.x;
	M_inverse.val[0][2] = w.x;

	M_inverse.val[1][0] = u.y;
	M_inverse.val[1][1] = v.y;
	M_inverse.val[1][2] = w.y;

	M_inverse.val[2][0] = u.z;
	M_inverse.val[2][1] = v.z;
	M_inverse.val[2][2] = w.z;


	/*Rotation x around theta matrix*/

	Rotate_x_theta.val[1][1] = cos(theta);
	Rotate_x_theta.val[1][2] = -1 * sin(theta);

	Rotate_x_theta.val[2][1] = sin(theta);
	Rotate_x_theta.val[2][2] = cos(theta);


	/*Final matrix*/
	Matrix4 Rotation_matrix = multiplyMatrixWithMatrix(M_inverse , multiplyMatrixWithMatrix(Rotate_x_theta,M));

	return Rotation_matrix;
}


void Scene::cameraTransformation(Camera& cam, Matrix4& camera_transformation_matrix){
	double pos_x = cam.pos.x;
	double pos_y = cam.pos.y;
	double pos_z = cam.pos.z;

	camera_transformation_matrix.val[0][0] = cam.u.x;
	camera_transformation_matrix.val[0][1] = cam.u.y;
	camera_transformation_matrix.val[0][2] = cam.u.z;
	camera_transformation_matrix.val[0][3] = -1.0 * (cam.u.x * pos_x + cam.u.y * pos_y + cam.u.z * pos_z);

	camera_transformation_matrix.val[1][0] = cam.v.x;
	camera_transformation_matrix.val[1][1] = cam.v.y;
	camera_transformation_matrix.val[1][2] = cam.v.z;
	camera_transformation_matrix.val[1][3] = -1.0 * (cam.v.x * pos_x + cam.v.y * pos_y + cam.v.z * pos_z);

	camera_transformation_matrix.val[2][0] = cam.w.x;
	camera_transformation_matrix.val[2][1] = cam.w.y;
	camera_transformation_matrix.val[2][2] = cam.w.z;
	camera_transformation_matrix.val[2][3] = -1.0 * (cam.w.x * pos_x + cam.w.y * pos_y + cam.w.z * pos_z);
	
}

void Scene::orthographicProjection(Camera& cam, Matrix4& orth_projection_matrix){
	orth_projection_matrix.val[0][0] = 2.0 / (cam.right - cam.left);
	orth_projection_matrix.val[0][3] = -1.0 * (cam.right + cam.left) / (cam.right - cam.left);

	orth_projection_matrix.val[1][1] = 2.0 / (cam.top - cam.bottom);
	orth_projection_matrix.val[1][3] = -1.0 * (cam.top + cam.bottom) / (cam.top - cam.bottom);

	orth_projection_matrix.val[2][2] = -2.0 / (cam.far - cam.near);
	orth_projection_matrix.val[2][3] = -1.0 * (cam.far + cam.near) / (cam.far - cam.near);
}

void Scene::perspectiveProjection(Camera& cam, Matrix4& pers_projection_matrix){

	pers_projection_matrix.val[0][0] = 2.0 * cam.near / (cam.right - cam.left);
	pers_projection_matrix.val[0][2] = (cam.right + cam.left) / (cam.right - cam.left);

	pers_projection_matrix.val[1][1] = 2.0 * cam.near / (cam.top - cam.bottom);
	pers_projection_matrix.val[1][2] = (cam.top + cam.bottom) / (cam.top - cam.bottom);

	pers_projection_matrix.val[2][2] = -1.0 * (cam.far + cam.near) / (cam.far - cam.near);
	pers_projection_matrix.val[2][3] = -1.0 * (2 * cam.far * cam.near) / (cam.far - cam.near);

	pers_projection_matrix.val[3][2] = -1.0;
	pers_projection_matrix.val[3][3] = 0.0; //because it sended as identity matrix it should be corrected
}

void Scene::viewportTransformation(Camera& cam, double viewport_transformation_matrix[3][4]){

	viewport_transformation_matrix[0][0] = cam.horRes / 2.0;
	viewport_transformation_matrix[0][3] = (cam.horRes - 1) / 2.0;

	viewport_transformation_matrix[1][1] = cam.verRes / 2.0;
	viewport_transformation_matrix[1][3] = (cam.verRes - 1) / 2.0;

	viewport_transformation_matrix[2][2] = 0.5;
	viewport_transformation_matrix[2][3] = 0.5;

}

void Scene::modelingTransformation(Camera& cam, Model* model, Matrix4& M_model){

	int number_of_Transformations = model->numberOfTransformations;

	for(int i=0; i < number_of_Transformations; i++){
		char transformation_type = model->transformationTypes[i];

		switch (transformation_type){
			case ('t'):				
				M_model = multiplyMatrixWithMatrix(translation(translations[model->transformationIds[i]-1]), M_model);
				break;

			case ('s'):
				M_model = multiplyMatrixWithMatrix(scaling(scalings[model->transformationIds[i]-1]), M_model);
				break;

			case ('r'):
				M_model = multiplyMatrixWithMatrix(rotation(rotations[model->transformationIds[i]-1]), M_model);
				break;
		}
	}
}


bool Scene::willCullorNot(Camera& cam, vector<Vec3> useforCull){
	Vec3 eye_vector = subtractVec3(useforCull[0], cam.pos);
	Vec3 transformed_normal = crossProductVec3(subtractVec3(useforCull[1], useforCull[0]), subtractVec3(useforCull[2],useforCull[0])); 

	if(dotProductVec3(transformed_normal, eye_vector) >= 0)
		return false;

	return true;
}


bool Scene::visible(double den, double num, double t_E, double t_L){

	double t = num / den;

	if(den > 0){
		if(t > t_L)
			return false;
		if(t > t_E)
			t_E = t;
	}

	else if(den < 0){
		if(t < t_E)
			return false;
		if(t < t_L)
			t_L = t;
	}

	else if(num > 0){
		return false;
	}

	return true;
}


void Scene::applyallMatrixtoVertexwithOrthographic(Camera& cam){

	int amountofModel = models.size();

	for(int i=0; i < amountofModel ; ++i ){
		TransformedModel transformed_model;
		transformed_model.transformedmodel_numberOfTriangles = models[i]->numberOfTriangles;
		transformed_model.transformedmodel_modelId = models[i]->modelId;
		transformed_model.transformedmodel_type = models[i]->type;
		
		Matrix4 M_model = getIdentityMatrix();
		Matrix4 M_cam = getIdentityMatrix();
		Matrix4 M_orth = getIdentityMatrix();
		double M_viewport[3][4] = {};
		
		modelingTransformation(cam, models[i], M_model);
		orthographicProjection(cam, M_orth);
		cameraTransformation(cam, M_cam);
		viewportTransformation(cam, M_viewport);

		Matrix4 matrix_orthAndcam(multiplyMatrixWithMatrix(M_orth, M_cam));

		int amountofTriangles = models[i]->numberOfTriangles;

		for(int j=0; j < amountofTriangles; ++j){
			Triangle current_triangle = models[i]->triangles[j];
			vector<Vec3> useforCull(3);
			vector<Vec3> new_triangle(3);
			
			/*creates transformed triangle*/
			for(int k=0; k < 3; ++k){
				int vertexids = current_triangle.vertexIds[k];
				Vec3* vertex = vertices[vertexids - 1];

				Vec4 point(vertex->x, vertex->y, vertex->z, 1.0, vertex->colorId);
				Vec4 transformed_point(multiplyMatrixWithVec4(M_model, point)); 
				
				useforCull[k].x = transformed_point.x;
				useforCull[k].y = transformed_point.y;
				useforCull[k].z = transformed_point.z;

				transformed_point = multiplyMatrixWithVec4(matrix_orthAndcam, transformed_point);
				double transpoint_t = transformed_point.t;
				transformed_point.x = transformed_point.x / transpoint_t;
				transformed_point.y = transformed_point.y / transpoint_t;
				transformed_point.z = transformed_point.z / transpoint_t;	
				transformed_point.t = transpoint_t / transpoint_t;

				/*CLIPPING BURADA MI YAPILACAK????*/
				Vec3 viewport_point(multiply3x4MatrixWithVec4(M_viewport, transformed_point));
				
				new_triangle[k].x = int(viewport_point.x);
				new_triangle[k].y = int(viewport_point.y);
				new_triangle[k].z = int(viewport_point.z);
				new_triangle[k].colorId = viewport_point.colorId;
			}

			/*push this transformed triangle to transformed model or cull*/
			bool visibility = true;
			
			if(cullingEnabled){
				visibility = willCullorNot(cam, useforCull);
				
			}

			if(visibility)
				transformed_model.transformed_triangles.push_back(new_triangle);
			
			else
				transformed_model.transformedmodel_numberOfTriangles -= 1;
		}

		Models_transformed.push_back(transformed_model);
	}
}


void Scene::applyallMatrixtoVertexwithPerspective(Camera& cam){

	int amountofModel = models.size();

	for(int i=0; i < amountofModel ; ++i ){

		TransformedModel transformed_model;
		transformed_model.transformedmodel_numberOfTriangles = models[i]->numberOfTriangles;
		transformed_model.transformedmodel_modelId = models[i]->modelId;
		transformed_model.transformedmodel_type = models[i]->type;
		
		Matrix4 M_model = getIdentityMatrix();
		Matrix4 M_cam = getIdentityMatrix();
		Matrix4 M_pers = getIdentityMatrix();
		double M_viewport[3][4] = {};
		
		modelingTransformation(cam, models[i], M_model);
		perspectiveProjection(cam, M_pers);
		cameraTransformation(cam, M_cam);
		viewportTransformation(cam, M_viewport);

		Matrix4 matrix_persAndcam(multiplyMatrixWithMatrix(M_pers, M_cam));

		int amountofTriangles = models[i]->numberOfTriangles;

		for(int j=0; j < amountofTriangles; ++j){
			Triangle current_triangle = models[i]->triangles[j];
			vector<Vec3> useforCull(3);
			vector<Vec3> new_triangle(3);

			/*creates transformed triangle*/
			for(int k=0; k < 3; ++k){
				int vertexids = current_triangle.vertexIds[k];

				Vec3* vertex = vertices[vertexids - 1];
	
				Vec4 point(vertex->x, vertex->y, vertex->z, 1.0, vertex->colorId);

				Vec4 transformed_point(multiplyMatrixWithVec4(M_model, point)); 
				
				useforCull[k].x = transformed_point.x;
				useforCull[k].y = transformed_point.y;
				useforCull[k].z = transformed_point.z;

				transformed_point = multiplyMatrixWithVec4(matrix_persAndcam, transformed_point);
				double transpoint_t = transformed_point.t;
				transformed_point.x = transformed_point.x / transpoint_t;
				transformed_point.y = transformed_point.y / transpoint_t;
				transformed_point.z = transformed_point.z / transpoint_t;	
				transformed_point.t = transpoint_t / transpoint_t;

				Vec3 viewport_point(multiply3x4MatrixWithVec4(M_viewport, transformed_point));
				
				new_triangle[k].x = int(viewport_point.x);
				new_triangle[k].y = int(viewport_point.y);
				new_triangle[k].z = int(viewport_point.z);
				new_triangle[k].colorId = viewport_point.colorId;
				
			}
			
			/*push this transformed triangle to transformed model or cull*/
			bool visibility = true;
			
			if(cullingEnabled){
				visibility = willCullorNot(cam, useforCull);
			}

			if(visibility)
				transformed_model.transformed_triangles.push_back(new_triangle);
			
			else
				transformed_model.transformedmodel_numberOfTriangles -= 1;
		}

		Models_transformed.push_back(transformed_model);
	}

}



void Scene::midpointAlgorithm(Camera& cam, TransformedModel transformed_model){
	int amountofTriangles = transformed_model.transformedmodel_numberOfTriangles;

	for(int i=0; i < amountofTriangles; ++i){

		for(int j=0; j < 3; ++j){
			
			double m, d;
			Color c_0,c_1,dc;
			int x, y;
			int p = (j+1)%3;
			int x0,y0,x1,y1;

			//x0 > x1 situation
			if(transformed_model.transformed_triangles[i][j].x > transformed_model.transformed_triangles[i][p].x){
				x0 = transformed_model.transformed_triangles[i][p].x;
				y0 = transformed_model.transformed_triangles[i][p].y;
				x1 = transformed_model.transformed_triangles[i][j].x;
				y1 = transformed_model.transformed_triangles[i][j].y;
				c_0 = (*(colorsOfVertices[transformed_model.transformed_triangles[i][p].colorId - 1]));
				c_1 = (*(colorsOfVertices[transformed_model.transformed_triangles[i][j].colorId - 1]));

			}

			else{
				x0 = transformed_model.transformed_triangles[i][j].x;
				y0 = transformed_model.transformed_triangles[i][j].y;
				x1 = transformed_model.transformed_triangles[i][p].x;
				y1 = transformed_model.transformed_triangles[i][p].y;
				c_0 = (*(colorsOfVertices[transformed_model.transformed_triangles[i][j].colorId - 1]));
				c_1 = (*(colorsOfVertices[transformed_model.transformed_triangles[i][p].colorId - 1]));

			}
			
			x = x0;
			y = y0;
			
			m = (double)(y1 - y0) / (double)(x1 - x0);

			//buranin en basinda -sonsuz veya 0 veya +sonsuz degerlerine esit
			// olmasina da bakacak miyiz oyleyse altta degisiklik yapip yukarda bakabilirim

			if(m > 0 && m < 1){
				x = x0;
				y = y0;
				d = 2 * (y0 - y1) + (x1 - x0);

				Color c = c_0;
				dc = divideColorwithInt(subtractColor(c_1,c_0),abs(x1-x0));

				for(; x < x1; ++x){
					drawPixel(cam, x, y, {round(c.r),round(c.g),round(c.b)});
		
					if(d < 0){
						y = y + 1;
						d += 2 * ((y0 - y1)+(x1 - x0));
					}
					else{
						d += 2 * (y0 - y1);
					}

					c.r += dc.r;
					c.g += dc.g;
					c.b += dc.b;
				}
			}

			else if(m >= 1){
				x = x0;
				y = y0;
				d = 2 * (x0 - x1) + (y1 - y0);

				Color c = c_0;
				dc = divideColorwithInt(subtractColor(c_1,c_0),abs(y1-y0));

				for(; y < y1; ++y){
					drawPixel(cam, x, y, {round(c.r),round(c.g),round(c.b)});

					if(d < 0){
						x = x + 1;
						d += 2 * ((x0 - x1) + (y1 - y0));
					}
					else{
						d += 2 * (x0 - x1);
					}

					c.r += dc.r;
					c.g += dc.g;
					c.b += dc.b;
				}
			}

			else if(m <= 0 && m >= -1){
				x = x0;
				y = y0;
				d = 2 * (y1 - y0) + (x1 - x0);

				Color c = c_0;
				dc = divideColorwithInt(subtractColor(c_1,c_0),abs(x1-x0));

				for(; x < x1; ++x){
					drawPixel(cam, x, y, {round(c.r),round(c.g),round(c.b)});
					if(d < 0){
						y = y - 1;
						d += 2 * ((y1 - y0) + (x1 - x0));
					}
					else{
						d += 2 * (y1 - y0);
					}

					c.r += dc.r;
					c.g += dc.g;
					c.b += dc.b;
				}
			}

			else if(m < -1){
				x = x0;
				y = y0;
				d = 2 * (x0 - x1) + (y0 - y1);

				Color c = c_0;
				dc = divideColorwithInt(subtractColor(c_1,c_0),abs(y1-y0));

				for(; y > y1; y--){
					drawPixel(cam, x, y, {round(c.r),round(c.g),round(c.b)});
					if(d < 0){
						x = x + 1;
						d += 2 * ((x0 - x1) + (y0 - y1));
					}
					else{
						d += 2 * (x0 - x1);
					}

					c.r += dc.r;
					c.g += dc.g;
					c.b += dc.b;
				}
			}
		}
	}
}


double Scene::f_01(int x_0, int y_0, int x_1, int y_1, int x, int y){
	return x * (y_0 - y_1) + y * (x_1 - x_0) + x_0 * y_1 - y_0 * x_1;
}

double Scene::f_12(int x_1, int y_1, int x_2, int y_2, int x, int y){
	return x * (y_1 - y_2) + y * (x_2 - x_1) + x_1 * y_2 - y_1 * x_2;
}

double Scene::f_20(int x_2, int y_2, int x_0, int y_0, int x, int y){
	return x * (y_2 - y_0) + y * (x_0 - x_2) + x_2 * y_0 - y_2 * x_0;
}

void Scene::rasterizeTriangle(Camera& cam, TransformedModel model){

	int amountofTriangle = model.transformedmodel_numberOfTriangles;
	
	for(int i=0; i < amountofTriangle; ++i){
		vector<Vec3> current_triangle = model.transformed_triangles[i];	

		int x0 = current_triangle[0].x;
		int x1 = current_triangle[1].x;
		int x2 = current_triangle[2].x;

		int y0 = current_triangle[0].y;
		int y1 = current_triangle[1].y;
		int y2 = current_triangle[2].y;

		//Bounding box for efficiency
		int x_min = floor(min(min(x0,x1),x2));
		int y_min = floor(min(min(y0,y1),y2));

		int x_max = ceil(max(max(x0,x1),x2));
		int y_max = ceil(max(max(y0,y1),y2));

		double f_12_alpha = f_12(x1, y1, x2, y2, x0, y0);
		double f_20_beta = f_20(x2, y2, x0, y0, x1, y1);
		double f_01_gama = f_01(x0, y0, x1, y1, x2, y2);

		double alpha,beta,gama;
	
		for(int y = y_min; y < y_max; ++y){
			for(int x = x_min; x < x_max; ++x){
				alpha = f_12(x1, y1, x2, y2, x, y) / (double)f_12_alpha;
				beta = f_20(x2, y2, x0, y0, x, y) / (double)f_20_beta;
				gama = f_01(x0, y0, x1, y1, x, y) / (double)f_01_gama;

				if((alpha >= 0) && (beta >= 0) && (gama >= 0)){

					Color c;

					Color c_0 = (*(colorsOfVertices[current_triangle[0].colorId-1]));
					Color c_1 = (*(colorsOfVertices[current_triangle[1].colorId-1]));
					Color c_2 = (*(colorsOfVertices[current_triangle[2].colorId-1]));
					
					c.r = (int)(round(alpha * c_0.r + beta * c_1.r + gama * c_2.r));  
					c.g = (int)(round(alpha * c_0.g + beta * c_1.g + gama * c_2.g));
					c.b = (int)(round(alpha * c_0.b + beta * c_1.b + gama * c_2.b));

					drawPixel(cam, x, y, c);

				}
			}
		}
	}
}


void Scene::rasterization(Camera& cam){
	int amountofTransformedModel = Models_transformed.size();

	for(int i=0; i < amountofTransformedModel; ++i){
		if(Models_transformed[i].transformedmodel_type){//solid
			rasterizeTriangle(cam, Models_transformed[i]);
		}
			
		else{//wireframe
			midpointAlgorithm(cam, Models_transformed[i]);
		}
					
	}
	//clear
	for(int i=0; i<amountofTransformedModel; ++i){
		Models_transformed[i].transformedmodel_type = 0;
		Models_transformed[i].transformedmodel_numberOfTriangles = 0;
		Models_transformed[i].transformedmodel_modelId = 0;
		Models_transformed[i].transformed_triangles.clear();
	}
	Models_transformed.clear();
}


void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function.
	
	//test_modeling_transforms();
	
	if(projectionType){
		applyallMatrixtoVertexwithPerspective(*camera);
	}
	
	else{
		applyallMatrixtoVertexwithOrthographic(*camera);//burada seg fault yiyor!!!!!!!	
	}


	rasterization(*camera);
}



/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}