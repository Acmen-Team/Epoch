#include "eppch.h"

#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <thread>

namespace Epoch {

  EditorLayer::EditorLayer() : Layer("Example"), m_CameraController(1.6f / 0.9f)
  {

  }

  void EditorLayer::OnAttach()
  {
	MeshData* earthData = Mesh::CreateMesh("assets/models/bunny.obj", "assets/models/", true);
	MeshData* cubeData = Mesh::CreateMesh("assets/models/cube.obj", "assets/models/", true);
	MeshData* bulbData = Mesh::CreateMesh("assets/models/bulb.obj", "assets/models/", true);
	MeshData* bunnyData = Mesh::CreateMesh("assets/models/dragon.obj", "assets/models/", true);

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

	// Bunny
	m_BunnyVertexArray.reset(Epoch::VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_BunnyVertexBuffer;

	m_BunnyVertexBuffer.reset(Epoch::VertexBuffer::Create((float*)&bunnyData->vertices_list[0], sizeof(float) * 8 * bunnyData->vertices_list.size()));

	Epoch::BufferLayout BunnyLayout = {
	  { Epoch::ShaderDataType::Float3, "a_Pos" },
	  { Epoch::ShaderDataType::Float3, "a_Normal" },
	  { Epoch::ShaderDataType::Float2, "a_TexCoord" }
	};

	m_BunnyVertexBuffer->SetLayout(BunnyLayout);
	m_BunnyVertexArray->AddVertexBuffer(m_BunnyVertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_BunnyIndexBuffer;
	m_BunnyIndexBuffer.reset(Epoch::IndexBuffer::Create((uint32_t*)&bunnyData->indices_list[0], bunnyData->indices_list.size()));
	m_BunnyVertexArray->SetIndexBuffer(m_BunnyIndexBuffer);

	// Load shader
	m_ShaderLibrary.Load("PhoneShading", "assets/shaders/Phone.glsl");
	m_ShaderLibrary.Load("ColorShading", "assets/shaders/Color.glsl");

	// Load texture
	m_DefaultTexture = Epoch::Texture2D::Create("assets/textures/defaultTexture.jpg");
	m_Texture = Epoch::Texture2D::Create("assets/textures/earth.jpg");
	m_CheckerboardTex = Epoch::Texture2D::Create("assets/textures/Checkerboard.png");
	m_FaceTexture = Epoch::Texture2D::Create("assets/textures/face.png");
	m_StareTexture = Epoch::Texture2D::Create("assets/textures/Stare.jpg");

	// Create Framebuffer
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
	PROFILE_SCOPE("EditorLayer::OnUpdate");

	{
	  PROFILE_SCOPE("Camera::OnUpdate");
	  m_CameraController.OnUpdate(timestep);
	}

	m_Framebuffer->Bind();

	{
	  PROFILE_SCOPE("Render::Pro");
	  Epoch::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	  Epoch::RenderCommand::Clear();
	  Epoch::RenderCommand::SetRenderModel(m_RenderModel);
	}

	// Bind Uniform
	auto PhoneShading = m_ShaderLibrary.Get("PhoneShading");
	auto ColorShading = m_ShaderLibrary.Get("ColorShading");

	{
	  PROFILE_SCOPE("Shader::BindUniform");
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->use();
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformInt("u_Texture1", 0);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("ObjectColor", m_ObjectColor);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("ViewPosition", m_CameraController.GetCameraPosition());
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("material.ambient", materialData->Ambient);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("material.diffuse", materialData->Diffuse);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("material.specular", materialData->Specular);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat("material.shininess", materialData->Shininess);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("light.ambient", lightData->Ambient);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("light.diffuse", lightData->Diffuse);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("light.specular", lightData->Specular);
	  std::dynamic_pointer_cast<Epoch::Shader>(PhoneShading)->UploadUniformFloat3("light.position", lightData->Position);

	  std::dynamic_pointer_cast<Epoch::Shader>(ColorShading)->use();
	  std::dynamic_pointer_cast<Epoch::Shader>(ColorShading)->UploadUniformFloat3("LightColor", lightData->Diffuse);
	}

	// Begin Rendering
	{
	  PROFILE_SCOPE("Rendering::Begin Scene");
	  Epoch::Renderer::BeginScene(m_CameraController.GetCamera());

	  m_DefaultTexture->Bind();
	  Epoch::Renderer::Submit(PhoneShading, m_BunnyVertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));

	  Epoch::Renderer::Submit(PhoneShading, m_EarthVertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f)));

	  m_StareTexture->Bind();
	  Epoch::Renderer::Submit(PhoneShading, m_CubeVertexArray, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)));

	  m_CheckerboardTex->Bind();
	  Epoch::Renderer::Submit(PhoneShading, m_PlaneVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(6.0f, 1.0f, 5.0f)));

	  //m_Texture->Bind();

	  glm::mat4 LightTransform = glm::translate(glm::mat4(1.0f), lightData->Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f));
	  Epoch::Renderer::Submit(ColorShading, m_BulbVertexArray, LightTransform);

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
	  //Setting
	  ImGui::Begin("Setting");
	  if (ImGui::Combo("RenderModel", &renderModel_idx, "Fill\0Line\0Point\0"))
	  {
		switch (renderModel_idx)
		{
		case 0: m_RenderModel = RenderModel::Fill; break;
		case 1: m_RenderModel = RenderModel::Line; break;
		case 2: m_RenderModel = RenderModel::Point; break;
		}
	  }
	  ImGui::End();
	}

	{
	  // Profile
	  ImGui::Begin("Profile");
	  for (auto& result : m_ProfileResults)
	  {
		char label[50];
		strcpy(label, result.Name);
		strcat(label, "  %.3fms");
		ImGui::Text(label, result.Time);
	  }
	  m_ProfileResults.clear();
	  ImGui::End();
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
	  //Light Setting
	  ImGui::ColorEdit3("Ambient", glm::value_ptr(lightData->Ambient), 0.03f);
	  ImGui::ColorEdit3("Diffuse", glm::value_ptr(lightData->Diffuse), 0.03f);
	  ImGui::ColorEdit3("Specular", glm::value_ptr(lightData->Specular), 0.03f);
	  ImGui::DragFloat3("Position", glm::value_ptr(lightData->Position), 0.1f);
	  ImGui::ColorEdit4("ObjectColor", glm::value_ptr(m_ObjectColor), 0.03f);

	  //ImGui::DragFloat3("Ambient", glm::value_ptr(lightData->Ambient), 0.1f);
	  //ImGui::DragFloat3("Diffuse", glm::value_ptr(lightData->Diffuse), 0.1f);
	  //ImGui::DragFloat3("Specular", glm::value_ptr(lightData->Specular), 0.1f);
	  //ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	  ImGui::End();
	}

	{
	  // Texture
	  ImGui::Begin("Texture");
	  ImGui::Text("Default Texture");
	  if (ImGui::ImageButton((void*)m_DefaultTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	  {
		m_DefaultTexture->Bind();
	  }
	  if (ImGui::ImageButton((void*)m_StareTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	  {
		m_StareTexture->Bind(0);
	  }
	  if (ImGui::ImageButton((void*)m_FaceTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	  {
		m_FaceTexture->Bind(0);
	  }
	  ImGui::End();
	}

	{
	  //material
	  ImGui::Begin("Material");
	  ImGui::DragFloat3("Ambient", glm::value_ptr(materialData->Ambient), 0.3f);
	  ImGui::DragFloat3("Diffuse", glm::value_ptr(materialData->Diffuse), 0.3f);
	  ImGui::DragFloat3("Specular", glm::value_ptr(materialData->Specular), 0.3f);
	  ImGui::DragFloat("Shininess", &(materialData->Shininess), 0.5f);
	  ImGui::End();
	}

	ImGui::End();
  }

}