#include "libpch.h"
#include "OpenGlTexture.h"

#include <stb_image.h>
#include<glad/glad.h>


namespace Lib
{
	OpenGlTexture::OpenGlTexture(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
			glTextureStorage2D(m_ID, 1, internalFormat, width, height);

			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
			std::cout << "Loaded texture from file:" << path << std::endl;
		}
		else
		{
			std::cout << "Unable to load texture from file: " << path << std::endl;
		}
	}

	OpenGlTexture::~OpenGlTexture()
	{
		glDeleteTextures(1, &m_ID);
	}

	void OpenGlTexture::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_ID);
	}

}