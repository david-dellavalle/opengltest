#include "TestMVP.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glm/gtx/transform.hpp>

	static float vertices[] = {
		0.0f,   0.0f,   0.0f, 0.0f,
		100.0f, 0.0f,   1.0f, 0.0f,
		100.0f, 100.0f, 1.0f, 1.0f,
		0.0f,   100.0f, 0.0f, 1.0f,
	};
	static unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

namespace test {

	TestMVP::TestMVP()
		: modelTranslationA(glm::vec3(480.0f, 140.0f, 0.0f)),
		modelTranslationB(glm::vec3(250.0f, 300.0f, 0.0f)),
		m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
		m_ProjectionMatrix(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f)),
		m_VAO(std::make_unique<VertexArray>()),
		m_VBO(std::make_unique<VertexBuffer>(vertices, sizeof(vertices))),
		m_IBO(std::make_unique<IndexBuffer>(indices, 6))
	{
		m_Texture = std::make_unique<Texture>("res/textures/pfp.png");
		m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);
		glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
	}

	TestMVP::~TestMVP() 
	{
	
	}

	void TestMVP::OnUpdate(float timeDelta)
	{

	}

	void TestMVP::OnRender()
	{
		Renderer renderer; // Free, because renderer has no member variables, thus obj only 1 byte
		renderer.Clear();

		m_Shader->Bind();
		m_Texture->Bind(0);

		glm::mat4 modelMatrixA = glm::translate(glm::mat4(1.0f), modelTranslationA);
		glm::mat4 mvp = m_ProjectionMatrix * m_ViewMatrix * modelMatrixA;
		m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

		glm::mat4 modelMatrixB = glm::translate(glm::mat4(1.0f), modelTranslationB);
		mvp = m_ProjectionMatrix * m_ViewMatrix * modelMatrixB;
		m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
	}

	void TestMVP::OnImGuiRender()
	{
		ImGui::SliderFloat3("A", &modelTranslationA.x, 0.0f, 700.0f);
		ImGui::SliderFloat3("B", &modelTranslationB.x, 0.0f, 700.0f);
	}

}
