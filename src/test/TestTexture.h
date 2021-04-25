#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Texture.h"

namespace test {

	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float timeDelta) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Texture m_Textures[2];
		unsigned int m_SelectedTexture;
		Shader m_Shader;
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_IBO;

	};

}
