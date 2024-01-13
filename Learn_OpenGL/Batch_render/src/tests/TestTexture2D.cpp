#include "TestTexture2D.h"
#include "Renderer.h"
#include <imgui/imgui.h>
#include "VertexBufferLayout.h"

namespace test
{
	TestTexture2D::TestTexture2D()
	: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        //���㡢����
        float positions[] = {
            -50.0f,-50.0f,0.0f,0.0f,
             50.0f,-50.0f,1.0f,0.0f,
             50.0f, 50.0f,1.0f,1.0f,
            -50.0f, 50.0f,0.0f,1.0f
        };

        //���㻺��
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));//ʹ�ܻ��
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//����Src_ALPHA��dest_ALPHA

        m_VAO = std::make_unique<VertexArray>();
		m_Vertexbuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float), GL_STATIC_DRAW);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_Vertexbuffer, layout);
        m_Indexbuffer = std::make_unique<IndexBuffer>(indices, 6);
		
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/texture/OpenGL.jpg", GL_CLAMP_TO_EDGE);
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdata(float daltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f,0.0f,0.0f,1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Render render;
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);//ģ������������
			glm::mat4 mvp = m_Proj * m_View * model;//���������
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			render.Draw(*m_VAO, *m_Indexbuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);//ģ������������
			glm::mat4 mvp = m_Proj * m_View * model;//���������
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			render.Draw(*m_VAO, *m_Indexbuffer, *m_Shader);
		}

	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA ", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB ", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Applocation average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}