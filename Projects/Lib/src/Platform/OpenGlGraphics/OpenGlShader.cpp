#include "libpch.h"
#include "OpenGlShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lib
{
	OpenGlShader::OpenGlShader(const std::string& filepath)
	{
		CreateShader(ReadShaderFromSource(filepath));
	}

	OpenGlShader::~OpenGlShader()
	{
		glDeleteProgram(m_Id);
	}

	void OpenGlShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: Uniform '" << name << "' does not exist" << std::endl;
		}
		glProgramUniformMatrix4fv(m_Id, location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGlShader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: Uniform '" << name << "' does not exist" << std::endl;
		}
		glProgramUniform4fv(m_Id, location, 1, glm::value_ptr(value));
	}

	void OpenGlShader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: Uniform '" << name << "' does not exist" << std::endl;
		}
		glProgramUniform3fv(m_Id, location, 1, glm::value_ptr(value));
	}

	void OpenGlShader::SetFloat(const std::string& name, const float value)
	{

		GLint location = glGetUniformLocation(m_Id, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: Uniform '" << name << "' does not exist" << std::endl;
		}
		glProgramUniform1f(m_Id, location, value);
	}

	void OpenGlShader::SetInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: Uniform '" << name << "' does not exist" << std::endl;
		}
		glProgramUniform1i(m_Id, location, value);
	}

	void OpenGlShader::Bind()
	{
		glUseProgram(m_Id);
	}

	void OpenGlShader::UnBind()
	{
		glUseProgram(0);
	}

	ShaderSources OpenGlShader::ReadShaderFromSource(const std::string& filepath)
	{
		int shaderType = 0;
		ShaderSources Source;
		std::ifstream infile(filepath);
		if (!infile)
		{
			std::cout << "Unable to read file" << std::endl;
		}
		std::string line;
		while (std::getline(infile, line))
		{
			//std::cout << line << std::endl;
			if (line == "#vertex")
			{
				shaderType = 1;
			}
			else if (line == "#fragment")
			{
				shaderType = 2;
			}

			if (shaderType && !(line == "#vertex" || line == "#fragment"))
			{
				if (shaderType == 1)
				{
					Source.vertexShaderSource += line;
					Source.vertexShaderSource += '\n';
				}
				else if (shaderType == 2)
				{
					Source.fragmentShaderSource += line;
					Source.fragmentShaderSource += '\n';

				}
			}
		}

		//std::cout << Source.vertexShaderSource << std::endl;
		//std::cout << Source.fragmentShaderSource << std::endl;

		return Source;
	}

	void OpenGlShader::CreateShader(const ShaderSources& shaderSources)
	{
		m_Id = glCreateProgram();

		uint32_t vs = CompileShader(shaderSources.vertexShaderSource, GL_VERTEX_SHADER, m_Id);
		uint32_t fs = CompileShader(shaderSources.fragmentShaderSource, GL_FRAGMENT_SHADER, m_Id);
		glLinkProgram(m_Id);

		int success;
		char infoLog[512];

		glGetProgramiv(m_Id, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(m_Id, 512, NULL, infoLog);;
			std::cout << "ERROR::SHADER::PROGRAM ::LINKING_FAILED\n" <<
				infoLog << std::endl;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	uint32_t OpenGlShader::CompileShader(const std::string& shaderSource, uint32_t shaderType, uint32_t program)
	{
		uint32_t shader;
		const char* source = shaderSource.c_str();
		shader = glCreateShader(shaderType);

		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::string type = (shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
			std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}

		glAttachShader(program, shader);
		return shader;
	}

}