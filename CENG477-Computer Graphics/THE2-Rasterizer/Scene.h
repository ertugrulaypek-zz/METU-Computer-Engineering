#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"

#include "Matrix4.h"

using namespace std;

class TransformedModel
{
	public:
    int transformedmodel_modelId;
    int transformedmodel_type;
    int transformedmodel_numberOfTriangles;
    vector<vector<Vec3>> transformed_triangles; 
};

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);

	Vec3 multiply3x4MatrixWithVec4(double m[3][4], Vec4 v);
	Color subtractColor(Color a, Color b);
	Color divideColorwithInt(Color a, int b);
	void drawPixel(Camera& cam, int x, int y, Color c);

	Matrix4 translation(Translation* translation);
	Matrix4 scaling(Scaling* scaling);
	Matrix4 rotation(Rotation* rotation);

	void cameraTransformation(Camera& cam, 	Matrix4& camera_transformation_matrix);
	void orthographicProjection(Camera& cam, Matrix4& orth_projection_matrix);
	void perspectiveProjection(Camera& cam, Matrix4& pers_projection_matrix);
	void viewportTransformation(Camera& cam, double viewport_transformation_matrix[3][4]);
	void modelingTransformation(Camera& cam, Model* model, Matrix4& M_model);
	/*Culling*/
	bool willCullorNot(Camera& cam, vector<Vec3> useforCull);

	/*Clipping algorithms*/
	bool visible(double den, double num, double t_E, double t_L);

	/*apply matrix and cull*/
	void applyallMatrixtoVertexwithOrthographic(Camera& cam);
	void applyallMatrixtoVertexwithPerspective(Camera& cam);
	
	/*Rasterization algorithms*/
	double f_01(int x_0, int y_0, int x_1, int y_1, int x, int y);
	double f_12(int x_1, int y_1, int x_2, int y_2, int x, int y);
	double f_20(int x_2, int y_2, int x_0, int y_0, int x, int y);
	void midpointAlgorithm(Camera& cam, TransformedModel transformed_model);
	void rasterizeTriangle(Camera& cam, TransformedModel transformed_model);
	void rasterization(Camera& cam);


};

#endif
