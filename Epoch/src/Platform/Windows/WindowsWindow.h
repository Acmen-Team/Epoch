#pragma once

#include "Epoch/Window.h"
#include "Epoch/Renderer/RendererContext.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Epoch {

  class WindowsWindow :public Window
  {
  public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;

	virtual void* GetNativeWindow() const override { return m_Window; }
	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }

	// Window attribute
	inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;
  private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();
  private:
	GLFWwindow* m_Window;
	RenderContext *m_Context;

	struct WindowData
	{
	  std::string Title;
	  unsigned int Width, Height;
	  bool VSync;

	  EventCallbackFn EventCallback;
	};

	WindowData m_Data;
  };

}
