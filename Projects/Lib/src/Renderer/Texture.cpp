#include "libpch.h"
#include "Platform/OpenGlGraphics/OpenGlTexture.h"

namespace Lib
{
	std::shared_ptr<Texture> Texture::CreateTexture(const std::string& path)
	{
		return std::make_shared<OpenGlTexture>(path);
	}

}