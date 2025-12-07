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
		bool perspectiveProj;
		float minZoom, maxZoom;
	public:
		SphereCamera(GLuint bindingPoint, glm::vec3 eye, glm::vec3 center, glm::vec3 up, bool perspectiveProj, float minZoom, float maxZoom);
		void addYaw(float yaw);
		void addPitch(float pitch);
		void addZoom(float zoom);
		void updateView();
		void setPerspectiveProj(bool perspectiveProj);
		glm::vec3 getViewVector() const;
		glm::vec3 getSideVector() const;
};

#endif // SPHERE_CAMERA_HPP
