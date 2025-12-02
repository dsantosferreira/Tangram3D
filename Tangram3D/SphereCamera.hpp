#ifndef SPHERE_CAMERA_HPP
#define SPHERE_CAMERA_HPP

#include "./mglCamera.hpp"

class SphereCamera : public mgl::Camera {
	private:
		glm::vec3 eye;
		glm::vec3 center;
		glm::vec3 up;
		float yawAngle, pitchAngle;
		int currProjection;
		const std::vector<glm::mat4> projectionMatrices;
	public:
		SphereCamera(GLuint bindingPoint, std::vector<glm::mat4> projectionMatrices, glm::vec3 eye, glm::vec3 center, glm::vec3 up);
		void nextProjection();
		void addYaw(float yaw);
		void addPitch(float pitch);
		void updateView();
};

#endif // SPHERE_CAMERA_HPP
