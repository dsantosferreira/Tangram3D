#ifndef SPHERE_CAMERA_HPP
#define SPHERE_CAMERA_HPP

#include "./mglCamera.hpp"

class SphereCamera : public mgl::Camera {
	private:
		int currProjection;
		const std::vector<glm::mat4> projectionMatrices;
	public:
		void nextProjection();
		SphereCamera(GLuint bindingPoint, std::vector<glm::mat4> projectionMatrices);
};

#endif // SPHERE_CAMERA_HPP
