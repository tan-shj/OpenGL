#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "renderer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

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

        test::Test* currentTest = nullptr;
        test::TestMenu* TestMenu = new test::TestMenu(currentTest);
        currentTest = TestMenu;

        TestMenu->RegisterTest<test::TestClearColor>("Clear Color");
        TestMenu->RegisterTest<test::TestTexture2D>("2D Texture");

        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

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