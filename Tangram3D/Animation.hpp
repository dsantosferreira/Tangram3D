#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "glm/glm.hpp"
#include <glm/ext/matrix_transform.hpp>

/*
 * This header file stores the necessary data to animate the scene
 * This includes starting and finishing positions for all pieces, as well as starting and finishing rotations
 * All Meshes are, by default, perpendicular to the Y axis (puzzle laying horizontally)
 * All singular piece translations and rotations happen in the XZ plane
 * This means translations always have Y = 0.0f, and rotations happen around the Y axis, except for the parallelogram piece which has to be flipped
 * Flipping the puzzle to lay vertically (perpendicular to the Z axis) is done by rotating the puzzle SceneNode 90 degrees around X axis
 * Since the pieces are all "children" of the puzzle SceneNode, this rotation propagates into the WorldMatrix of each piece
 * This simplifies the transformations needed on each piece, as all their transformations can be done on the XZ plane without considering the final position on XY
 */ 

// TODO: rotation using quaternions

// Basic variables created to reduce written code;
glm::mat4 I = glm::mat4(1.0f);
glm::vec3 X = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 Y = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Z = glm::vec3(0.0f, 0.0f, 1.0f);

glm::mat4 squarePosStart = glm::translate(I, glm::vec3(22.25f, 0.0f, 0.0f));
glm::mat4 squareAngleStart = glm::rotate(I, glm::radians(-45.0f), Y);
glm::mat4 squarePosEnd = glm::translate(I, glm::vec3(-69.4494, 0.0f, -24.9494f));
glm::mat4 squareAngleEnd = glm::rotate(I, glm::radians(90.0f), Y);

glm::mat4 parallelogramPosStart = glm::translate(I, glm::vec3(-11.125f, 0.0f, 33.375f));
glm::mat4 parallelogramAngleStart = glm::rotate(I, glm::radians(180.0f), Y);
glm::mat4 parallelogramPosEnd = glm::translate(I, glm::vec3(-37.9832, 0.0f, -9.21627));
glm::mat4 parallelogramAngleEnd = glm::rotate(I, glm::radians(-45.0f), Y) * glm::rotate(I, glm::radians(180.0f), X);

glm::mat4 sTriangle1PosStart = glm::translate(I, glm::vec3(44.5f, 0.0f, -22.25f));
glm::mat4 sTriangle1AngleStart = glm::rotate(I, glm::radians(-45.0f), Y);
glm::mat4 sTriangle1PosEnd = glm::translate(I, glm::vec3(-100.916, 0.0f, -24.9494f));
glm::mat4 sTriangle1AngleEnd = glm::rotate(I, glm::radians(90.0f), Y);

glm::mat4 sTriangle2PosStart = glm::translate(I, glm::vec3(0.0f, 0.0f, 22.25f));
glm::mat4 sTriangle2AngleStart = glm::rotate(I, glm::radians(-135.0f), Y);
glm::mat4 sTriangle2PosEnd = glm::translate(I, glm::vec3(0.0f, 0.0f, 22.25f));
glm::mat4 sTriangle2AngleEnd = glm::rotate(I, glm::radians(45.0f), Y);

glm::mat4 mTrianglePosStart = glm::translate(I, glm::vec3(22.25f, 0.0f, 22.25f));
glm::mat4 mTriangleAngleStart = glm::rotate(I, glm::radians(90.0f), Y);
glm::mat4 mTrianglePosEnd = glm::translate(I, glm::vec3(0.0f, 0.0f, 0.0f));
glm::mat4 mTriangleAngleEnd = glm::rotate(I, glm::radians(0.0f), Y);

glm::mat4 lTriangle1PosStart = glm::translate(I, glm::vec3(0.0f, 0.0f, -44.5f));
glm::mat4 lTriangle1AngleStart = glm::rotate(I, glm::radians(45.0f), Y);
glm::mat4 lTriangle1PosEnd = glm::translate(I, glm::vec3(66.75f, 0.0f, 22.25f));
glm::mat4 lTriangle1AngleEnd = glm::rotate(I, glm::radians(-135.0f), Y);

glm::mat4 lTriangle2PosStart = glm::translate(I, glm::vec3(-44.5f, 0.0f, 0.0f));
glm::mat4 lTriangle2AngleStart = glm::rotate(I, glm::radians(135.0f), Y);
glm::mat4 lTriangle2PosEnd = glm::translate(I, glm::vec3(22.25f, 0.0f, -22.25f));
glm::mat4 lTriangle2AngleEnd = glm::rotate(I, glm::radians(45.0f), Y);

glm::mat4 puzzleAngleStart = glm::rotate(I, glm::radians(0.0f), X);
glm::mat4 puzzleAngleEnd = glm::rotate(I, glm::radians(90.0f), X);

#endif