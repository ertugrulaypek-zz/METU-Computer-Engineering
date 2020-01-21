#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const 
{
    Vector3f point = this->origin + t*this->direction;
    return point;
}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
	if(! this->direction[0]) return (p-this->origin)[0]/this->direction[0];
	else if(!this->direction[1]) return (p - this->origin)[1] / this->direction[1];
	else return (p - this->origin)[2] / this->direction[2];
}

