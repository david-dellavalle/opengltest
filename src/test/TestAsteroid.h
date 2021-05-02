#pragma once
#include <memory>
#include <glm/glm.hpp> 
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace test {

	class TestAsteroid : public Test
	{
	public:
		TestAsteroid();
		~TestAsteroid();

		void OnUpdate(float timeDelta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::vector<float> TEST_GenerateAsteroid(glm::vec2 origin, unsigned int numVertices,
			unsigned int avgRadius, unsigned int variance);
		void TEST_ApplyAsteroid();

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_ProjectionMatrix;

	};

}
