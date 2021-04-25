#pragma once
#include <memory>

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Texture.h"

namespace test
{

	class TestMVP : public Test
	{
	public:
		TestMVP();
		~TestMVP();

		void OnUpdate(float timeDelta) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;

		glm::vec3 modelTranslationA;
		glm::vec3 modelTranslationB;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
	};
}
