#ifndef SPHERE_CAMERA_HPP
#define SPHERE_CAMERA_HPP

#include "./mglCamera.hpp"

class SphereCamera : public mgl::Camera {
	private:
		glm::vec3 eye;
		glm::vec3 center;
		glm::vec3 up;
		float yawAngle, pitchAngle;
		float zoom;
		int currProjection;
	public:
		SphereCamera(GLuint bindingPoint, glm::vec3 eye, glm::vec3 center, glm::vec3 up);
		void nextProjection();
		void addYaw(float yaw);
		void addPitch(float pitch);
		void addZoom(float zoom);
		void updateView();
};

#endif // SPHERE_CAMERA_HPP
