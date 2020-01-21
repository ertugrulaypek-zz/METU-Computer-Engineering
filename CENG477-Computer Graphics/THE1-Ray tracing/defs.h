#ifndef _DEFS_H_
#define _DEFS_H_

#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;
class Scene;

/* Structure to hold return value from ray intersection routine. 
This should hold information related to the intersection point, 
for example, coordinate of the intersection point, surface normal at the intersection point etc. 
Think about the variables you will need for this purpose and declare them here inside of this structure. */
typedef struct ReturnVal
{
    bool isHit;
    Vector3f hitPoint;
    Vector3f normalAtHitPoint;
    float hitTime;
    int objectShape;
    int matIndex;
    int objectId;
} ReturnVal;

/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color. 
Use x, y, z for coordinate computations, and use r, g, b for color computations. 
Note that you do not have to use this one if you use any vector computation library like Eigen. */
/*typedef struct Vector3f
{
	union 
	{
		float x;
		float r;
	};
	union
	{
		float y;
		float g;
	};
	union
	{
		float z;
		float b;
	};
} Vector3f;*/

//
// The global variable through which you can access the scene data
//
extern Scene* pScene;

#endif
