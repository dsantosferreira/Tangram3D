#include <utility>

#include "SceneNode.hpp"
#include <mglConventions.hpp>
#include <glm/gtc/type_ptr.hpp>

SceneNode::SceneNode(mgl::Mesh* mesh, mgl::ShaderProgram* Shaders, glm::vec4 Color) {
	this->mesh = mesh;
	this->Color = Color;
	this->Shaders = Shaders;
	if (Shaders != nullptr) {
		ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
	}

	// Set default model and world matrices
	ModelMatrix = glm::mat4(1.0f);
	WorldMatrix = glm::mat4(1.0f);
}

void SceneNode::setModelMatrix(glm::mat4 matrix) {
	ModelMatrix = matrix;
	for (auto & child : children) {
		child.setWorldMatrix(matrix); // Update world matrix of all children with this nodes model matrix
	}
}

void SceneNode::setWorldMatrix(glm::mat4 matrix) {
	WorldMatrix = matrix;
	for (auto& child : children) {
		child.setWorldMatrix(WorldMatrix * matrix); // World matrix of all children depends on world and model matrices of parent
	}
}

void SceneNode::addChild(SceneNode node) {
	children.push_back(node);
	node.setParent(*this);
}

void SceneNode::setParent(SceneNode node) {
	this->parent = &node;
}

// TODO figure out how to pass uniforms to children without shaders
mgl::ShaderProgram* SceneNode::getShaders() {
	if (Shaders == nullptr) { // If this node has no shaders, get shaders from parent
		return parent->getShaders();
	}
	else {
		return Shaders;
	}
}

void SceneNode::draw() {
	if (mesh != nullptr) { // If node has a mesh, draw node
		this->getShaders()->bind();

		glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(WorldMatrix * ModelMatrix));
		mesh->draw();

		this->getShaders()->unbind();
	}
	for (auto& child : children) { // Tell all children to draw themselves
		child.draw();
	}
}