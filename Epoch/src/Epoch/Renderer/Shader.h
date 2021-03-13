#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  class Shader
  {
  public:
	virtual ~Shader() = default;

	virtual void use() const = 0;

	static Shader* Create(const char* vertexPath, const char* fragmentPath);
	static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
  };

}