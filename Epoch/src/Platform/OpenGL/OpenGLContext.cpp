#include "eppch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Epoch {

  OpenGLContext::OpenGLContext(GLFWwindow* windowHanle) : m_WindowHandle(windowHanle)
  {
	EP_CORE_ASSERT(windowHanle, "windowHandle is NUll!")
  }

  void OpenGLContext::Init()
  {
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	EP_CORE_ASSERT(status, "Failed to initialize Glad!");

	EP_CORE_INFO("------OpenGL Info------");
	EP_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
	EP_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
	EP_CORE_INFO("Version: {0}", glGetString(GL_VERSION));
  }

  void OpenGLContext::SwapBuffers()
  {
	glfwSwapBuffers(m_WindowHandle);
  }

}