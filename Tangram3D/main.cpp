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

private:
    const GLuint POSITION = 0, COLOR = 1, UBO_BP = 0;
    GLuint VaoId;
    float width = 800, height = 600;

    int currCamera = 0;
    std::vector<SphereCamera*> cameras;
    std::vector<int> currProjection;
    std::vector<glm::mat4> projectionMatrices;
    float lastXPos = width / 2;
    float lastYPos = height / 2;

    bool rightBtnActive = false;

    std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
    std::unique_ptr<mgl::Camera> Camera = nullptr;
    GLint ModelMatrixId;

    void createShaderProgram();
    void createBufferObjects();
    void createCameras();
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

///////////////////////////////////////////////////////////////////////// SHADER

void MyApp::createShaderProgram() {
    Shaders = std::make_unique<mgl::ShaderProgram>();
    Shaders->addShader(GL_VERTEX_SHADER, "color-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "color-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
    Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
    Shaders->addUniform(mgl::MODEL_MATRIX);
    Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

    Shaders->create();

    ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
}

////////////////////////////////////////////////////////////////////////// CAMERAS

void MyApp::createCameras() {
    // Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
    // Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
    const glm::mat4 ProjectionMatrix1 =
        glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, 1.0f, 10.0f);
    const glm::mat4 ProjectionMatrix2 =
        glm::perspective(glm::radians(30.0f), width / height, 1.0f, 15.0f);

    projectionMatrices.push_back(ProjectionMatrix1);
    projectionMatrices.push_back(ProjectionMatrix2);

    currProjection.assign(projectionMatrices.size(), 0);

    SphereCamera *camera1 = new SphereCamera(UBO_BP, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    SphereCamera *camera2 = new SphereCamera(UBO_BP, glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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

    glBindVertexArray(VaoId);
    Shaders->bind();

    glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
    glDrawElements(GL_TRIANGLES, sizeof(Indices), GL_UNSIGNED_INT, nullptr);

    Shaders->unbind();
    glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    glfwSetCursorPos(win, width / 2, height / 2);
    createBufferObjects();
    createShaderProgram();
    createCameras();
}

void MyApp::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        currCamera = (currCamera + 1) % cameras.size();
        cameras[currCamera]->bind();
    } else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        currProjection[currCamera] = (currProjection[currCamera] + 1) % projectionMatrices.size();
        cameras[currCamera]->setProjectionMatrix(projectionMatrices[currProjection[currCamera]]);
    }
}

void MyApp::cursorCallback(GLFWwindow* win, double xpos, double ypos) {
    if (rightBtnActive) {
        float xdelta = lastXPos - xpos;
        float ydelta = lastYPos - ypos;
        cameras[currCamera]->addYaw(xdelta);
        cameras[currCamera]->addPitch(ydelta);
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
    cameras[currCamera]->addZoom(yoffset);
}

void MyApp::windowCloseCallback(GLFWwindow* win) { 
    destroyCameras();
    destroyBufferObjects(); 
}

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
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
