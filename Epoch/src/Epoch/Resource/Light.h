#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  struct LightData
  {
	glm::vec3 Position;
	glm::vec3 Direction;

	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;

	float CutOff;

	LightData()
	{
	  Position = glm::vec3(0.0f, 0.7f, 0.0f);
	  Direction = glm::vec3(-0.2f, -1.0f, -0.3f);

	  Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	  Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	  Specular = glm::vec3(1.0f, 1.0f, 1.0f);

	  Constant = 1.0f;
	  Linear = 0.09f;
	  Quadratic = 0.032f;

	  CutOff = glm::cos(glm::radians(12.5f));
	}
  };

  class Light
  {
  public:
	static LightData* Create();
  };

}