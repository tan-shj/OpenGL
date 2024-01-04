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

    //x,y,z   vertex  color  texcoord
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    GLuint indices[] = {
        0, 1, 2, // ��һ��������
        2, 3, 0  // �ڶ���������
    };

    GLCall(glEnable(GL_BLEND));//ʹ�ܻ��
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//����Src_ALPHA��dest_ALPHA

    //�󶨶�������
    GLuint vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    VertexArray va;
    VertexBuffer vb(vertices, 8 * 4 * sizeof(GLfloat));

    VertexBufferLayout layout;
    layout.Push<float>(3);//vertex
    layout.Push<float>(3);//color
    layout.Push<float>(2);//texcoord
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/Shader/Basic.shader");
    shader.Bind();

    Texture texture1("res/Texture/OpenGL.jpg",GL_CLAMP_TO_EDGE);
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
        render.Clear();
        
        shader.Bind(); 
        texture1.Bind(0);
        texture2.Bind(1);
 
        {
            glm::mat4 trans = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0, 0.0, 1.0));//��Z����ת90��/*glm::radians(90.0f)*/
            //trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));//ÿ�������ű���
            trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));//λ��
            shader.SetUniformMat4("transform", trans);
            render.Draw(va, ib, shader);
        }

        {
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));//λ��
            trans = glm::scale(trans, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), sin(glfwGetTime())));//ÿ�������ű���
            shader.SetUniformMat4("transform", trans);
            render.Draw(va, ib, shader);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
