////////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// Copyright (c) 2013-25 by Carlos Martinho
//
// INTRODUCES:
// VIEW PIPELINE, UNIFORM BUFFER OBJECTS, mglCamera.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <memory>

#include <iostream>
#include "../mgl/mgl.hpp"
#include "./SphereCamera.hpp"
#include "SceneNode.hpp"

#define ORTH_PROJ 0
#define PERSPECTIVE_PROJ 1

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowCloseCallback(GLFWwindow* win) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;
    void keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) override;
    void cursorCallback(GLFWwindow* win, double xpos, double ypos) override;
    void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) override;
    void scrollCallback(GLFWwindow* win, double xoffset, double yoffset) override;
    void createMeshes();

private:
    SceneNode* puzzle, *square, *parallelogram, *sTriangle1, *sTriangle2, *mTriangle, *lTriangle1, *lTriangle2;
    mgl::Mesh* Mesh = nullptr;
    const GLuint POSITION = 0, COLOR = 1, UBO_BP = 0;
    GLuint VaoId;
    float width = 800, height = 600;

    int currCamera = 0;
    std::vector<SphereCamera*> cameras;
    std::vector<int> currProjection;
    std::vector<glm::mat4> projectionMatrices;
    float lastXPos = width / 2;
    float lastYPos = height / 2;
    const float perspectiveNear = 1.0f, perspectiveFar = 500.0f;

    bool rightBtnActive = false;

    mgl::ShaderProgram* Shaders = nullptr;
    GLint ModelMatrixId;

    void createShaderPrograms();
    void createBufferObjects();
    void createCameras();
    mgl::Mesh* getMesh(std::string mesh_dir, std::string mesh_file);
    void createSceneGraph();
    void destroyCameras();
    void destroyBufferObjects();
    void drawScene();
};

////////////////////////////////////////////////////////////////// VAO, VBO, EBO

typedef struct {
    GLfloat XYZW[4];
    GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] = {
    {{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 0 - FRONT
    {{1.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 1
    {{1.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 2
    // 2
{{0.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.1f, 1.0f}}, // 3
// 0

{{1.0f, 0.0f, 1.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 1 - RIGHT
{{1.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 5
{{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 6
// 6
{{1.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.9f, 0.1f, 1.0f}}, // 2
// 1

{{1.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 2 - TOP
{{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 6
{{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 7
// 7
{{0.0f, 1.0f, 1.0f, 1.0f}, {0.1f, 0.1f, 0.9f, 1.0f}}, // 3
// 2

{{1.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 5 - BACK
{{0.0f, 0.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 4
{{0.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 7
// 7
{{1.0f, 1.0f, 0.0f, 1.0f}, {0.1f, 0.9f, 0.9f, 1.0f}}, // 6
// 5

{{0.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 4 - LEFT
{{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 0
{{0.0f, 1.0f, 1.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 3
// 3
{{0.0f, 1.0f, 0.0f, 1.0f}, {0.9f, 0.1f, 0.9f, 1.0f}}, // 7
// 4

{{0.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}, // 0 - BOTTOM
{{0.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}, // 4
{{1.0f, 0.0f, 0.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}, // 5
// 5
{{1.0f, 0.0f, 1.0f, 1.0f}, {0.9f, 0.9f, 0.1f, 1.0f}}  // 1
// 0
};

const unsigned int Indices[] = {
    0,  1,  2,  2,  3,  0,  // FRONT
    4,  5,  6,  6,  7,  4,  // RIGHT
    8,  9,  10, 10, 11, 8,  // TOP
    12, 13, 14, 14, 15, 12, // BACK
    16, 17, 18, 18, 19, 16, // LEFT
    20, 21, 22, 22, 23, 20  // BOTTOM
};

void MyApp::createBufferObjects() {
    GLuint boId[2];

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    {
        glGenBuffers(2, boId);
        glBindBuffer(GL_ARRAY_BUFFER, boId[0]);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(0));

            glEnableVertexAttribArray(COLOR);
            glVertexAttribPointer(
                COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(sizeof(Vertices[0].XYZW)));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boId[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
            GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(2, boId);
}

void MyApp::destroyCameras() {
    for (int i = 0; i < cameras.size(); i++) {
        delete cameras[i];
    }
}

void MyApp::destroyBufferObjects() {
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(COLOR);
    glDeleteVertexArrays(1, &VaoId);
    glBindVertexArray(0);
}

///////////////////////////////////////////////////////////////////////// MESHES

void MyApp::createMeshes() {
    std::string mesh_dir = "../assets/";
    std::string mesh_file = "square_vn.obj";
    std::string mesh_fullname = mesh_dir + mesh_file;

    Mesh = new mgl::Mesh();
    Mesh->joinIdenticalVertices();
    Mesh->create(mesh_fullname);
}

mgl::Mesh* MyApp::getMesh(std::string mesh_dir, std::string mesh_file) {
    std::string mesh_fullname = mesh_dir + mesh_file;

    mgl::Mesh* mesh = new mgl::Mesh();
    mesh->joinIdenticalVertices();
    mesh->create(mesh_fullname);
    return mesh;
}

///////////////////////////////////////////////////////////////////////// SCENE GRAPH

void MyApp::createSceneGraph() {
    std::string dir = "../assets/";
    mgl::Mesh* squareMesh = getMesh(dir, "square_vn.obj");
    mgl::Mesh* sTriangle1Mesh = getMesh(dir, "small_triangle_1_vn.obj");
    mgl::Mesh* sTriangle2Mesh = getMesh(dir, "small_triangle_2_vn.obj");
    mgl::Mesh* parallelogramMesh = getMesh(dir, "parallelogram_vn.obj");
    mgl::Mesh* mTriangleMesh = getMesh(dir, "medium_triangle_vn.obj");
    mgl::Mesh* lTriangleMesh = getMesh(dir, "large_triangle_vn.obj");

    // Note: only puzzle has shaders. When draw is called on each piece, they will obtain their parent node's shaders
    // in this case their parent being puzzle

    puzzle = new SceneNode(nullptr, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), Shaders);
    square = new SceneNode(squareMesh, glm::vec4((34.0 / 255), (171.0 / 255), (36.0 / 255), 1.0f));
    sTriangle1 = new SceneNode(sTriangle1Mesh, glm::vec4((235.0 / 255), (71.0 / 255), (38.0 / 255), 1.0f));
    sTriangle2 = new SceneNode(sTriangle2Mesh, glm::vec4((0.0 / 255), (158.0 / 255), (166.0 / 255), 1.0f));
    parallelogram = new SceneNode(parallelogramMesh, glm::vec4((253.0 / 255), (140.0 / 255), (0.0 / 255), 1.0f));
    mTriangle = new SceneNode(mTriangleMesh, glm::vec4((109.0 / 255), (59.0 / 255), (191.0 / 255), 1.0f));
    lTriangle1 = new SceneNode(lTriangleMesh, glm::vec4((205.0 / 255), (14.0 / 255), (102.0 / 255), 1.0f));
    lTriangle2 = new SceneNode(lTriangleMesh, glm::vec4((15.0 / 255), (130.0 / 255), (242.0 / 255), 1.0f));

    puzzle->addChild(square);
    puzzle->addChild(sTriangle1);
    puzzle->addChild(sTriangle2);
    puzzle->addChild(parallelogram);
    puzzle->addChild(mTriangle);
    puzzle->addChild(lTriangle1);
    puzzle->addChild(lTriangle2);

    square->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(22.25f, 0.0f, 0.0f))*
                           glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    sTriangle1->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(44.5f, 0.0f, -22.25f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    sTriangle2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 22.25f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    parallelogram->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-11.125f, 0.0f, 33.375f)) *
                                  glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    mTriangle->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(22.25f, 0.0f, 22.25f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    lTriangle1->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -44.5f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    lTriangle2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-44.5f, 0.0f, 0.0f)) *
                               glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}


///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderPrograms() {
    Shaders = new mgl::ShaderProgram();
    Shaders->addShader(GL_VERTEX_SHADER, "cube-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "color-normal-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
    if (Mesh->hasNormals()) {
        Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
    }
    if (Mesh->hasTexcoords()) {
        Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
    }
    if (Mesh->hasTangentsAndBitangents()) {
        Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);
    }

    Shaders->addUniform(mgl::MODEL_MATRIX);
    Shaders->addUniform("Color");
    Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
    Shaders->create();

    ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

////////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCameras() {
    // Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
    // Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
    const glm::mat4 OrthoProjection =
        glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f , 120.0f, 500.0f);
    const glm::mat4 PerspectiveProjection =
        glm::perspective(glm::radians(60.0f), width / height, perspectiveNear, perspectiveFar);

    projectionMatrices.push_back(OrthoProjection);
    projectionMatrices.push_back(PerspectiveProjection);

    currProjection.assign(projectionMatrices.size(), 0);

    SphereCamera *camera1 = new SphereCamera(UBO_BP, glm::vec3(250.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), false, perspectiveNear, perspectiveFar);
    SphereCamera *camera2 = new SphereCamera(UBO_BP, glm::vec3(0.0f, 0.0f, 250.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), false, perspectiveNear, perspectiveFar);

    camera1->setProjectionMatrix(projectionMatrices[0]);
    camera2->setProjectionMatrix(projectionMatrices[0]);

    cameras.push_back(camera1);
    cameras.push_back(camera2);

    cameras[currCamera]->bind();
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -0.5f));

void MyApp::drawScene() {
    cameras[currCamera]->updateView();
    puzzle->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    glfwSetCursorPos(win, width / 2, height / 2);
    createMeshes();
    createShaderPrograms();
    createSceneGraph();
    createCameras();
}

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        currCamera = (currCamera + 1) % cameras.size();
        cameras[currCamera]->bind();
    } else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        currProjection[currCamera] = (currProjection[currCamera] + 1) % projectionMatrices.size();
        cameras[currCamera]->setProjectionMatrix(projectionMatrices[currProjection[currCamera]]);
        cameras[currCamera]->setPerspectiveProj(currProjection[currCamera] == PERSPECTIVE_PROJ);
    }
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
    if (rightBtnActive) {
        float sensitivity = 0.25f;
        float xdelta = lastXPos - xpos;
        float ydelta = lastYPos - ypos;
        cameras[currCamera]->addYaw(xdelta * sensitivity);
        cameras[currCamera]->addPitch(ydelta * sensitivity);
    }

    lastXPos = xpos;
    lastYPos = ypos;
}

void MyApp::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightBtnActive = action == GLFW_PRESS;
    }
}

void MyApp::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    // Only apply zoom when using a perspective projection
    if (currProjection[currCamera] == PERSPECTIVE_PROJ)
        cameras[currCamera]->addZoom(yoffset);
}

void MyApp::windowCloseCallback(GLFWwindow* win) { 
    destroyCameras();
    destroyBufferObjects(); 
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
    const glm::mat4 OrthoProjection = glm::ortho(-100.0f * (float) winx / 800.0f, 100.0f * (float) winx / 800.0f, -100.0f * (float) winy / 600.0f, 100.0f * (float) winy / 600.0f, 120.0f, 500.0f);
    projectionMatrices[0] = OrthoProjection;
    const glm::mat4 PerspectiveProjection = glm::perspective(glm::radians(60.0f), (float) winx / (float) winy, perspectiveNear, perspectiveFar);
    projectionMatrices[1] = PerspectiveProjection;

    for (int i = 0; i < cameras.size(); i++) {
        cameras[i]->setProjectionMatrix(projectionMatrices[currProjection[i]]);
    }
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(new MyApp());
    engine.setOpenGL(4, 6);
    engine.setWindow(800, 600, "Hello Modern 3D World", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////
