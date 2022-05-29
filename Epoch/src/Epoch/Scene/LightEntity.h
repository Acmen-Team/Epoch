#pragma once

#include "Entity.h"

namespace Epoch {

  struct LightProperty
  {
	glm::vec4 Color;
	float Intensity;
	float Range;
	float SpotAngle;

	int LightType;

	LightProperty()
	{
	  Color = glm::vec4(0.2f, 0.2f, 0.3f, 1.0f);
	  Range = 5;
	  Intensity = 1.0;
	  SpotAngle = 15;

	  LightType = 0;
	}
  };

  class LightEntity : public Entity
  {
  public:
	LightEntity(entt::entity handle, Scene* scene) : Entity(handle, scene)
	{
	  m_LightData = new LightProperty();
	}
	~LightEntity();

	LightProperty* GetProperty() { return m_LightData; }
  private:
	LightProperty* m_LightData;
  };

}