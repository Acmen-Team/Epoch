#pragma once

#include "Core.h"

#include "Window.h"
#include "Epoch/Core/LayerStack.h"
#include "Epoch/Events/Event.h"
#include "Epoch/Events/ApplicationEvent.h"

#include "Epoch/ImGui/ImGuiLayer.h"

namespace Epoch {

  class Application
  {
  public:
	Application(const std::string& name = "Epoch");
	virtual ~Application();

	void StartUp();
	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);

	void Exit();

	inline Window& GetWindow() { return *m_Window; }
	inline static Application& Get() { return *m_Instance; }
  private:
	bool OnWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	LayerStack m_LayerStack;

	float m_LastFramTime = 0.0f;
  private:
	static Application* m_Instance;
  };
  
  Application* CreateApplication();
}

