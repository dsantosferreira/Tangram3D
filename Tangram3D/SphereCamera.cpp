#include "./SphereCamera.hpp"
#include <iostream>

SphereCamera::SphereCamera(GLuint bindingPoint, glm::vec3 eye, glm::vec3 center, glm::vec3 up, bool perspectiveProj, float minZoom, float maxZoom)
	: mgl::Camera(bindingPoint) {
	this->eye = eye;
	this->center = center;
	this->up = up;
	this->yawAngle = 0.0f;
	this->pitchAngle = 0.0f;
	this->zoom = 0.0f;
	this->perspectiveProj = perspectiveProj;
	this->minZoom = minZoom;
	this->maxZoom = maxZoom;
	this->setViewMatrix(glm::lookAt(eye, center, up));
}

void SphereCamera::addYaw(float yaw) {
	this->yawAngle += yaw;
}

void SphereCamera::addPitch(float pitch) {
	this->pitchAngle += pitch;
}

// TODO: Take elapsed time into accountglm::normalize
void SphereCamera::updateView() {
	glm::vec3 view = this->center - this->eye;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::normalize(view) * this->zoom);
	glm::vec3 side = glm::normalize(glm::cross(view, this->up));
	glm::vec3 objUp = glm::normalize(glm::cross(side, view));
	
	glm::quat yawQ = glm::angleAxis(glm::radians(this->yawAngle), objUp);
	glm::quat pitchQ = glm::angleAxis(glm::radians(this->pitchAngle), side);
	glm::quat rot = yawQ * pitchQ;

	this->eye = rot * glm::vec4(this->eye, 1.0f) * glm::inverse(rot);
	this->up = rot * glm::vec4(this->up, 1.0f) * glm::inverse(rot);

	// If the camera is using a perspective projection, translate the camera and up vector in the view matrix to zoom
	if (this->perspectiveProj) {
		glm::vec3 zoomedEye = T * glm::vec4(this->eye, 1.0f);
		this->setViewMatrix(glm::lookAt(zoomedEye, this->center, this->up));
	} else {
		this->setViewMatrix(glm::lookAt(this->eye, this->center, this->up));
	}

	this->yawAngle = 0;
	this->pitchAngle = 0;
}

void SphereCamera::addZoom(float zoom) {
	this->zoom += zoom;

	glm::vec3 view = this->center - this->eye;
	std::cout << "Curr zoom: " << this->zoom << " " << glm::length(view) << std::endl;
	if (glm::length(view) - this->zoom > this->maxZoom)
		this->zoom = glm::length(view) - this->maxZoom;

	if (glm::length(view) - this->zoom < this->minZoom)
		this->zoom =  glm::length(view) - this->minZoom;
}

void SphereCamera::setPerspectiveProj(bool perspectiveProj) {
	this->perspectiveProj = perspectiveProj;
}

float SphereCamera::getEyePos() {
	glm::vec3 view = this->center - this->eye;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::normalize(view) * this->zoom);
	return glm::length(T * glm::vec4(this->eye, 1.0f));
}