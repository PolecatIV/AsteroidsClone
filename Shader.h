#pragma once


#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <conio.h>
#include <glew.h>

class Shader
{
public:
	bool error = false;
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "\nShader Error: cannot read the file\n" << std::endl;
			error = true;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar * fShaderCode = fragmentCode.c_str();
		
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];
	
		
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "\nShader Error: (Vertex Shader): compilation failed\n" << infoLog << std::endl;
			error = true;
		}
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "\nShader Error: (Fragment Shader): compilation failed\n" << infoLog << std::endl;
			error = true;
		}
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "\nShader Error: linking failed\n" << infoLog << std::endl;
			error = true;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (error)
		{
			std::cout << "\n...Press any key to continue...";
			_getch();
		}

	}
	
	void Use()
	{
		glUseProgram(this->Program);
	}

	void Unuse()
	{
		glUseProgram(0);
	}

	GLuint Shader::getUniformLoaction(const std::string& uniformName)
	{
		GLuint location = glGetUniformLocation(Program, uniformName.c_str());

		return location;
	}
};

#endif

