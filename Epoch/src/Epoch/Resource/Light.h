#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  struct LightData
  {
	glm::vec3 Position;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	LightData()
	{
	  Position = glm::vec3(0.0f, 0.7f, 0.0f);
	  Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	  Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	  Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

  };

  class Light
  {
  public:
	static LightData* Create();
  };

}