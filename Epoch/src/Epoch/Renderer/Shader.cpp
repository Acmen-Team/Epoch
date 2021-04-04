#include "eppch.h"

#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Epoch {

  std::shared_ptr<Shader> Shader::Create(const std::string& filePath)
  {
	return std::make_shared(OpenGLShader(filePath));
  }

  std::shared_ptr<Shader> Shader::Create(const char* vertexPath, const char* fragmentPath)
  {
	return std::make_shared(OpenGLShader(vertexPath, fragmentPath));
  }

  std::shared_ptr<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
  {
	return std::make_shared(OpenGLShader(vertexSource, fragmentSource));
  }

  void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
  {
	auto& name = shader->GetName();
	Add(name, shader);
  }

  void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
  {
	EP_CORE_ASSERT(!Exists(name), "Shader already exists!");
	m_Shaders[name] = shader;
  }

  std::shared_ptr<Epoch::Shader> ShaderLibrary::Load(const std::string& filepath)
  {
	auto shader = Shader::Create(filepath);
	Add(shader);
	return shader;
  }

  std::shared_ptr<Epoch::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
  {
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
  }

  std::shared_ptr<Epoch::Shader> ShaderLibrary::Get(const std::string& name)
  {
	EP_CORE_ASSERT(Exists(name), "Shader not found!");
	return m_Shaders[name];
  }

  bool ShaderLibrary::Exists(const std::string& name) const
  {
	return m_Shaders.find(name) != m_Shaders.end();
  }

}