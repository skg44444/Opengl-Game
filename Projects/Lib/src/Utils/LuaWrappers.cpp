#include "libpch.h"
#include "LuaWrappers.h"
#include "Scene/Components.h"

namespace Lib
{
	void LuaWrappers::SetEntity(Entity entity)
	{
		auto& e = entity;
		m_CurrentEntity = e;
	}
	int LuaWrappers::wrap_Sum(lua_State* L)
	{
		int a = lua_tonumber(L, 1);
		int b = lua_tonumber(L, 2);

		lua_pushnumber(L, a + b);
		return 1;
	}

	int LuaWrappers::wrap_Move(lua_State* L)
	{
		LuaWrappers* object = static_cast<LuaWrappers*>(lua_touserdata(L, 1));
		float vx = (float)lua_tonumber(L, 2);
		float vy = (float)lua_tonumber(L, 3);
		float vz = (float)lua_tonumber(L, 4);

		object->Move(vx, vy, vz);
		return 0;
	}

	void LuaWrappers::Move(float vx, float vy, float vz)
	{
		auto& t = m_CurrentEntity.GetComponents<TransformComponent>().Translation;
		t.x += vx;
		t.y += vy;
		t.z += vz;
		std::cout << "Translating by :" << vx << std::endl;
	}

}