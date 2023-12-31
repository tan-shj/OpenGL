#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = 800.0 / 2.0;
GLfloat lastY = 600.0 / 2.0;

bool keys[1024];
bool firstMouse = true;
GLfloat aspect = glm::radians(45.0f);

struct vec3 { float x; float y; float z; };
struct vec2 { float x; float y;};

struct Vertex
{
    vec3 position;
    vec2 texcoords;
};

static std::array<Vertex, 4> CreatQuadz(float x, float y, float z)
{
    float size = 1.0f;

    Vertex v0;
    v0.position = { x, y, z };
    v0.texcoords = { 0.0f, 0.0f };


    Vertex v1;
    v1.position = { x+size, y, z };
    v1.texcoords = { 1.0f, 0.0f };

      
    Vertex v2;
    v2.position = { x+size, y+size, z };
    v2.texcoords = { 1.0f, 1.0f };


    Vertex v3;
    v3.position = { x, y+size, z };
    v3.texcoords = { 0.0f, 1.0f };


    return { v0,v1,v2,v3 };
}

static std::array<Vertex, 4> CreatQuadx(float x, float y, float z)
{
    float size = 1.0f;

    Vertex v0;
    v0.position = { x, y, z };
    v0.texcoords = { 0.0f, 0.0f };


    Vertex v1;
    v1.position = { x, y + size, z};
    v1.texcoords = { 1.0f, 0.0f };


    Vertex v2;
    v2.position = { x, y + size, z + size};
    v2.texcoords = { 1.0f, 1.0f };


    Vertex v3;
    v3.position = { x, y, z + size};
    v3.texcoords = { 0.0f, 1.0f };


    return { v0,v1,v2,v3 };
}

static std::array<Vertex, 4> CreatQuady(float x, float y, float z)
{
    float size = 1.0f;

    Vertex v0;
    v0.position = { x, y, z };
    v0.texcoords = { 0.0f, 0.0f };


    Vertex v1;
    v1.position = { x + size, y, z };
    v1.texcoords = { 1.0f, 0.0f };


    Vertex v2;
    v2.position = { x + size, y, z + size };
    v2.texcoords = { 1.0f, 1.0f };


    Vertex v3;
    v3.position = { x, y, z + size };
    v3.texcoords = { 0.0f, 1.0f };


    return { v0,v1,v2,v3 };
}

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

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);//按键
    glfwSetCursorPosCallback(window, mouse_callback);//鼠标
    glfwSetScrollCallback(window, scroll_callback);//滚轮

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    //x,y,z   vertex  color  texcoord
    /*GLfloat vertices[] = {
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
    };*/

    GLuint indices[] = {
        0,   1,  2,  2,  3,  0,
        4,   5,  6,  6,  7,  4,
        8,   9, 10, 10, 11,  8,
       12,  13, 14, 14, 15, 12,
       16,  17, 18, 18, 19, 16,
       20,  21, 22, 22, 23, 20
    };

    auto q0 = CreatQuadz(-0.5f, -0.5f, -0.5f);
    auto q1 = CreatQuadz(-0.5f, -0.5f,  0.5f);
    auto q2 = CreatQuadx(-0.5f, -0.5f, -0.5f);
    auto q3 = CreatQuadx( 0.5f, -0.5f, -0.5f); 
    auto q4 = CreatQuady(-0.5f, -0.5f, -0.5f);
    auto q5 = CreatQuady(-0.5f,  0.5f, -0.5f);
    Vertex vertices[24];
    memcpy(vertices+q0.size()*0, q0.data(), q0.size() * sizeof(Vertex));
    memcpy(vertices+q0.size()*1, q1.data(), q1.size() * sizeof(Vertex));
    memcpy(vertices+q0.size()*2, q2.data(), q2.size() * sizeof(Vertex));
    memcpy(vertices+q0.size()*3, q3.data(), q3.size() * sizeof(Vertex));
    memcpy(vertices+q0.size()*4, q4.data(), q4.size() * sizeof(Vertex));
    memcpy(vertices+q0.size()*5, q5.data(), q5.size() * sizeof(Vertex));

    GLCall(glEnable(GL_BLEND));//使能混合
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置Src_ALPHA和dest_ALPHA

    //绑定顶点数组
    GLuint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(nullptr, sizeof(Vertex) * 1000, GL_DYNAMIC_DRAW);

    VertexBufferLayout layout;
    layout.Push<float>(3);//vertex
    //layout.Push<float>(3);//color
    layout.Push<float>(2);//texcoord
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 36);

    Shader shader("res/Shader/Basic.shader");
    shader.Bind();

    Texture texture1("res/Texture/OpenGL.jpg", GL_REPEAT);
    Texture texture2("res/Texture/ChernoLogo.png",GL_REPEAT);
    texture1.Bind(0);
    texture2.Bind(1);
    //int samples[2] = { 0, 1 };
    //shader.SetUniform1iv("u_Texture", 2, samples);
    shader.SetUniform1i("u_Texture1", 0);
    shader.SetUniform1i("u_Texture2", 1);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    texture1.Unbind();
    texture2.Unbind();

    Render render;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        do_movement();

        vb.Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render.Clear();
        
        shader.Bind(); 
        texture1.Bind(0);
        texture2.Bind(1);
 
        {
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 proj = glm::perspective(aspect, 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 MVP = proj * view * model;
            shader.SetUniformMat4("u_MVP", MVP);
            render.Draw(va, ib, shader);
        }
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 0.01f;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (aspect >= 1.0f && aspect <= 45.0f)
        aspect -= yoffset;
    if (aspect <= 1.0f)
        aspect = 1.0f;
    if (aspect >= 45.0f)
        aspect = 45.0f;
}
