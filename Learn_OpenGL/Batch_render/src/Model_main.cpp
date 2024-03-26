#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    Shader shader("res/Shader/model.shader");
    Shader SpotLight_Shader("res/Shader/Basic.shader");

    Model OurModel("res/nanosuit/nanosuit.obj");

    SpotLight_Shader.Bind();
    SpotLight_Shader.SetUniform1i("material.diffuse", 0);
    SpotLight_Shader.SetUniform1i("material.specular", 1);//物体受到镜面光照影响的颜色  镜面（反射）高光

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        SpotLight_Shader.Bind();
        SpotLight_Shader.SetUniformMat4("u_model", model);
        SpotLight_Shader.SetUniformMat4("u_VP", proj * view);
        SpotLight_Shader.SetUniform1f("material.shininess", 64.0f);//影响高光的半径
        SpotLight_Shader.SetUniform3f("ViewPos", camera.Position);
        SpotLight_Shader.SetUniform3f("spotLight.position", camera.Position);
        SpotLight_Shader.SetUniform3f("spotLight.direction", camera.Front);
        SpotLight_Shader.SetUniform3f("spotLight.ambient",  glm::vec3(1.0f, 1.0f, 1.0f));//光的环境强度
        SpotLight_Shader.SetUniform3f("spotLight.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));//光的漫反射强度
        SpotLight_Shader.SetUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));//光的镜面反射强度
        SpotLight_Shader.SetUniform1f("spotLight.constant", 1.0f);//常数项
        SpotLight_Shader.SetUniform1f("spotLight.linear", 0.09f);//一次项
        SpotLight_Shader.SetUniform1f("spotLight.quadratic", 0.032f);//二次项
        SpotLight_Shader.SetUniform1f("spotLight.cutoff", glm::cos(glm::radians(12.5f)));//聚光角度 内
        SpotLight_Shader.SetUniform1f("spotLight.outercutoff", glm::cos(glm::radians(17.5f)));//聚光角度 外
        
        shader.Bind();
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetUniformMat4("u_MVP", proj * view * model);
        OurModel.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}