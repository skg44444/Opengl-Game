#pragma once

#include "Renderer/Shader.h"

namespace Lib
{
	struct ShaderSources
	{
		std::string vertexShaderSource, fragmentShaderSource;
	};

	class OpenGlShader : public Shader
	{
	public:
		OpenGlShader(const std::string& filepath);
		~OpenGlShader();

		void SetMat4(const std::string& name, const glm::mat4& value) override;
		void SetVec4(const std::string& name, const glm::vec4& value) override;
		void SetVec3(const std::string& name, const glm::vec3& value) override;
		void SetFloat(const std::string& name, const float value) override;
		void SetInt(const std::string& name, const int value) override;
		
		void Bind() override;
		void UnBind() override;
	private:
		ShaderSources ReadShaderFromSource(const std::string& filepath);
		void CreateShader(const ShaderSources& shaderSources);
		uint32_t CompileShader(const std::string& shaderSource, uint32_t shaderType, uint32_t program);
	private:
		uint32_t m_Id;
	};
}