#include "eppch.h"

#include <Epoch.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ExampleLayer :public Epoch::Layer
{
public:
  ExampleLayer() : Layer("Example"), m_Camera(45.0f, 1.6f, 0.9f, 0.1f, 100.0f)
  {
	//矩形顶点数据
	float vertices[] = {
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,   // 正右上角
		0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f,  // 正右下角
		-0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // 正左下角
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,  // 正左上角

		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,   // 背右上角
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,  // 背右下角
		-0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, // 背左下角
		-0.5f, 0.5f, -0.5f, 0.2f, 0.0f, 0.0f, 0.0f   // 背左上角
	};
	//索引绘制
	unsigned int indices[] = { // 注意索引从0开始! 
	  //正面
		2, 3, 1, // 正左下三角形
		0, 1, 3, // 正右上三角形
	  //左侧面
		6, 7, 3,
		3, 2, 6,
	  //背面
		5, 4, 6,
		7, 6, 4,
	  //右侧面
		1, 0, 5,
		4, 5, 0,
	  //顶部
		3, 7, 0,
		4, 0, 7,
	  //底部
		6, 2, 5,
		1, 5, 2
	};

	m_VertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_VertexBuffer;

	m_VertexBuffer.reset(Epoch::VertexBuffer::Create(vertices, sizeof(vertices)));

	Epoch::BufferLayout layout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float4, "a_Color" }
	};

	m_VertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Epoch::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

	m_Shader.reset(Epoch::Shader::Create(vertexShaderSource, fragmentShaderSource));
  }

  void OnUpdate(Epoch::Timestep timestep) override
  {
	if (Epoch::Input::IsKeyPressed(EP_KEY_LEFT))
	  m_CameraPosition.x -= m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_RIGHT))
	  m_CameraPosition.x += m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_DOWN))
	  m_CameraPosition.y -= m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_UP))
	  m_CameraPosition.y += m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_W))
	  m_CameraPosition.z -= m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_S))
	  m_CameraPosition.z += m_CameaSpeed * timestep.GetSeconds();

	if (Epoch::Input::IsKeyPressed(EP_KEY_A))
	  m_SquarePosition.x -= m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_D))
	  m_SquarePosition.x += m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_Q))
	  m_SquarePosition.y -= m_CameaSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_E))
	  m_SquarePosition.y += m_CameaSpeed * timestep.GetSeconds();

	Epoch::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.2f, 1.0f });
	Epoch::RenderCommand::Clear();

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(m_Rotation);

	// Begin Rendering
	{
	  Epoch::Renderer::BeginScene(m_Camera);
	  for (int i = 0; i < 10; i++)
	  {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
		float angle = 20.0f * i;
		transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		Epoch::Renderer::Submit(m_Shader, m_VertexArray, transform);
	  }
	  Epoch::Renderer::EndScene();
	}
  }

  void OnImGuiRender() override
  {
	ImGui::Begin("Renderer Info");
	ImGui::Text("  Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text(" Version: %s", glGetString(GL_VERSION));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
  }

  void OnEvent(Epoch::Event& event) override
  {

  }
private:
  std::shared_ptr<Epoch::VertexArray> m_VertexArray;
  std::shared_ptr<Epoch::Shader> m_Shader;

  Epoch::PrespectiveCamera m_Camera;

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

};

class SandBox :public Epoch::Application
{
public:
  SandBox()
  {
	PushLayer(new ExampleLayer());
  }
  ~SandBox()
  {

  }
};

Epoch::Application* Epoch::CreateApplication()
{
  return new SandBox();
}