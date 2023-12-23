#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBufferLayout.h"
#include "renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "tests/TestClearColor.h"

int main(void)
{
    if (!glfwInit())//GLFW��ʼ��
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//ָ�������������ı�����ݵĿͻ���API�汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ָ�������������ĵ�OpenGL�������ļ�Ϊ���İ汾

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", nullptr, nullptr);//���ߣ���������
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);//֪ͨGLFW�����ڵ�����������Ϊ��ǰ�̵߳���������

    //GLCall(glfwSwapInterval(1));

    if (glewInit() != GLEW_OK)//GLEW��ʼ��
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //���㡢����
        float positions[] = {
            -50.0f,-50.0f,0.0f,0.0f,
             50.0f,-50.0f,1.0f,0.0f,
             50.0f, 50.0f,1.0f,1.0f,
            -50.0f, 50.0f,0.0f,1.0f,
        };

        //���㻺��
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));//ʹ�ܻ��
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//����Src_ALPHA��dest_ALPHA

        //�󶨶�������
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj  = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);//����ͶӰ����
        glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));//ģ������ƶ�

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/texture/OpenGL.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
        texture.Unbind();

        Render render;
        
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);

        test::Test* currentTest = nullptr;
        test::TestMenu* TestMenu = new test::TestMenu(currentTest);
        currentTest = TestMenu;

        TestMenu->RegisterTest<test::TestClearColor>("Clear Color");

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            render.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdata(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != TestMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = TestMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);//ģ������������
                glm::mat4 mvp = proj * view * model;//���������
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                render.Draw(va, ib, shader);
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);//ģ������������
                glm::mat4 mvp = proj * view * model;//���������
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                render.Draw(va, ib, shader);
            }

            texture.Bind();
            shader.SetUniform1i("u_Texture", 0);

            {
                ImGui::SliderFloat3("TranslationA ", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("TranslationB ", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Applocation average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);//������ɫ���壬��Ϊ�����ʾ����Ļ��
            glfwPollEvents();//��������¼�����
        }
        delete currentTest;
        if (currentTest != TestMenu)
            delete TestMenu;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}