#include <iostream>
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include "TestTexture.h"

namespace test {
	

	float vertices[] = {
		250.0f, 150.0f, 0.0f, 0.0f, // 0
		550.0f, 150.0f, 1.0f, 0.0f, // 1
		550.0f, 450.0f, 1.0f, 1.0f, // 2
		250.0f, 450.0f, 0.0f, 1.0f, // 3
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	TestTexture::TestTexture()
		:
		m_Textures{ Texture("res/textures/pfp.png"), Texture("res/textures/alpha.png") },
		m_SelectedTexture(0),
		m_Shader("res/shaders/basic.shader"),
		m_VAO(),
		m_VBO(vertices, sizeof(vertices)),
		m_IBO(indices, 6)
	{

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO.AddBuffer(m_VBO, layout);
		m_VAO.Bind();

		m_IBO.Bind();

		m_Shader.Bind();
		glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		m_Shader.SetUniformMat4f("u_ModelViewProjectionMatrix", proj);

		m_Textures[0].Bind(0);

		m_Shader.SetUniform1i("u_Texture", 0); 

		GLCall(glClearColor(0.2f, 0.4f, 0.4f, 1.0f));

	}

	TestTexture::~TestTexture()
	{

	}

	void TestTexture::OnUpdate(float timeDelta)
	{

	}

	void TestTexture::OnRender()
	{
		//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void TestTexture::OnImGuiRender()
	{
		if ( ImGui::Button("PFP Texture"))
		{
			m_SelectedTexture = 0;
			m_Textures[m_SelectedTexture].Bind(0);
		}
		if (ImGui::Button("Nike Texture"))
		{
			m_SelectedTexture = 1;
			m_Textures[m_SelectedTexture].Bind(0);
		}
	}



	
}