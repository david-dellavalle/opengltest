#include <cstdlib>
#include <iostream>
#include <array>

#include <glad/glad.h>
#include <glm/gtx/transform.hpp>
#include <imgui/imgui.h>

#include "TestAsteroid.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "input/Keys.h"


namespace test {

	static int asteroidNumVert = 10;
	static int asteroidVariance = 10;
	static int asteroidRadius = 100;
	static float asteroidPosition[2] = { 0.0f, 0.0f };

	TestAsteroid::TestAsteroid()
		: m_VAO(std::make_unique<VertexArray>()),
		m_Shader(std::make_unique<Shader>("res/shaders/one-color.shader")),
		m_ProjectionMatrix(glm::ortho(0.0f, 800.0f, 0.0f, 600.0f)),
		m_ViewMatrix(glm::translate(glm::vec3(0.0f, 100.0f, 0.0f))),
		m_CameraOffset(glm::vec3(0.0f, 0.0f, 0.0f))
	{

		TEST_ApplyAsteroid();

		//VertexBufferLayout layout;
		//layout.Push<float>(2);
		//m_VAO->AddBuffer(*m_VBO, layout);

		m_VBO->Bind();
		m_VAO->Bind();
		m_Shader->Bind();
		m_IBO->Bind();

		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		m_MVP = m_ProjectionMatrix * m_ViewMatrix;
		m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", m_MVP);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	TestAsteroid::~TestAsteroid()
	{
	}

	void TestAsteroid::OnUpdate(float timeDelta)
	{
		if (Keys::getKey('W'))
		{
			m_CameraOffset.y -= 280 * timeDelta;
		}
		if (Keys::getKey('A'))
		{
			m_CameraOffset.x += 280 * timeDelta;
		}
		if (Keys::getKey('S'))
		{
			m_CameraOffset.y += 280 * timeDelta;
		}
		if (Keys::getKey('D'))
		{
			m_CameraOffset.x -= 280 * timeDelta;
		}

		m_ViewMatrix = glm::translate(m_CameraOffset);
	}

	void TestAsteroid::OnRender()
	{
		Renderer renderer;
		renderer.Clear();

		m_MVP = m_ProjectionMatrix * m_ViewMatrix;
		m_Shader->SetUniformMat4f("u_ModelViewProjectionMatrix", m_MVP);
		
		// Enable wireframe made
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
	}

	void TestAsteroid::OnImGuiRender()
	{
		ImGui::SliderInt("# Vertices", &asteroidNumVert, 3, 100);
		ImGui::SliderInt("Variance", &asteroidVariance, 0, 300);
		ImGui::SliderFloat2("Position", asteroidPosition, -300.0f, 300.0f);
		if (ImGui::Button("Update"))
		{
			TEST_ApplyAsteroid();
		}
	}

	void TestAsteroid::TEST_ApplyAsteroid()
	{
		glm::vec2 origin = glm::vec2(asteroidPosition[0], asteroidPosition[1]);
		std::vector<float> asteroidVertices = TEST_GenerateAsteroid(origin, asteroidNumVert, asteroidRadius, asteroidVariance);
		std::vector<unsigned int> indices;


		for (int i = 0; i < asteroidNumVert; i+=2)
		{
			indices.emplace_back(i);
			if (i == asteroidNumVert - 1)
			{
				indices.emplace_back(0);
				indices.emplace_back(1);
			}
			else 
			{
				indices.emplace_back(i + 1);
				if (i + 2 > asteroidNumVert - 1)
					indices.emplace_back(0);
				else
					indices.emplace_back(i + 2);
			}
		}

		m_VBO = std::make_unique<VertexBuffer>(asteroidVertices.data(), sizeof(float) * asteroidNumVert * 2);
		m_IBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());

		VertexBufferLayout layout;
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);

		m_VAO->Bind();
		m_VBO->Bind();
		m_IBO->Bind();

	}

	std::vector<float> TestAsteroid::TEST_GenerateAsteroid(glm::vec2 origin,  unsigned int numVertices,
		unsigned int avgRadius, unsigned int variance)
	{
		std::vector<float> vertices;
		vertices.reserve((size_t)numVertices * 2);

		// Using (0, 0) as center of asteroid
		float angleDelta = 2 * 3.14159f / numVertices;
		float angle = 0;
		for (int i = 0; i < numVertices; i++)
		{
			unsigned int r;
			if (variance != 0)
				r = avgRadius + (rand() % variance - variance / 2);
			else
				r = avgRadius;
			
			float x = r * cos(angle) + origin.x;
			float y = r * sin(angle) + origin.y;
			
			vertices.emplace_back(x);
			vertices.emplace_back(y);

			angle += angleDelta;
		}

		return vertices;
	}
}
