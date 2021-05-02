#include <glad/glad.h>
#include <imgui/imgui.h>

#include "TestMenu.h"

namespace test {

	test::TestMenu::TestMenu(Test*& currentTestPtr)
		: m_CurrentTest(currentTestPtr)
	{
	}

	TestMenu::~TestMenu()
	{
	}

	void test::TestMenu::OnRender()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void test::TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}
