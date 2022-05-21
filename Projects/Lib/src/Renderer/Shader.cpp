#include "libpch.h"

#include "Platform/OpenGlGraphics/OpenGlShader.h"

namespace Lib
{
	std::shared_ptr<Shader> Shader::CreateShader(const std::string& filepath)
	{
		return std::make_shared<OpenGlShader>(filepath);
	}
}