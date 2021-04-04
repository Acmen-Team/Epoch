#include "eppch.h"

#include <Epoch.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ExampleLayer :public Epoch::Layer
{
public:
  ExampleLayer() : Layer("Example"), m_Camera(45.0f, 1.6f, 0.9f, 0.1f, 100.0f)
  {
	//矩形顶点数据
	float CubeVertices[] = {
	  //背面
		/*    position    */  /* Coord */  /*       Color      */	/*     Normal    */
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.8f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f,  0.0f, -1.0f,
	  //正面
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	  //左侧面
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	  //右侧面
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.8f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.8f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	  //底面
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.8f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f, -1.0f,  0.0f,
	  //顶面
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
	//索引绘制
	unsigned int CubeIndices[] = { // 注意索引从0开始! 
	  //正面
		6, 7, 8, // 正左下三角形
		9, 10, 11, // 正右上三角形
	  //左侧面
		12, 13, 14,
		15, 16, 17,
	  //背面
		0, 1, 2,
		3, 4, 5,
	  //右侧面
		18, 19, 20,
		21, 22, 23,
	  //顶部
		30, 31, 32,
		33, 34, 35,
	  //底部
		24, 25, 26,
		27, 28, 29
	};

	//矩形顶点数据
	float PlaneVertices[] = {
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};
	//索引绘制
	uint32_t PlaneIndices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	//Cube
	m_CubeVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_CubeVertexBuffer;

	m_CubeVertexBuffer.reset(Epoch::VertexBuffer::Create(CubeVertices, sizeof(CubeVertices)));

	Epoch::BufferLayout CubeLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" },
	  { Epoch::ShaderDataType::Float4, "a_Color" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" }
	};

	m_CubeVertexBuffer->SetLayout(CubeLayout);
	m_CubeVertexArray->AddVertexBuffer(m_CubeVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_CubeIndexBuffer;
	m_CubeIndexBuffer.reset(Epoch::IndexBuffer::Create(CubeIndices, sizeof(CubeIndices) / sizeof(uint32_t)));
	m_CubeVertexArray->SetIndexBuffer(m_CubeIndexBuffer);

	//Plane
	m_PlaneVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_PlaneVertexBuffer;

	m_PlaneVertexBuffer.reset(Epoch::VertexBuffer::Create(PlaneVertices, sizeof(PlaneVertices)));

	Epoch::BufferLayout PlaneLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" }
	};

	m_PlaneVertexBuffer->SetLayout(PlaneLayout);
	m_PlaneVertexArray->AddVertexBuffer(m_PlaneVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_PlaneIndexBuffer;
	m_PlaneIndexBuffer.reset(Epoch::IndexBuffer::Create(PlaneIndices, sizeof(PlaneIndices) / sizeof(uint32_t)));
	m_PlaneVertexArray->SetIndexBuffer(m_PlaneIndexBuffer);

	// Load shader
	m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	m_ShaderLibrary.Load("ColorShader", "assets/shaders/VertexColor.glsl");
	m_ShaderLibrary.Load("PlanShader", "assets/shaders/Plane.glsl");
	m_ShaderLibrary.Load("LightShader", "assets/shaders/test/Light.glsl");
	// Load texture
	m_Texture = Epoch::Texture2D::Create("assets/textures/container.jpg");
	m_CheckerboardTex = Epoch::Texture2D::Create("assets/textures/Checkerboard.png");
	m_FaceTexture = Epoch::Texture2D::Create("assets/textures/face.png");
  }

  void OnUpdate(Epoch::Timestep timestep) override
  {
	if (Epoch::Input::IsKeyPressed(EP_KEY_LEFT))
	  m_CameraPosition.x -= m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_RIGHT))
	  m_CameraPosition.x += m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_DOWN))
	  m_CameraPosition.y -= m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_UP))
	  m_CameraPosition.y += m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_W))
	  m_CameraPosition.z -= m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_S))
	  m_CameraPosition.z += m_CameraSpeed * timestep.GetSeconds();

	if (Epoch::Input::IsKeyPressed(EP_KEY_A))
	  m_SquarePosition.x -= m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_D))
	  m_SquarePosition.x += m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_Q))
	  m_SquarePosition.y -= m_CameraSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_E))
	  m_SquarePosition.y += m_CameraSpeed * timestep.GetSeconds();

	Epoch::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Epoch::RenderCommand::Clear();

	m_Camera.SetPosition(m_CameraPosition);
	m_Camera.SetRotation(m_CameraRotation);

	// Bind Uniform
	auto TexShader = m_ShaderLibrary.Get("Texture");
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->use();
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformInt("u_Texture1", 0);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformInt("u_Texture2", 1);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(TexShader)->UploadUniformFloat3("u_viewPos", m_CameraPosition);

	auto PlanShader = m_ShaderLibrary.Get("PlanShader");
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->use();
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->UploadUniformInt("u_Texture1", 2);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(PlanShader)->UploadUniformFloat3("u_viewPos", m_CameraPosition);

	auto ColorShader = m_ShaderLibrary.Get("ColorShader");
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(ColorShader)->use();
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(ColorShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(ColorShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(ColorShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(ColorShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(ColorShader)->UploadUniformFloat3("u_viewPos", m_CameraPosition);

	auto LightShader = m_ShaderLibrary.Get("LightShader");
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(LightShader)->use();
	std::dynamic_pointer_cast<Epoch::OpenGLShader>(LightShader)->UploadUniformFloat4("u_Color", m_LightColor);
	glm::mat4 LightTransform = glm::translate(glm::mat4(1.0f), m_LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));

	// Begin Rendering
	{
	  Epoch::Renderer::BeginScene(m_Camera);
	  for (int i = 0; i < 15; i++)
	  {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), cubePositions[i]);
		float angle = 20.0f * i;
		transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		if (i % 2 == 0)
		{
		  m_Texture->Bind();
		  m_FaceTexture->Bind(1);
		  Epoch::Renderer::Submit(TexShader, m_CubeVertexArray, transform);
		}
		else
		  Epoch::Renderer::Submit(ColorShader, m_CubeVertexArray, transform);
	  }

	  m_CheckerboardTex->Bind(2);
	  Epoch::Renderer::Submit(PlanShader, m_PlaneVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 1.0f, 4.0f)));

	  Epoch::Renderer::Submit(LightShader, m_CubeVertexArray, LightTransform);

	  Epoch::Renderer::EndScene();
	}
  }

  void OnImGuiRender() override
  {
	//Renderer Info
	ImGui::Begin("Renderer Info");
	ImGui::Text("  Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text(" Version: %s", glGetString(GL_VERSION));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	//setting
	ImGui::Begin("Setting");
	//ambientStrength
	ImGui::DragFloat("AmbientStrength", &ambientStrength, 0.03f, 0.0f, 1.0f);
	//SpecularStrength
	ImGui::DragFloat("SpecularStrength", &specularStrength, 0.03f, 0.0f, 1.0f);
	//Camera Rotation
	ImGui::DragFloat3("Camera Rotation", glm::value_ptr(m_CameraRotation), 0.03f);
	//Light Position
	ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPosition), 0.03f);
	//Light Color
	ImGui::ColorEdit4("Light Color", glm::value_ptr(m_LightColor));
	ImGui::End();
  }

  void OnEvent(Epoch::Event& event) override
  {

  }
private:
  std::shared_ptr<Epoch::VertexArray> m_CubeVertexArray;
  std::shared_ptr<Epoch::VertexArray> m_PlaneVertexArray;

  Epoch::ShaderLibrary m_ShaderLibrary;

  std::shared_ptr<Epoch::Texture2D> m_Texture;
  std::shared_ptr<Epoch::Texture2D> m_CheckerboardTex;
  std::shared_ptr<Epoch::Texture2D> m_FaceTexture;

  Epoch::PrespectiveCamera m_Camera;

  glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 3.0f };

  glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };

  float m_LastFramTime = 0.0f;
  //Camera
  float m_CameraSpeed = 0.8f;
  glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };
  float m_RotationSpeed = 5.0f;

  //Light
  glm::vec4 m_LightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
  glm::vec3 m_LightPosition = { 0.0f, 0.0f, 0.0f };
  //ambientStrength
  float ambientStrength = 0.1f;
  float specularStrength = 0.1f;

  glm::vec3 cubePositions[15] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(4.0f,  5.0f, -15.0f),
	glm::vec3(-2.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(3.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(2.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-2.3f,  1.0f, -2.5f),
	glm::vec3(-1.3f,  0.7f, -3.5f),
	glm::vec3(1.9f,  -0.5f, -1.5f),
	glm::vec3(-0.3f,  1.1f, -2.5f),
	glm::vec3(0.9f,  -4.0f, -11.5f),
	glm::vec3(2.3f,  1.0f, -7.3f)
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