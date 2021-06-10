#pragma once

#include "Scene.h"

namespace Epoch {

  class Entity
  {
  public:
	Entity() = default;
	Entity(const Entity& other) = default;
	Entity(entt::entity handle, Scene* scene);

	template<typename T>
	bool HasComponent()
	{
	  return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
	  EP_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
	  return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	void RemoveComponent()
	{
	  EP_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
	  m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	template<typename T>
	T& GetComponent()
	{
	  EP_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
	  return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	operator bool() const
	{
	  if (m_Scene == nullptr)return false;
	  return m_Scene->m_Registry.valid(m_EntityHandle);
	}

  private:
	entt::entity m_EntityHandle{ entt::null };
	Scene* m_Scene = nullptr;
  };

}