#include "eppch.h"

#include "EditorLayer.h"

#include "Resource/Mesh.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Epoch {

  EditorLayer::EditorLayer() : Layer("Example"), m_CameraController(1.6f / 0.9f)
  {

  }

  void EditorLayer::OnAttach()
  {
	MeshData* earthData = Mesh::CreateMesh("assets/models/earth.obj", "assets/models/", true);
	MeshData* cubeData = Mesh::CreateMesh("assets/models/cube.obj", "assets/models/", true);
	MeshData* bulbData = Mesh::CreateMesh("assets/models/bulb.obj", "assets/models/", true);

	//矩形顶点数据
	float PlaneVertices[] = {
		 0.5f, -0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};
	//索引绘制
	uint32_t PlaneIndices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};

	//Cube
	m_CubeVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_CubeVertexBuffer;

	m_CubeVertexBuffer.reset(Epoch::VertexBuffer::Create((float*)&cubeData->vertices_list[0], sizeof(float) * 8 * cubeData->vertices_list.size()));

	Epoch::BufferLayout CubeLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_CubeVertexBuffer->SetLayout(CubeLayout);
	m_CubeVertexArray->AddVertexBuffer(m_CubeVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_CubeIndexBuffer;
	m_CubeIndexBuffer.reset(Epoch::IndexBuffer::Create((uint32_t*)&cubeData->indices_list[0], cubeData->indices_list.size()));
	m_CubeVertexArray->SetIndexBuffer(m_CubeIndexBuffer);

	//Plane
	m_PlaneVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_PlaneVertexBuffer;

	m_PlaneVertexBuffer.reset(Epoch::VertexBuffer::Create(PlaneVertices, sizeof(PlaneVertices)));

	Epoch::BufferLayout PlaneLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_PlaneVertexBuffer->SetLayout(PlaneLayout);
	m_PlaneVertexArray->AddVertexBuffer(m_PlaneVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_PlaneIndexBuffer;
	m_PlaneIndexBuffer.reset(Epoch::IndexBuffer::Create(PlaneIndices, sizeof(PlaneIndices) / sizeof(uint32_t)));
	m_PlaneVertexArray->SetIndexBuffer(m_PlaneIndexBuffer);

	// Bulb
	m_BulbVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_BulbVertexBuffer;

	m_BulbVertexBuffer.reset(Epoch::VertexBuffer::Create((float*)&bulbData->vertices_list[0], sizeof(float) * 8 * bulbData->vertices_list.size()));

	Epoch::BufferLayout BulbLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_BulbVertexBuffer->SetLayout(BulbLayout);
	m_BulbVertexArray->AddVertexBuffer(m_BulbVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_BulbIndexBuffer;
	m_BulbIndexBuffer.reset(Epoch::IndexBuffer::Create((uint32_t*)&bulbData->indices_list[0], bulbData->indices_list.size()));
	m_BulbVertexArray->SetIndexBuffer(m_BulbIndexBuffer);

	// Earth
	m_EarthVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_EarthVertexBuffer;

	m_EarthVertexBuffer.reset(Epoch::VertexBuffer::Create((float*)&earthData->vertices_list[0], sizeof(float) * 8 * earthData->vertices_list.size()));

	Epoch::BufferLayout EarthLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_EarthVertexBuffer->SetLayout(EarthLayout);
	m_EarthVertexArray->AddVertexBuffer(m_EarthVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_EarthIndexBuffer;
	m_EarthIndexBuffer.reset(Epoch::IndexBuffer::Create((uint32_t*)&earthData->indices_list[0], earthData->indices_list.size()));
	m_EarthVertexArray->SetIndexBuffer(m_EarthIndexBuffer);

	// Load shader
	m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	m_ShaderLibrary.Load("PlanShader", "assets/shaders/Plane.glsl");
	m_ShaderLibrary.Load("LightShader", "assets/shaders/Demo/Light.glsl");
	m_ShaderLibrary.Load("Test", "assets/shaders/Test.glsl");

	// Load texture
	m_Texture = Epoch::Texture2D::Create("assets/textures/earth.jpg");
	m_CheckerboardTex = Epoch::Texture2D::Create("assets/textures/Checkerboard.png");
	m_FaceTexture = Epoch::Texture2D::Create("assets/textures/Stare.jpg");
	m_StareTexture = Epoch::Texture2D::Create("assets/textures/stars_map.jpg");

	Epoch::FramebufferSpecification fbSpec;
	fbSpec.Width = 1366;
	fbSpec.Height = 768;
	m_Framebuffer = Epoch::Framebuffer::Create(fbSpec);
  }

  void EditorLayer::OnDetach()
  {

  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
	m_CameraController.OnUpdate(timestep);

	m_Framebuffer->Bind();

	Epoch::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	Epoch::RenderCommand::Clear();

	// Bind Uniform
	auto TexShader = m_ShaderLibrary.Get("Texture");
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformInt("u_Texture1", 0);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::Shader>(TexShader)->UploadUniformFloat3("u_viewPos", m_CameraController.GetCamera().GetPosition());

	auto PlanShader = m_ShaderLibrary.Get("PlanShader");
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformInt("u_Texture1", 2);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat("u_AmbientStrength", ambientStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat("u_SpecularStrength", specularStrength);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat4("u_LightColor", m_LightColor);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat3("u_lightPos", m_LightPosition);
	std::dynamic_pointer_cast<Epoch::Shader>(PlanShader)->UploadUniformFloat3("u_viewPos", m_CameraController.GetCamera().GetPosition());

	auto LightShader = m_ShaderLibrary.Get("LightShader");
	std::dynamic_pointer_cast<Epoch::Shader>(LightShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(LightShader)->UploadUniformFloat4("u_Color", m_LightColor);
	glm::mat4 LightTransform = glm::translate(glm::mat4(1.0f), m_LightPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f));

	auto TestShader = m_ShaderLibrary.Get("Test");
	std::dynamic_pointer_cast<Epoch::Shader>(TestShader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(TestShader)->UploadUniformInt("u_Texture1", 3);

	// Begin Rendering
	{
	  Epoch::Renderer::BeginScene(m_CameraController.GetCamera());

	  //m_StareTexture->Bind();
	  //Epoch::Renderer::Submit(TexShader, m_CubeVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
	  m_Texture->Bind();
	  Epoch::Renderer::Submit(TexShader, m_EarthVertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f)));


	  m_StareTexture->Bind(3);
	  Epoch::Renderer::Submit(TestShader, m_EarthVertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f)));

	  m_CheckerboardTex->Bind(2);
	  Epoch::Renderer::Submit(PlanShader, m_PlaneVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 1.0f, 5.0f)));

	  Epoch::Renderer::Submit(LightShader, m_BulbVertexArray, LightTransform);

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
	  //Light Setting
	  ImGui::DragFloat("ambientStrength", &ambientStrength, 0.003f, 0.0f, 1.0f);
	  ImGui::DragFloat("specularStrength", &specularStrength, 0.003f, 0.0f, 1.0f);
	  ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPosition), 0.03f);
	  ImGui::ColorEdit4("Light Color", glm::value_ptr(m_LightColor), 0.03f);
	  ImGui::DragFloat3("Dragon Rotation", glm::value_ptr(m_Position), 0.03f);
	  ImGui::End();
	}

	ImGui::End();
  }

}