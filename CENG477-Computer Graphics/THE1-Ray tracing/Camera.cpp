#include "Camera.h"

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
     this->id = id;
     this->pos = pos;
     this->gaze = gaze;
     this->up = up;
     this->imgPlane = imgPlane;
     int i;
     for(i=0 ; *(imageName + i) ; i++){
          this->imageName[i] = *(imageName + i);
     }
     this->imageName[i] = 0;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
     float right = this->imgPlane.right;  
     float left = this->imgPlane.left;  
     float top = this->imgPlane.top;  
     float bottom = this->imgPlane.bottom;  

     float su = (right - left)*(col + 0.5)/this->imgPlane.nx;
     float sv = (top - bottom)*(row + 0.5)/this->imgPlane.ny;

     Vector3f m, q, u, s;
     u = this->up.cross(-1*this->gaze);

     m = this->pos + this->imgPlane.distance * this->gaze;

     u.normalize();


     q = m + u*left + this->up*top;
     s = q + u*su - this->up*sv;

     Ray ray;
     ray.origin = this->pos;
     ray.direction = (s - this->pos).normalized();

     return ray;

}
