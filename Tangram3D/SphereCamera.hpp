#ifndef SPHERE_CAMERA_HPP
#define SPHERE_CAMERA_HPP

#include "./mglCamera.hpp"

class SphereCamera : public mgl::Camera {
	public:
		SphereCamera(GLuint bindingPoint);
};

#endif // SPHERE_CAMERA_HPP
