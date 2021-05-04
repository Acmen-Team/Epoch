#include "eppch.h"

#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Epoch {

  EditorLayer::EditorLayer() : Layer("Example"), m_Camera(45.0f, 1.6f, 0.9f, 0.1f, 100.0f)
  {

  }

  void EditorLayer::OnAttach()
  {
	//���ζ�������
	float CubeVertices[] = {
	  //����
		/*    position    */  /* Coord */  /*       Color      */	/*     Normal    */
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.8f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f,  0.0f, -1.0f,
	  //����
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
	  //�����
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
	  //�Ҳ���
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.8f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.8f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
	  //����
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.8f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.5f, 1.0f, 0.0f, 0.7f,  0.0f, -1.0f,  0.0f,
	  //����
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.6f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.2f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
	//��������
	unsigned int CubeIndices[] = { // ע��������0��ʼ! 
	  //����
		6, 7, 8, // ������������
		9, 10, 11, // ������������
	  //�����
		12, 13, 14,
		15, 16, 17,
	  //����
		0, 1, 2,
		3, 4, 5,
	  //�Ҳ���
		18, 19, 20,
		21, 22, 23,
	  //����
		30, 31, 32,
		33, 34, 35,
	  //�ײ�
		24, 25, 26,
		27, 28, 29
	};

	//���ζ�������
	float PlaneVertices[] = {
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};
	//��������
	uint32_t PlaneIndices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
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
	m_ShaderLibrary.Load("LightShader", "assets/shaders/Demo/Light.glsl");
	// Load texture
	m_Texture = Epoch::Texture2D::Create("assets/textures/container.jpg");
	m_CheckerboardTex = Epoch::Texture2D::Create("assets/textures/Checkerboard.png");
	m_FaceTexture = Epoch::Texture2D::Create("assets/textures/face.png");

	Epoch::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Epoch::Framebuffer::Create(fbSpec);
  }

  void EditorLayer::OnDetach()
  {

  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
	m_CameraController.OnUpdate(timestep);
	//if (Epoch::Input::IsKeyPressed(EP_KEY_LEFT))
	//  m_CameraPosition.x -= m_CameraSpeed * timestep.GetSeconds();
	//if (Epoch::Input::IsKeyPressed(EP_KEY_RIGHT))
	//  m_CameraPosition.x += m_CameraSpeed * timestep.GetSeconds();
	//if (Epoch::Input::IsKeyPressed(EP_KEY_DOWN))
	//  m_CameraPosition.y -= m_CameraSpeed * timestep.GetSeconds();
	//if (Epoch::Input::IsKeyPressed(EP_KEY_UP))
	//  m_CameraPosition.y += m_CameraSpeed * timestep.GetSeconds();

	//if (Epoch::Input::IsKeyPressed(EP_KEY_A))
	//  m_SquarePosition.x -= m_CameraSpeed * timestep.GetSeconds();
	//if (Epoch::Input::IsKeyPressed(EP_KEY_D))
	//  m_SquarePosition.x += m_CameraSpeed * timestep.GetSeconds();
	//if (Epoch::Input::IsKeyPressed(EP_KEY_Q))
	//  m_SquarePosition.y -= m_CameraSpeed * timestep.GetSeconds();
	//if (Epoch::Input::IsKeyPressed(EP_KEY_E))
	//  m_SquarePosition.y += m_CameraSpeed * timestep.GetSeconds();

	m_Framebuffer->Bind();

	Epoch::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Epoch::RenderCommand::Clear();

	// Bind Uniform
	auto TexShader = m_ShaderLibrary.Get("Texture");
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformInt("u_Texture1", 0);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformInt("u_Texture2", 1);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat3("u_viewPos", m_CameraPosition);

	auto PlanShader = m_ShaderLibrary.Get("PlanShader");
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformInt("u_Texture1", 2);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat3("u_viewPos", m_CameraPosition);

	auto ColorShader = m_ShaderLibrary.Get("ColorShader");
	std::dynamic_pointer_cast<Epoch::Shader>(ColorShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(ColorShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(ColorShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(ColorShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::Shader>(ColorShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::Shader>(ColorShader)->UploadUniformFloat3("u_viewPos", m_CameraPosition);

	auto LightShader = m_ShaderLibrary.Get("LightShader");
	std::dynamic_pointer_cast<Epoch::Shader>(LightShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(LightShader)->UploadUniformFloat4("u_Color", m_LightColor);
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

	m_Framebuffer->UnBind();
  }

  void EditorLayer::OnEvent(Event& event)
  {
	m_CameraController.OnEvent(event);
  }

  void EditorLayer::OnImGuiRender()
  {
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	static bool *p_open;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
	  ImGuiViewport* viewport = ImGui::GetMainViewport();
	  ImGui::SetNextWindowPos(viewport->GetWorkPos());
	  ImGui::SetNextWindowSize(viewport->GetWorkSize());
	  ImGui::SetNextWindowViewport(viewport->ID);
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
	  dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
	  window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
	  ImGui::PopStyleVar();

	if (opt_fullscreen)
	  ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
	  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
	  if (ImGui::BeginMenu("File"))
	  {
		// Disabling fullscreen would allow the window to be moved to the front of other windows,
		// which we can't undo at the moment without finer window depth/z control.
		if (ImGui::MenuItem("Exit"))
		  Epoch::Application::Get().Exit();

		ImGui::EndMenu();
	  }

	  ImGui::EndMenuBar();
	}

	{
	  // Scene
	  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0, 0.0 });
	  ImGui::Begin("Scene");
	  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	  if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
	  {
		m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	  }
	  ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
	  ImGui::End();
	  ImGui::PopStyleVar();
	}

	{
	  // Detail
	  ImGui::Begin("Detail");
	  //ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	  ImGui::End();
	}

	{
	  // Texture
	  ImGui::Begin("Texture");
	  ImGui::Text("Default Texture");
	  //ImGui::ImageButton((void*)m_DfaultTex->GetRendererID(), ImVec2{ 64.0f, 64.0f });
	  ImGui::End();
	}

	{
	  //Setting
	  ImGui::Begin("Setting");
	  //Camera Rotation
	  //ImGui::DragFloat3("Camera Rotation", glm::value_ptr(m_CameraRotation), 0.03f);
	  ImGui::End();
	}

	ImGui::End();
  }

}