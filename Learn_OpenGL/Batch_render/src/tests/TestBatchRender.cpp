#include "TestBatchRender.h"

#include "Renderer.h"
#include <vendor/imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace test
{
	TestBatchRender::TestBatchRender()
		:m_Proj(glm::ortho(0.0f,800.f,0.0f,600.f,-1.0f,1.0f)),
		m_View(glm::translate(glm::mat4(1.0f),glm::vec3(-100,0,0))),
		m_Translation(glm::vec3(0,0,0))
	{
		float positions[] = {
			100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			200.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			100.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

			300.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			400.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			400.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			300.0f, 200.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 11 * 8 * sizeof(float), GL_STATIC_DRAW);
		
		VertexBufferLayout layout;
		layout.Push<float>(4);//position--x,y,z,w  wÆë´Î×ø±ê  
		layout.Push<float>(4);//color
		layout.Push<float>(2);//TexCoords
		layout.Push<float>(1);//TexIndex
		 
		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);
		m_Shader = std::make_unique<Shader>("res/Shader/Batch.shader");
		m_Shader->Bind();

		m_Texture[0] = std::make_unique<Texture>("res/Texture/ChernoLogo.png", GL_REPEAT);
		m_Texture[1] = std::make_unique<Texture>("res/Texture/OpenGL.jpg", GL_REPEAT);
		m_Texture[0]->Bind(0);
		m_Texture[1]->Bind(1);

		int samplers[2] = { 0,1 };
		m_Shader->SetUniform1iv("u_Texture", 2, samplers);
	}

	TestBatchRender::~TestBatchRender()
	{
	}

	void TestBatchRender::OnUpdata(float deltaTime)
	{
	}

	void TestBatchRender::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Render render;
		glm::mat4 MVP = m_Proj * m_View;
		m_Shader->Bind();
		m_Shader->SetUniformMat4("u_MVP", MVP);
		render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestBatchRender::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate);
	}
}
