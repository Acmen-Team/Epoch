#pragma once

#include <glm/glm.hpp>

namespace Epoch {

  class Shader
  {
  public:
	virtual ~Shader() = default;

	virtual void use() const = 0;
	virtual const std::string& GetName() const = 0;

	virtual void UploadUniformInt(const std::string& name, const int value) = 0;
	virtual void UploadUniformFloat(const std::string& name, const float value) = 0;
	virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value) = 0;
	virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value) = 0;
	virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value) = 0;

	virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) = 0;
	virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) = 0;

	static std::shared_ptr<Shader> Create(const std::string& filePath);
	static std::shared_ptr<Shader> Create(const char* vertexPath, const char* fragmentPath);
	static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
  };

  class ShaderLibrary
  {
  public:
	  void Add(const std::shared_ptr<Shader>& shader);
	  void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
	  std::shared_ptr<Shader> Load(const std::string& filepath);
	  std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

	  std::shared_ptr<Shader> Get(const std::string& name);

	  bool Exists(const std::string& name) const;
  private:
	  std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
  };

}