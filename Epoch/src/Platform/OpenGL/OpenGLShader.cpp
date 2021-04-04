#include "eppch.h"

#include "OpenGLShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace Epoch {

  static GLenum ShaderTypeFromString(const std::string& type)
  {
	if (type == "VERTEX") return GL_VERTEX_SHADER;
	if (type == "FRAGMENT" || type == "PIXEL") return GL_FRAGMENT_SHADER;

	EP_CORE_ASSERT(false, "Unknow shader type!");
	return 0;
  }

  OpenGLShader::OpenGLShader(const std::string& filePath)
  {
	std::string source = ReadFile(filePath);
	auto shaderSource = PreProcess(source);
	Compile(shaderSource);

	// can be greate?
	auto lastSlash = filePath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filePath.rfind('.');
	auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filePath.substr(lastSlash, count);
  }

  OpenGLShader::OpenGLShader(const char* vertexPath, const char* fragmentPath)
  {
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
	  vShaderFile.open(vertexPath);
	  fShaderFile.open(fragmentPath);
	  std::stringstream vShaderStream, fShaderStream;

	  vShaderStream << vShaderFile.rdbuf();
	  fShaderStream << fShaderFile.rdbuf();

	  vShaderFile.close();
	  fShaderFile.close();

	  vertexCode = vShaderStream.str();
	  fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
	  EP_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	uint32_t vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
	  glDeleteShader(vertexShader);
	  glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	  EP_CORE_ERROR("ERROR::SHADER::VERTEXSHADER::COMPILATION_FAILED\n");
	  EP_CORE_ERROR("{0}", infoLog);
	  return;
	};

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
	  glDeleteShader(fragmentShader);
	  glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	  EP_CORE_ERROR("ERROR::SHADER::FRAGMENTSHADER::COMPILATION_FAILED\n");
	  EP_CORE_ERROR("{0}", infoLog);
	  return;
	};

	m_RendererID = glCreateProgram();

	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);

	glLinkProgram(m_RendererID);

	glDetachShader(m_RendererID, vertexShader);
	glDetachShader(m_RendererID, fragmentShader);
  }

  OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	: m_Name(name)
  {
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSource;
	sources[GL_FRAGMENT_SHADER] = fragmentSource;
	Compile(sources);
  }

  void OpenGLShader::use() const
  {
	glUseProgram(m_RendererID);
  }

  void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(location, value);
  }

  void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(location, value);
  }

  void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(location, value.x, value.y);
  }

  void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
  }

  void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
  }

  void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
  {
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  std::string OpenGLShader::ReadFile(const std::string& filePath)
  {
	std::string result;
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if (in)
	{
	  in.seekg(0, std::ios::end);
	  result.resize(in.tellg());
	  in.seekg(0, std::ios::beg);
	  in.read(&result[0], result.size());
	  in.close();
	}
	else
	{
	  EP_CORE_ERROR("Could not open file '{0}'", filePath);
	}

	return result;
  }

  std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
  {
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
	  size_t eol = source.find_first_of("\r\n", pos);
	  EP_CORE_ASSERT(eol != std::string::npos, "Synatx Error");
	  size_t begin = pos + typeTokenLength + 1;
	  std::string type = source.substr(begin, eol - begin);
	  EP_CORE_ASSERT(type == "VERTEX" || type == "FRAGMENT" || type == "PIXEL", "Invalid shader type specific");

	  size_t nextLinePos = source.find_first_not_of("\r\n", eol);
	  pos = source.find(typeToken, nextLinePos);
	  shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

	}
	return shaderSources;
  }

  void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
  {
	GLint program = glCreateProgram();
	std::vector<GLenum> glShaderIDs;
	glShaderIDs.reserve(shaderSources.size());

	for (auto& kv : shaderSources)
	{
	  GLenum type = kv.first;
	  const std::string& source = kv.second;

	  // Create an empty shader handle
	  GLint shader = glCreateShader(type);

	  // Send the vertex shader source code to GL
	  const char *sourceStr = (const char *)source.c_str();
	  glShaderSource(shader, 1, &sourceStr, 0);

	  // Compile the vertex shader
	  glCompileShader(shader);

	  GLint isCompiled = 0;
	  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	  if (isCompiled == GL_FALSE)
	  {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(shader);

		// Use the infoLog as you see fit.
		EP_CORE_ERROR("ERROR::SHADER::FRAGMENTSHADER::COMPILATION_FAILED\n");
		EP_CORE_ERROR("ERROR::SHADER::SHADER::COMPILATION_FAILED\n");
		EP_CORE_ERROR("{0}", infoLog.data());

		return;
	  }
	  // Attach our shaders to our program
	  glAttachShader(program, shader);
	  glShaderIDs.push_back(shader);
	}
	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_RendererID = program;

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
	  GLint maxLength = 0;
	  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	  // The maxLength includes the NULL character
	  std::vector<GLchar> infoLog(maxLength);
	  glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
	  // We don't need the program anymore.
	  glDeleteProgram(program);
	  // Don't leak shaders either.
	  for (auto id : glShaderIDs)
		glDeleteShader(id);

	  // Use the infoLog as you see fit.
	  EP_CORE_ERROR("ERROR::SHADER::LINK_FAILED\n");
	  EP_CORE_ERROR("{0}", infoLog.data());

	  return;
	}
	// Always detach shaders after a successful link.
	for (auto id : glShaderIDs)
	  glDetachShader(program, id);
	m_RendererID = program;
  }

  OpenGLShader::~OpenGLShader()
  {
	glDeleteProgram(m_RendererID);
  }

}
