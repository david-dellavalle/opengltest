#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{

	const unsigned int SCREEN_WIDTH = 800;
	const unsigned int SCREEN_HEIGHT = 600;

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	/*
		OpenGL
	*/

	float vertices[] = {
		-50.0f, -50.0f, 0.0f, 0.0f, // 0
		 50.0f, -50.0f, 1.0f, 0.0f, // 1
		 50.0f,  50.0f, 1.0f, 1.0f, // 2
		-50.0f,  50.0f, 0.0f, 1.0f, // 3
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	float quad1[] = {
		-0.76f, -0.5f,
		  0.0f, -0.5f,
		  0.0f,  0.26f,
		-0.76f,  0.26f,
	};

	unsigned int quad1_indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	// Blending
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	VertexArray vao;
	VertexBuffer vbo(vertices, sizeof(vertices));
	IndexBuffer ibo(indices, 6);
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	vao.AddBuffer(vbo, layout);

	VertexArray quad1VAO;
	VertexBuffer quad1VBO(quad1, sizeof(quad1));
	IndexBuffer quad1IBO(quad1_indices, 6);
	VertexBufferLayout quad1_layout;
	quad1_layout.Push<float>(2);
	quad1VAO.AddBuffer(quad1VBO, quad1_layout);

	// left, right, bottom, top, near, far
	glm::mat4 proj = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT, 0.0f, 1.0f); // Orthographic projection matrix -- mapping world space to normalized device coordinates
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // "View matrix"
	//glm::mat4 mvp = model * view * proj;

	// Basic yellow shader
	Shader yellowShader("res/shaders/yellow.shader");
	yellowShader.Bind();

	// Texture shaders
	Shader shader("res/shaders/basic.shader");
	shader.Bind();

	// Textures
	Texture texture("res/textures/pfp.png");
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0); // Sending texture to fragment shader ( to calculate pixel color ) ...However, it needs to know what area of the texture any particular location is ( giving texture coordinates )

	Renderer renderer;

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

    bool imgui_show_demo_window = true;
    bool imgui_show_another_window = false;
    ImVec4 imgui_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	glm::vec3 translationA = glm::vec3(200, 200, 0);
	glm::vec3 translationB = glm::vec3(400, 200, 0);

	// Render loop
	float r1 = 1.0f;
	float r_inc = 0.01f;
	while (!glfwWindowShouldClose(window))
	{
		if (r1 + r_inc >= 1.0f || r1 + r_inc <= 0.0f)
			r_inc *= -1;
		r1 += r_inc;

		// input
		processInput(window);

		// rendering commands here
		//glClearColor(r1, 0.3f, 0.3f, 1.0f);
		glClearColor(imgui_clear_color.x, imgui_clear_color.y, imgui_clear_color.z, 1.0f);
		renderer.Clear();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		renderer.Draw(quad1VAO, quad1IBO, yellowShader);

		shader.Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
			renderer.Draw(vao, ibo, shader);

		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_ModelViewProjectionMatrix", mvp);
			renderer.Draw(vao, ibo, shader);
		}

        {
            ImGui::Begin("Debug");                          

            ImGui::SliderFloat3("TranslationA", &translationA.x, 0.0f, SCREEN_WIDTH);            
			ImGui::SliderFloat3("TranslationB", &translationB.x, 0.0f, SCREEN_WIDTH);
            ImGui::ColorEdit3("clear color", (float*)&imgui_clear_color); 

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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}