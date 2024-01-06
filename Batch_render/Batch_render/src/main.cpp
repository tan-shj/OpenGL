#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

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

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    //x,y,z   vertex  color  texcoord
    GLfloat vertices[] = {
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

    GLuint indices[] = {
        0,   1,  2,  3,  4,  5,
        6,   7,  8,  9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };

    GLCall(glEnable(GL_BLEND));//使能混合
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置Src_ALPHA和dest_ALPHA

    //绑定顶点数组
    GLuint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(vertices, 36 * 5 * sizeof(GLfloat));

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
    shader.SetUniform1i("u_Texture1", 0);
    texture2.Bind(1);
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
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render.Clear();
        
        shader.Bind(); 
        texture1.Bind(0);
        texture2.Bind(1);
 
        {
            //glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));//模型矩阵--旋转(rotate)、平移(translate)、缩放(scale)
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * glm::radians(-90.0f), glm::vec3(0.5, 1.0, 0.0));//模型矩阵--旋转(rotate)、平移(translate)、缩放(scale)
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5.0));//观察矩阵--相机位置；通过旋转、平移将世界坐标转换到观察空间--相机只能移动，不能旋转
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);//投影矩阵--parameter->视野、宽高比、平截头体的近平面、平截头体的远平面
            glm::mat4 MVP = proj * view * model;
            shader.SetUniformMat4("u_MVP", MVP);
            render.Draw(va, ib, shader);
        }

        //{
            //glm::mat4 model = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));//模型矩阵--旋转(rotate)、平移(translate)、缩放(scale)
            //glm::mat4 model = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.0, 1.0, 0.0));
            //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
            //glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            //glm::mat4 MVP = proj * view * model;
            //shader.SetUniformMat4("u_MVP", view);
            //render.Draw(va, ib, shader);
        //}

       /* {
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.0, 0.0, 1.0));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -3.0));
            glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 MVP = proj * view * model;
            shader.SetUniformMat4("u_MVP", MVP);
            render.Draw(va, ib, shader);
        }*/

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
