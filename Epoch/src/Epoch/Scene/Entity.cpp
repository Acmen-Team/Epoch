#include "eppch.h"

#include "Entity.h"

namespace Epoch {

  Entity::Entity(entt::entity handle, Scene* scene)
	: m_EntityHandle(handle), m_Scene(scene)
  {
  }

}