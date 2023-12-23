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
        //顶点、纹理
        float positions[] = {
            -50.0f,-50.0f,0.0f,0.0f,
             50.0f,-50.0f,1.0f,0.0f,
             50.0f, 50.0f,1.0f,1.0f,
            -50.0f, 50.0f,0.0f,1.0f,
        };

        //顶点缓存
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));//使能混合
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置Src_ALPHA和dest_ALPHA

        //绑定顶点数组
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

        glm::mat4 proj  = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);//正交投影矩阵
        glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));//模拟相机移动

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
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);//模型向左向上移
                glm::mat4 mvp = proj * view * model;//从右往左乘
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                render.Draw(va, ib, shader);
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);//模型向左向上移
                glm::mat4 mvp = proj * view * model;//从右往左乘
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