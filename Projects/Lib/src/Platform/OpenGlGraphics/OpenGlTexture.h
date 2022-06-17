#pragma once

#include "Renderer/Texture.h"

namespace Lib
{
	class OpenGlTexture : public Texture
	{
	public:
		OpenGlTexture(const std::string& path);
		~OpenGlTexture();

		void Bind(uint32_t slot = 0) override;
	private:
		uint32_t m_ID;
	};
}