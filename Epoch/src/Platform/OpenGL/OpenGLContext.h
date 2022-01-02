#pragma once

#include "Epoch/Graphics/Renderer/RendererContext.h"

struct GLFWwindow;

namespace Epoch {

  class OpenGLContext : public RenderContext
  {
  public:
	OpenGLContext(GLFWwindow* windowHanle);

	virtual void Init() override;
	virtual void SwapBuffers() override;
  private:
	GLFWwindow* m_WindowHandle;
  };

}