#include "Shape.h"
#include "Scene.h"
#include <cstdio>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
	: id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f>* pVertices)
	: Shape(id, matIndex)
{
	this->radius = R;
	this->center = (*pVertices)[cIndex - 1];
}

/* Sphere-ray intersection routine. You will implement this.
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray& ray) const
{
	ReturnVal retVal;
	retVal.isHit = false;
	float A = ray.direction.dot(ray.direction);
	Vector3f fromCenterToOrigin = ray.origin - this->center;
	float B = 2 * ray.direction.dot(fromCenterToOrigin);
	float C = fromCenterToOrigin.dot(fromCenterToOrigin) - this->radius * this->radius;
	float discriminant = B * B - 4 * A * C;
	if (discriminant >= 0) {
		
		float t1 = (-1 * B + sqrtf(discriminant)) / 2 * A;
		float t2 = (-1 * B - sqrtf(discriminant)) / 2 * A;
		
		if(t1 > pScene->intTestEps && t2 > pScene->intTestEps){
			
			retVal.isHit = true;
			retVal.matIndex = this->matIndex;
			retVal.objectShape = 0;
			retVal.objectId = this->id;

			float t = fmin(t1, t2);
			retVal.hitPoint = ray.origin + t * ray.direction;
			retVal.normalAtHitPoint = retVal.hitPoint - this->center;
			retVal.normalAtHitPoint /= this->radius;
			retVal.normalAtHitPoint.normalize();
			retVal.hitTime = t;
		}
		
	}
	return retVal;
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f>* pVertices)
	: Shape(id, matIndex)
{

	this->pt1 = (*pVertices)[p1Index - 1];
	this->pt2 = (*pVertices)[p2Index - 1];
	this->pt3 = (*pVertices)[p3Index - 1];


}

/* Triangle-ray intersection routine. You will implement this.
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray& ray) const
{
	ReturnVal hit;
	hit.isHit = false;

	Vector3f A_firstCol = this->pt1 - this->pt2;
	Vector3f A_secondCol = this->pt1 - this->pt3;
	Vector3f A_thirdCol = ray.direction;

	Vector3f B = this->pt1 - ray.origin;

	Matrix3f A;
	A.row(0) = A_firstCol;
	A.row(1) = A_secondCol;
	A.row(2) = A_thirdCol;
	float detA = A.determinant();

	Matrix3f t;
	t.row(0) = A_firstCol;
	t.row(1) = A_secondCol;
	t.row(2) = B;
	float detT = t.determinant() / detA;

	if (detT <= pScene->intTestEps) {
		return hit;
	}

	Matrix3f beta;
	beta.row(0) = B;
	beta.row(1) = A_secondCol;
	beta.row(2) = A_thirdCol;
	float detBeta = beta.determinant() / detA;

	if (detBeta < 0) {
		return hit;
	}

	Matrix3f gamma;
	gamma.row(0) = A_firstCol;
	gamma.row(1) = B;
	gamma.row(2) = A_thirdCol;
	float detGamma = gamma.determinant() / detA;

	if (detGamma < 0) {
		return hit;
	}

	if (detGamma + detBeta > 1) {
		return hit;
	}

	hit.isHit = true;
	hit.matIndex = this->matIndex;
	hit.objectShape = 1;
	hit.objectId = this->id;
	hit.hitPoint = ray.origin + detT * ray.direction;
	hit.normalAtHitPoint = (this->pt2 - this->pt1).cross(this->pt3 - this->pt1);
	hit.normalAtHitPoint = hit.normalAtHitPoint.normalized();
	hit.hitTime = detT;

	return hit;

}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int>* pIndices, vector<Vector3f>* pVertices)
	: Shape(id, matIndex)
{
	this->faces = faces;
}

/* Mesh-ray intersection routine. You will implement this.
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc.
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray& ray) const
{
	ReturnVal tempHit;
	ReturnVal resultHit;
	tempHit.isHit = false;
	resultHit.isHit = false;
	vector<ReturnVal> hitVec;

	for (int faceNumber = 0; faceNumber < this->faces.size(); faceNumber++) {
		tempHit = this->faces[faceNumber].intersect(ray);
		if (tempHit.isHit)
			hitVec.push_back(tempHit);
	}

	for (int i = 0; i < hitVec.size(); i++) {

		if (!resultHit.isHit) {
			resultHit = hitVec[i];
		}

		if (resultHit.hitTime > hitVec[i].hitTime) {
			resultHit = hitVec[i];
		}
	}
	return resultHit;
}