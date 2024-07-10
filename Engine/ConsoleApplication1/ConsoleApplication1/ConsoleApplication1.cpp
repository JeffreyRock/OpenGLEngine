// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

#include <vector>

#include "Shader.h"

using namespace std;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
};

unsigned int ImportShaders(const string FilePath, unsigned int module_type);
unsigned int Make_Shader(const string ShaderFilePath, const string FragFilePath);
void OpenGLFuctionThatDrawsOurTriangle();

int main(void)
{

	

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn Open GL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create Open GL window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize glad" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

	

	float vertices[] = {
		// positions // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
	};

	unsigned int indices[] = {
	0, 1, 2
	};

	//unsigned int ShaderProgram = Make_Shader("Shaders\\shader.txt", "Shaders\\Fragment.txt");
	shader ourShader("Shaders\\shader.txt", "Shaders\\Fragment.txt");
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffers, and then configure vertex attributes
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	//glUseProgram(ShaderProgram);

	while (!glfwWindowShouldClose(window))
	{
		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the triangle
		//glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		ourShader.use();
		ourShader.setFloat("someUniform", 1.0f);

		// Swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glDeleteProgram(ShaderProgram);
	//glDeleteProgram(ourShader);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

unsigned int Make_Shader(const string ShaderFilePath, const string FragFilePath) {
	std::vector<unsigned int> modules;
	modules.push_back(ImportShaders(ShaderFilePath, GL_VERTEX_SHADER));
	modules.push_back(ImportShaders(FragFilePath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);
	int success;
	char infoLog[512];
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
		return 0;
	}
	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}
	return shader;
};

unsigned int ImportShaders(const string FilePath, unsigned int module_type) {
	ifstream file;
	stringstream bufferLines;
	string line;
	file.open(FilePath);
	while (getline(file, line)) {
		bufferLines << line << "\n";
	}
	string shaderSource = bufferLines.str();
	const char* shaderSrc = shaderSource.c_str();
	bufferLines.str("");
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);

	glCompileShader(shaderModule);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
		return 0;
	}
	return shaderModule;
};