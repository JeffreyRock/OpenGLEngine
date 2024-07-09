// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

#include <vector>

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
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
		
	};


	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	unsigned int EBO;
	unsigned int ShaderProgram = Make_Shader("Shaders\\shader.txt", "Shaders\\Fragment.txt");
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		
		float timeV = glfwGetTime();
		float GreenValue = (sin(timeV) / 2.0f) + 0.5f;
		float redValue = (cos(timeV) / 2.0f) + 0.5f;
		float yellowValue = (tan(timeV) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(ShaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, redValue, GreenValue, yellowValue, 1.0f);

		
	}
	glDeleteShader(ShaderProgram);
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