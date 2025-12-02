#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <memory>

#include "./mglScenegraph.hpp"
#include "./mglShader.hpp"
#include "./mglMesh.hpp"
#include "glm/glm.hpp"

class SceneNode : public mgl::IDrawable {
	private:
		mgl::Mesh* mesh;
		std::vector<SceneNode> children;
		glm::mat4 WorldMatrix;	// Matrix with transformations passed down from parent nodes
		glm::mat4 ModelMatrix;	// Matrix with transformations applied to this node
		glm::vec4* Color;
		std::unique_ptr<mgl::ShaderProgram> Shaders;
	public:
		SceneNode(mgl::Mesh* mesh = nullptr, glm::vec4* Color = nullptr,std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr);
		void setModelMatrix(glm::mat4 matrix);
		void setWorldMatrix(glm::mat4 matrix);
		void addChild(SceneNode node);
		void draw() override;
};

#endif