#include "eppch.h"

#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Epoch {

  Shader::Shader(const char* vertexPath, const char* fragmentPath)
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

  Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
  {
	// Create an empty vertex shader handle
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const char *source = (const char *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
	  GLint maxLength = 0;
	  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

	  // The maxLength includes the NULL character
	  std::vector<GLchar> infoLog(maxLength);
	  glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

	  // We don't need the shader anymore.
	  glDeleteShader(vertexShader);

	  // Use the infoLog as you see fit.
	  EP_CORE_ERROR("ERROR::SHADER::VERTEXSHADER::COMPILATION_FAILED\n");
	  EP_CORE_ERROR("{0}", infoLog.data());

	  return;
	}

	// Create an empty fragment shader handle
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = (const char *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
	  GLint maxLength = 0;
	  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

	  // The maxLength includes the NULL character
	  std::vector<GLchar> infoLog(maxLength);
	  glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

	  // We don't need the shader anymore.
	  glDeleteShader(fragmentShader);
	  // Either of them. Don't leak shaders.
	  glDeleteShader(vertexShader);

	  // Use the infoLog as you see fit.
	  EP_CORE_ERROR("ERROR::SHADER::FRAGMENTSHADER::COMPILATION_FAILED\n");
	  EP_CORE_ERROR("{0}", infoLog.data());

	  return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_RendererID = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(m_RendererID, vertexShader);
	glAttachShader(m_RendererID, fragmentShader);

	// Link our program
	glLinkProgram(m_RendererID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
	  GLint maxLength = 0;
	  glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

	  // The maxLength includes the NULL character
	  std::vector<GLchar> infoLog(maxLength);
	  glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

	  // We don't need the program anymore.
	  glDeleteProgram(m_RendererID);
	  // Don't leak shaders either.
	  glDeleteShader(vertexShader);
	  glDeleteShader(fragmentShader);

	  // Use the infoLog as you see fit.
	  EP_CORE_ERROR("ERROR::SHADER::LINK_FAILED\n");
	  EP_CORE_ERROR("{0}", infoLog.data());

	  return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_RendererID, vertexShader);
	glDetachShader(m_RendererID, fragmentShader);
  }

  void Shader::use()
  {
	glUseProgram(m_RendererID);
  }

  Shader::~Shader()
  {
	glDeleteProgram(m_RendererID);
  }

}