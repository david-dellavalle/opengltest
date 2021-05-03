#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "input/Keys.h"
#include "input/Mouse.h"

#include "test/TestClearColor.h"
#include "test/TestTexture.h"
#include "test/TestMVP.h"
#include "test/TestMenu.h"
#include "test/TestAsteroid.h"

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Mouse::setMousePosition((float)xpos, (float)ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Presed left mouse button" << "\n";
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Keys::setKey('W', true);
	else
		Keys::setKey('W', false);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Keys::setKey('A', true);
	else
		Keys::setKey('A', false);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Keys::setKey('S', true);
	else
		Keys::setKey('S', false);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Keys::setKey('D', true);
	else
		Keys::setKey('D', false);
}


int main()
{

	static const unsigned int SCREEN_WIDTH = 800;
	static const unsigned int SCREEN_HEIGHT = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// GLFW Callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	/*
		OpenGL
	*/
	// Blending
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//Renderer renderer;

	/*
		Set up ImGui
	*/
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);

	glfwSwapInterval(1); // V-Sync

	test::Test* currentTest = nullptr;
	test::TestMenu* menu = new test::TestMenu(currentTest);
	currentTest = menu;

	menu->RegisterTest<test::TestClearColor>("Clear Color");
	menu->RegisterTest<test::TestTexture>("Texture");
	menu->RegisterTest<test::TestMVP>("MVP");
	menu->RegisterTest<test::TestAsteroid>("Asteroid");

	std::chrono::time_point<std::chrono::steady_clock> timeAtPreviousFrame;
	auto timeAtCurrentFrame = std::chrono::high_resolution_clock::now();

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		timeAtPreviousFrame = timeAtCurrentFrame;
		timeAtCurrentFrame = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> previousFrameDuration = timeAtCurrentFrame - timeAtPreviousFrame;

		// input
		processInput(window);

		if (currentTest)
		{
			currentTest->OnUpdate(previousFrameDuration.count());
			currentTest->OnRender();
		}

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Debug");                          

			if (currentTest) 
			{
				currentTest->OnImGuiRender();
			}
			if (currentTest != menu && ImGui::Button("Back"))
			{
				delete currentTest;
				currentTest = menu;
			}


            ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

		// Rendering ImGui
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete currentTest;
	if (currentTest != menu)
		delete menu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}