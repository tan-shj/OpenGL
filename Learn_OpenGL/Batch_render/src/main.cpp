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
glm::vec3 cameraPos = glm::vec3(5.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(-5.0f, 0.0f, -5.0f);
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
    vec3 normal;
};

static std::array<Vertex, 4> CreatQuadz(float x, float y, float z)
{
    float size = 1.0f;
    float direction;

    z < 0 ? (direction = -1.0f) : (direction = 1.0f);

    Vertex v0;
    v0.position = { x, y, z };
    v0.texcoords = { 0.0f, 0.0f };
    v0.normal = { 0.0f, 0.0f, direction };

    Vertex v1;
    v1.position = { x+size, y, z };
    v1.texcoords = { 1.0f, 0.0f };
    v1.normal = { 0.0f, 0.0f, direction };
      
    Vertex v2;
    v2.position = { x+size, y+size, z };
    v2.texcoords = { 1.0f, 1.0f };
    v2.normal = { 0.0f, 0.0f, direction };

    Vertex v3;
    v3.position = { x, y+size, z };
    v3.texcoords = { 0.0f, 1.0f };
    v3.normal = { 0.0f, 0.0f, direction };

    return { v0,v1,v2,v3 };
}

static std::array<Vertex, 4> CreatQuadx(float x, float y, float z)
{
    float size = 1.0f;
    float direction;

    x < 0 ? (direction = -1.0f) : (direction = 1.0f);

    Vertex v0;
    v0.position = { x, y, z };
    v0.texcoords = { 0.0f, 0.0f };
    v0.normal = { direction, 0.0f, 0.0f };

    Vertex v1;
    v1.position = { x, y + size, z};
    v1.texcoords = { 1.0f, 0.0f };
    v1.normal = { direction, 0.0f, 0.0f };

    Vertex v2;
    v2.position = { x, y + size, z + size};
    v2.texcoords = { 1.0f, 1.0f };
    v2.normal = { direction, 0.0f, 0.0f };

    Vertex v3;
    v3.position = { x, y, z + size};
    v3.texcoords = { 0.0f, 1.0f };
    v3.normal = { direction, 0.0f, 0.0f };

    return { v0,v1,v2,v3 };
}

static std::array<Vertex, 4> CreatQuady(float x, float y, float z)
{
    float size = 1.0f;
    float direction;

    y < 0 ? (direction = -1.0f) : (direction = 1.0f);

    Vertex v0;
    v0.position = { x, y, z };
    v0.texcoords = { 0.0f, 0.0f };
    v0.normal = { 0.0f, direction, 0.0f };

    Vertex v1;
    v1.position = { x + size, y, z };
    v1.texcoords = { 1.0f, 0.0f };
    v1.normal = { 0.0f, direction, 0.0f };

    Vertex v2;
    v2.position = { x + size, y, z + size };
    v2.texcoords = { 1.0f, 1.0f };
    v2.normal = { 0.0f, direction, 0.0f };

    Vertex v3;
    v3.position = { x, y, z + size };
    v3.texcoords = { 0.0f, 1.0f };
    v3.normal = { 0.0f, direction, 0.0f };

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // Positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
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
    layout.Push<float>(2);//texcoord
    layout.Push<float>(3);//normal
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 36);
 
    Shader shader1("res/Shader/Basic.shader");
    Shader shader2("res/Shader/Light.shader");

    Texture texture1("res/Texture/container2.png", GL_REPEAT);
    Texture texture2("res/Texture/container2_specular.png", GL_REPEAT);
    texture1.Bind(0);
    texture2.Bind(1);

    shader1.Bind();
    shader1.SetUniform1i("material.diffuse", 0);
    shader1.SetUniform1i("material.specular", 1);//物体受到镜面光照影响的颜色  镜面（反射）高光
    shader1.SetUniform1f("material.shininess", 64.0f);//影响高光的半径
    //shader1.SetUniform3f("spotLight.ambient",  glm::vec3(1.0f, 1.0f, 1.0f));//光的环境强度
    //shader1.SetUniform3f("spotLight.diffuse",  glm::vec3(0.8f, 0.8f, 0.8f));//光的漫反射强度
    //shader1.SetUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));//光的镜面反射强度
    //shader1.SetUniform1f("spotLight.constant", 1.0f);//常数项
    //shader1.SetUniform1f("spotLight.linear", 0.09f);//一次项
    //shader1.SetUniform1f("spotLight.quadratic", 0.032f);//二次项
    //shader1.SetUniform1f("spotLight.cutoff", glm::cos(glm::radians(12.5f)));//聚光角度 内
    //shader1.SetUniform1f("spotLight.outercutoff", glm::cos(glm::radians(17.5f)));//聚光角度 外

    shader1.SetUniform3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader1.SetUniform3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader1.SetUniform3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    shader1.SetUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    shader1.SetUniform3f("pointLights[0].position", pointLightPositions[0]);
    shader1.SetUniform3f("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader1.SetUniform3f("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader1.SetUniform3f("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader1.SetUniform1f("pointLights[0].constant", 1.0f);
    shader1.SetUniform1f("pointLights[0].linear", 0.09f);
    shader1.SetUniform1f("pointLights[0].quadratic", 0.032f);

    shader1.SetUniform3f("pointLights[1].position", pointLightPositions[1]);
    shader1.SetUniform3f("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader1.SetUniform3f("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader1.SetUniform3f("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader1.SetUniform1f("pointLights[1].constant", 1.0f);
    shader1.SetUniform1f("pointLights[1].linear", 0.09f);
    shader1.SetUniform1f("pointLights[1].quadratic", 0.032f);

    shader1.SetUniform3f("pointLights[2].position", pointLightPositions[2]);
    shader1.SetUniform3f("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader1.SetUniform3f("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader1.SetUniform3f("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader1.SetUniform1f("pointLights[2].constant", 1.0f);
    shader1.SetUniform1f("pointLights[2].linear", 0.09f);
    shader1.SetUniform1f("pointLights[2].quadratic", 0.032f);

    shader1.SetUniform3f("pointLights[3].position", pointLightPositions[3]);
    shader1.SetUniform3f("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader1.SetUniform3f("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    shader1.SetUniform3f("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader1.SetUniform1f("pointLights[3].constant", 1.0f);
    shader1.SetUniform1f("pointLights[3].linear", 0.09f);
    shader1.SetUniform1f("pointLights[3].quadratic", 0.032f);

    shader2.Bind();

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader1.Unbind();
    shader2.Unbind();
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
        
        texture1.Bind(0);
        texture2.Bind(1);
        
        shader1.Bind();
        shader1.SetUniform3f("ViewPos", cameraPos);//观察者（相机位置）
        //shader1.SetUniform3f("spotLight.direction", cameraFront);//光的方向
        //shader1.SetUniform3f("spotLight.position", cameraPos);//光的位置--指向物体
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//摄像机位置、目标位置、上向量
        glm::mat4 proj = glm::perspective(aspect, 800.0f / 600.0f, 0.1f, 100.0f); 
        
        {
            shader1.Bind(); 
            glm::mat4 model = glm::mat4(1.0f);
            shader1.SetUniformMat4("u_MVP", proj * view * model);
            render.Draw(va, ib, shader1);
        }

        {
            shader1.Bind();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.0f, 0.0f, 2.0f));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(0.3f));
            shader1.SetUniformMat4("u_MVP", proj * view * model);
            render.Draw(va, ib, shader1);
        }

        {
            shader2.Bind();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
            model = glm::scale(model, glm::vec3(0.3f));
            shader2.SetUniformMat4("u_MVP", proj * view * model);
            //render.Draw(va, ib, shader2);
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
