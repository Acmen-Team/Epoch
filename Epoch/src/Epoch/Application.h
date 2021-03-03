#pragma once

#include "Core.h"

#include "Window.h"
#include "Epoch/LayerStack.h"
#include "Epoch/Events/Event.h"
#include "Epoch/Events/ApplicationEvent.h"

#include "Epoch/ImGui/ImGuiLayer.h"

#include "Epoch/Renderer/Shader.h"
#include "Epoch/Renderer/Buffer.h"

namespace Epoch {

  class Application
  {
  public:
	Application();
	virtual ~Application();

	void StartUp();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *m_Instance; }
  private:
	bool OnWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
  private:
	static Application* m_Instance;
  };
  
  Application* CreateApplication();
}

