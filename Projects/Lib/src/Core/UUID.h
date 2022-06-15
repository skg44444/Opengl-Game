#pragma once
#include <chrono>

namespace Lib
{
	class UUID
	{
	public:
		UUID() 
		{
			m_ID = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
		UUID(uint64_t id)
		{
			m_ID = id;
		}
		~UUID()
		{

		}

		operator uint64_t() { return (uint64_t)m_ID; }
	private:
		uint64_t m_ID;
	};
}