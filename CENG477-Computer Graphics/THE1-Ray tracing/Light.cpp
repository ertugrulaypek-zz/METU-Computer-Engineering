#include "Light.h"

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
    
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{

     float squareDistance = (p[0] - this->position[0]) * (p[0] - this->position[0]) + 
        (p[1] - this->position[1]) * (p[1] - this->position[1]) + 
        (p[2] - this->position[2]) * (p[2] - this->position[2]);

    Vector3f contribution = this->intensity / squareDistance;

    return contribution;


}
