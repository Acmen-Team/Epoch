#pragma once

#include "Epoch/Core/Timestep.h"

#include "Entity.h"

namespace Epoch {

  class ScriptableEntity
  {
  public:
	virtual ~ScriptableEntity() = default;

	template<typename T>
	T& GetComponent()
	{
	  return m_Entity.GetComponent<T>();
	}
  protected:
	virtual void OnCreate() {}
	virtual void OnUpdate(Timestep timestep) {}
	virtual void OnDestroy() {}

  private:
	Entity m_Entity;
	friend class Scene;
  };

}
