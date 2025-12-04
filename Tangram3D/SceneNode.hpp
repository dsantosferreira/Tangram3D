#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <memory>

#include "./mglScenegraph.hpp"
#include "./mglShader.hpp"
#include "./mglMesh.hpp"
#include "glm/glm.hpp"

class SceneNode : public mgl::IDrawable {
	private:
		SceneNode* parent = nullptr;
		mgl::Mesh* mesh;
		mgl::ShaderProgram* Shaders;
		std::vector<SceneNode*> children;
		glm::mat4 WorldMatrix;	// Matrix with transformations passed down from parent nodes
		glm::mat4 ModelMatrix;	// Matrix with transformations applied to this node
		glm::vec4 Color;
	public:
		SceneNode(mgl::Mesh* mesh = nullptr, glm::vec4 Color = glm::vec4(0.0f), mgl::ShaderProgram* Shaders = nullptr);
		void setModelMatrix(glm::mat4 matrix);
		void setWorldMatrix(glm::mat4 matrix);
		void addChild(SceneNode* node);
		void setParent(SceneNode* node);
		mgl::ShaderProgram* getShaders();
		void draw() override;
};

#endif