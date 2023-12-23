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
    if (!glfwInit())//GLFW初始化
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//指定创建的上下文必须兼容的客户端API版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//指定创建的上下文的OpenGL的配置文件为核心版本

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", nullptr, nullptr);//宽，高，窗口名字
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);//通知GLFW将窗口的上下文设置为当前线程的主上下文

    //GLCall(glfwSwapInterval(1));

    if (glewInit() != GLEW_OK)//GLEW初始化
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

            glfwSwapBuffers(window);//交换颜色缓冲，作为输出显示在屏幕上
            glfwPollEvents();//检查有无事件触发
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