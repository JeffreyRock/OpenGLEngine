#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class MyClass
{
private:
	std::string vertex, frag;
	std::ifstream vertexShaderFile, FragmentShader;

	const char* vertexShaderData, * fragmentShaderData;

	unsigned int vertex, fragment;

public:
	unsigned int ID; 

	Shader(const char* vertexPath, const char fragmentPath);

	//
};

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}
