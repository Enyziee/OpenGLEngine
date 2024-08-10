#include "Renderer.h"
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cctype>

#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "GLLog.h"
#include "Texture.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float fov = 45.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float roll = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX, lastY;
bool firstMouse = true;

bool fullScreen = false;
bool paused = false;

void static scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;

    if (fov < 1.0f) {
        fov = 1.0f;
    } 
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

void static mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void static load_obj(std::string path, std::vector<float>* vertices, std::vector<uint32_t>* indexes) {
    std::ifstream file(path);

    if (!file) {
        std::cout << "Cannot open the file" << std::endl;
        return;
    }

    std::string line;
    
    while (getline(file, line)) {
        if (line.find("v ") != std::string::npos) {
            std::string number;

            for (int i = 2; i < line.size(); i++) {
                char ch = line[i];

                if (isblank(ch)) {
                    float value = std::stod(number);
                    vertices->push_back(value);
                    number.clear();
                    continue;
                }

                if (i == (line.size() - 1)) {
                    number.push_back(ch);

                    float value = std::stod(number);
                    vertices->push_back(value);
                    number.clear();
                    continue;
                }

                number.push_back(ch);
            }
        }
        if (line.find("f ") != std::string::npos) {
            std::string number;

            for (int i = 2; i < line.size(); i++) {
                char ch = line[i];

                if (isblank(ch)) {
                    uint32_t value = std::stoi(number) - 1;
                    indexes->push_back(value);
                    number.clear();
                    continue;
                }

                if (i == (line.size() - 1)) {
                    number.push_back(ch);

                    uint32_t value = std::stoi(number) - 1;
                    indexes->push_back(value);
                    number.clear();
                    continue;
                }

                number.push_back(ch);
            }
        }
    }

}

void static process_input(GLFWwindow* window) {
    
    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        std::cout << "Wireframe mode enabled" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        std::cout << "Wireframe mode disabled" << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraUp;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void static framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    std::cout << "Window resized to " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

int main(void) {
    GLFWwindow* window;
    const int WIDTH = 800;
    const int HEIGHT = 600;

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    VertexBufferLayout cubeLayout;
    cubeLayout.Push<float>(3);
    cubeLayout.Push<float>(2);
    
    VertexBuffer cubeVertex = VertexBuffer(cubeVertices, 180);
    ShaderProgram shader = ShaderProgram("res/shader/Basic.shader");
    
    VertexArray cubeVA = VertexArray();
    cubeVA.addData(cubeVertex, cubeLayout);

    Texture texture1("./res/textures/container.jpg");
    Texture texture2("./res/textures/awesomeface.png");

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    std::vector<float>* teapotVertex = new std::vector<float>;
    std::vector<uint32_t>* teapotIndex = new std::vector<uint32_t>;

    load_obj("./res/objects/Txotas.obj", teapotVertex, teapotIndex);
    VertexBufferLayout loadedLayot;
    loadedLayot.Push<float>(3);

    IndexBuffer loadedIB = IndexBuffer(teapotIndex->data(), teapotIndex->size());
    VertexBuffer loadedVB = VertexBuffer(teapotVertex->data(), teapotVertex->size());
    
    VertexArray loadedVA = VertexArray();
    loadedVA.addData(loadedVB, loadedLayot);

    Renderer renderer = Renderer();

    while (!glfwWindowShouldClose(window)) {
        
        renderer.clear();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        process_input(window);
        
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        texture1.bind(0);
        texture2.bind(1);

        shader.bind();
        shader.setUniform1i("texture1", 0);
        shader.setUniform1i("texture2", 1);

        shader.setUniformMatrix4fv("projection", glm::value_ptr(projection));
        shader.setUniformMatrix4fv("view", glm::value_ptr(view));

        for (unsigned int i = 0; i < 10; i++) {
            cubeVA.bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniformMatrix4fv("model", glm::value_ptr(model));
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
            cubeVA.unbind();
        }


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 15.0f));
        shader.setUniformMatrix4fv("model", glm::value_ptr(model));
        renderer.draw(loadedVA, loadedIB);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
