#include "sceneNode.hpp"

SceneNode::SceneNode(mgl::Mesh* mesh = nullptr, glm::vec4* Color = nullptr, std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr) {
	this->mesh = mesh;
	this->Color = Color;
	this->Shaders = std::make_unique<mgl::ShaderProgram>(Shaders);

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

//TODO add parent to child node
void SceneNode::addChild(SceneNode node) {
	children.push_back(node);
}

//TODO draw
void SceneNode::draw() {}