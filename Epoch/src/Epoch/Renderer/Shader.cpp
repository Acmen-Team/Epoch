#include "eppch.h"

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Epoch {

  Shader* Shader::Create(const char* vertexPath, const char* fragmentPath)
  {
	return new OpenGLShader(vertexPath, fragmentPath);
  }

  Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
  {
	return new OpenGLShader(vertexSource, fragmentSource);
  }

}