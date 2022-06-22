#pragma once

#include <glm/glm.hpp>

namespace Lib
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetInt(const std::string& name, const int value) = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		
		static std::shared_ptr<Shader> CreateShader(const std::string& filepath);
	};
}