#pragma once

namespace Lib
{
	class Texture
	{
	public:
		static std::shared_ptr<Texture> CreateTexture(const std::string& path);
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) = 0;
	};
}