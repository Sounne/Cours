#include "GLShader.h"

#include <fstream>
#include <iostream>

#include "glew-2.0.0/include/GL/glew.h"

auto GLShader::LoadShader(uint32_t type, const char* path) -> bool
{
	// on presuppose type = GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER
	uint32_t shaderID;
	shaderID = glCreateShader(type);

	switch (type)
	{
		case GL_VERTEX_SHADER:
			_VertexShader = shaderID;
			break;
		case GL_FRAGMENT_SHADER:
			_FragmentShader = shaderID;
			break;
		default: break;
	}

	std::ifstream fileStream;
	fileStream.open(path, std::ios::binary);

	if (!fileStream.is_open())
		return false;

	auto begin = fileStream.tellg();

	fileStream.seekg(0, std::ios::end);
	auto length = fileStream.tellg() - begin;

	char* buffer = new char[length + 1];
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(buffer, length);
	buffer[length] = '\0';

	fileStream.close();

	glShaderSource(shaderID, 1, &buffer, nullptr);
	delete[] buffer;
	glCompileShader(shaderID);

	int compileSuccess = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess)
	{
		char errorBuffer[4096];
		int error_len;
		glGetShaderInfoLog(shaderID, 4096, &error_len, errorBuffer);
		std::cout << path << ", erreur dans le " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader : " << errorBuffer << std::endl;
		return false;
	}

	return true;
}

auto GLShader::Create() -> bool
{
	_Program = glCreateProgram();
	glAttachShader(_Program, _VertexShader);
	glAttachShader(_Program, _FragmentShader);
	glLinkProgram(_Program);
	//TODO : checker le resultat du linkage
	int linkSuccess = 0;
	glGetProgramiv(_Program, GL_LINK_STATUS, &linkSuccess);
	
	if (!linkSuccess)
	{
		char errorBuffer[4096];
		int error_len;
		glGetProgramInfoLog(_Program, 4096, &error_len, errorBuffer);
		std::cout << "erreur de linkage : " << errorBuffer << std::endl;
		return false;
	}
	return true;
}

auto GLShader::Destroy() -> void
{
	glDeleteShader(_VertexShader);
	glDeleteShader(_FragmentShader);
	glDeleteProgram(_Program);
}

auto GLShader::Bind() -> void
{
	glUseProgram(_Program);
}

auto GLShader::Unbind() -> void
{
	glUseProgram(0);
}