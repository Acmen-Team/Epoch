#include "eppch.h"
#include "Application.h"
#include "Log.h"

#include "Core/Timestep.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Input.h"
#include "KeyCodes.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Epoch {

  #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

  Application* Application::m_Instance = nullptr;

  Application::Application()
	: m_Camera(45.0f, 1.6f, 0.9f, 0.1f, 100.0f)
  {
	EP_CORE_ASSERT(!m_Instance, "Application already exists!");
	Application::m_Instance = this;

	m_Window = std::unique_ptr<Window>(Window::Create());
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	m_ImGuiLayer = new ImGuiLayer();
	PushOverlay(m_ImGuiLayer);

	//矩形顶点数据
	float vertices[] = {
		0.5f, 0.5f, 0.0f, 0.8f, 0.2f, 0.3f, 1.0f,  // 右上角
		0.5f, -0.5f, 0.0f, 0.3f, 0.4f, 0.7f, 1.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, 0.5f, 0.8f, 0.2f, 1.0f, // 左下角
		-0.5f, 0.5f, 0.0f, 0.2f, 0.5f, 0.3f, 1.0f   // 左上角
	};
	//索引绘制
	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	m_VertexArray.reset(VertexArray::Create());

	std::shared_ptr<VertexBuffer> m_VertexBuffer;

	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

	BufferLayout layout = {
	  { ShaderDataType::Float3, "a_Pos" },
	  { ShaderDataType::Float4, "a_Color" }
	};

	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	std::string vertexShaderSource = R"(
	  #version 330 core
	  layout(location = 0) in vec3 a_Pos;
	  layout(location = 1) in vec4 a_Color;

	  out vec4 v_Color;
	  uniform mat4 u_ViewProjection;
	  uniform mat4 u_Transform;

	  void main() 
	  {
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
		v_Color = a_Color;
	  }
	)";

	std::string fragmentShaderSource = R"(
	  #version 330 core

	  in  vec4 v_Color;
	  out vec4 FragColor;

	  void main() 
	  {
		FragColor = v_Color;
	  }
	)";

	m_Shader.reset(new Shader(vertexShaderSource, fragmentShaderSource));
  }

  Application::~Application()
  {

  }

  void Application::StartUp()
  {
	WindowResizeEvent e(1280, 720);
	if (e.IsInCategory(EventCategoryApplication))
	{
	  while (m_Running)
	  {
		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFramTime;
		m_LastFramTime = time;

		if (Input::IsKeyPressed(EP_KEY_LEFT))
		  m_CameraPosition.x -= m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_RIGHT))
		  m_CameraPosition.x += m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_DOWN))
		  m_CameraPosition.y -= m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_UP))
		  m_CameraPosition.y += m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_W))
		  m_CameraPosition.z -= m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_S))
		  m_CameraPosition.z += m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_Q))
		  m_Rotation -= m_RotationSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_E))
		  m_Rotation += m_RotationSpeed * timestep.GetSeconds();

		if (Input::IsKeyPressed(EP_KEY_A))
		  m_SquarePosition.x -= m_CameaSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_D))
		  m_SquarePosition.x += m_CameaSpeed * timestep.GetSeconds();

		RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_Rotation);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition);

		// Begin Rendering
		{
		  Renderer::BeginScene(m_Camera);
		  Renderer::Submit(m_Shader, m_VertexArray, transform);
		  Renderer::Submit(m_Shader, m_VertexArray);
		  Renderer::EndScene();
		}

		for (Layer* layer : m_LayerStack)
		  layer->OnUpdate(timestep);

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
		  layer->OnImGuiRender();
		m_ImGuiLayer->End();


		m_Window->OnUpdate();
	  }
	}
  }

  void Application::OnEvent(Event & e)
  {
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

	//DK_CORE_TRACE("{0}", e);

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
	{
	  (*--it)->OnEvent(e);
	  if (e.Handled)
		break;
	}
  }

  void Application::PushLayer(Layer* layer)
  {
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer)
  {
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
  }

  bool Application::OnWindowClose(WindowCloseEvent & e)
  {
	m_Running = false;
	return true;
  }

  Application * CreateApplication()
  {
	return new Application();
  }

}