#pragma once

#include "Core.h"

#include "Window.h"
#include "Epoch/LayerStack.h"
#include "Epoch/Events/Event.h"
#include "Epoch/Events/ApplicationEvent.h"

#include "Epoch/ImGui/ImGuiLayer.h"

#include "Epoch/Renderer/Shader.h"
#include "Epoch/Renderer/Buffer.h"
#include "Epoch/Renderer/VertexArray.h"
#include "Epoch/Renderer/PrespectiveCamera.h"

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
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<Shader> m_Shader;

	PrespectiveCamera m_Camera;

	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 3.0f };

	glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };

	float m_CameaSpeed = 0.8f;
	float m_LastFramTime = 0.0f;
	float m_Rotation = 0.0f;
	float m_RotationSpeed = 5.0f;


	glm::vec3 cubePositions[10] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(4.0f,  5.0f, -15.0f),
	  glm::vec3(-2.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(3.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(2.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-2.3f,  1.0f, -1.5f)
	};
  private:
	static Application* m_Instance;
  };
  
  Application* CreateApplication();
}

