#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  class Shader
  {
  public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();

	void use();
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
  private:
	uint32_t m_RendererID;
  };

}