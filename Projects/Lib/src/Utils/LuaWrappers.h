#pragma once

#include "Scene/Entity.h"

#ifndef __LUA_INC_H__
#define __LUA_INC_H__

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#endif // __LUA_INC_H__

namespace Lib
{
	class LuaWrappers
	{
	public:
		LuaWrappers() {}
		void SetEntity(Entity entity); 

		static int wrap_Sum(lua_State* L);
		static int wrap_Move(lua_State* L);
	private:
		void Move(float vx, float vy, float vz);
	private:
		Entity m_CurrentEntity;
	};
}