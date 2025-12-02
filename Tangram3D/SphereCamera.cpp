#include "./SphereCamera.hpp"
#include <iostream>

SphereCamera::SphereCamera(GLuint bindingPoint, std::vector<glm::mat4> projectionMatrices) : 
	currProjection(0), projectionMatrices(projectionMatrices), mgl::Camera(bindingPoint) {}

void SphereCamera::nextProjection() {
	currProjection = (currProjection + 1) % projectionMatrices.size();
	this->setProjectionMatrix(projectionMatrices[currProjection]);
}