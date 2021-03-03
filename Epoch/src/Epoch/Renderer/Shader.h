#pragma once

namespace Epoch {

  class Shader
  {
  public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	~Shader();

	void use();
  private:
	uint32_t m_RendererID;
  };

}