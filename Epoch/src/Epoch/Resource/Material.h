#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  struct MaterialData
  {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;

	MaterialData()
	{
	  Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	  Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	  Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	  Shininess = 32.0f;
	}

  };

  class Material
  {
  public:
	static MaterialData* Create();
  };

}