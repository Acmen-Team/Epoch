#pragma once
#include "Epoch/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Epoch {

  class OpenGLShader : public Shader
  {
  public:
	OpenGLShader(const char* vertexPath, const char* fragmentPath);
	OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
	virtual ~OpenGLShader() override;

	virtual void use() const override;

	void UploadUniformInt(const std::string& name, const int value);
	void UploadUniformFloat(const std::string& name, const float value);
	void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
	void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
	void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

	void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
  private:
	uint32_t m_RendererID;
  };

}