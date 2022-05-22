#include "libpch.h"
#include "Entity.h"

namespace Lib
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{

	}
}