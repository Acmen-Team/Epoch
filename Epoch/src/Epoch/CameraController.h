#pragma once

#include "Epoch/Core/Timestep.h"
#include "Epoch/Scene/ScriptableEntity.h"

namespace Epoch {

  class CameraController : public ScriptableEntity
  {
  public:
	void OnCreate();

	void OnUpdate(Timestep timestep);

	void OnDestroy();
  private:

  };

}