#include "./SphereCamera.hpp"
#include <iostream>

SphereCamera::SphereCamera(GLuint bindingPoint, std::vector<glm::mat4> projectionMatrices, glm::vec3 eye, glm::vec3 center, glm::vec3 up) : 
	currProjection(0), projectionMatrices(projectionMatrices), mgl::Camera(bindingPoint) {
	this->eye = eye;
	this->center = center;
	this->up = up;
	this->yawAngle = 0.0f;
	this->pitchAngle = 0.0f;
	this->setViewMatrix(glm::lookAt(eye, center, up));
	if (!projectionMatrices.empty())
		this->setProjectionMatrix(projectionMatrices[0]);
}

void SphereCamera::nextProjection() {
	currProjection = (currProjection + 1) % projectionMatrices.size();
	this->setProjectionMatrix(projectionMatrices[currProjection]);
}

void SphereCamera::addYaw(float yaw) {
	this->yawAngle += yaw;
}

void SphereCamera::addPitch(float pitch) {
	this->pitchAngle += pitch;
}

void SphereCamera::updateView() {
	glm::vec3 view = this->center - this->eye;
	glm::vec3 side = glm::normalize(glm::cross(view, this->up));
	glm::vec3 objUp = glm::normalize(glm::cross(side, view));
	
	glm::quat yawQ = glm::angleAxis(glm::radians(this->yawAngle), objUp);
	glm::quat pitchQ = glm::angleAxis(glm::radians(this->pitchAngle), side);
	glm::quat rot = yawQ * pitchQ;

	this->eye = rot * this->eye * glm::inverse(rot);
	this->up = rot * this->up * glm::inverse(rot);

	this->setViewMatrix(glm::lookAt(this->eye, this->center, this->up));

	this->yawAngle = 0;
	this->pitchAngle = 0;
}