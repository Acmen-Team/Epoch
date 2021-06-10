#include "eppch.h"

#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "FileDialog.h"

#include <thread>

namespace Epoch {

  EditorLayer::EditorLayer() : Layer("Example")
  {

  }

  void EditorLayer::OnAttach()
  {

	// Load shader
	m_ShaderLibrary.Load("Phone", "assets/shaders/Phone.glsl");
	m_ShaderLibrary.Load("TestShading", "assets/shaders/Phone.glsl");
	m_ShaderLibrary.Load("ColorShading", "assets/shaders/Color.glsl");
	m_ShaderLibrary.Load("PhoneShading", "assets/shaders/PhongShading.glsl");

	// Create Framebuffer
	Epoch::FramebufferSpecification fbSpec;
	fbSpec.Width = 1366;
	fbSpec.Height = 768;
	m_Framebuffer = Epoch::Framebuffer::Create(fbSpec);

	m_Scene = std::make_shared<Scene>();

	Entity cube = m_Scene->CreatEntity("Cube");
	if (cube.HasComponent<TagComponent>())
	{
	  std::cout << "Yes" << std::endl;
	}
	cube.AddComponent<MeshConponent>("assets/models/cube.obj", "assets/models/");
	cube.AddComponent<TransformComponent>(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f)));

	m_PerspectiveCameraEntity = m_Scene->CreatEntity("PreCamera Entity");
	m_PerspectiveCameraEntity.AddComponent<CameraComponent>(glm::perspective(glm::radians(45.0f), (1.6f / 0.9f), 0.1f, 100.0f));
	m_PerspectiveCameraEntity.AddComponent<TransformComponent>();

	class CameraController : public ScriptableEntity
	{
	public:
	  void OnCreate(){}

	  void OnUpdate(Timestep timestep)
	  {
		auto& transform = GetComponent<TransformComponent>().Transform;

		float m_CamearTransformationSpeed = 1.0;

		if (Input::IsKeyPressed(EP_KEY_W))
		  transform[3][2] += m_CamearTransformationSpeed * timestep.GetSeconds();
		if (Input::IsKeyPressed(EP_KEY_S))
		  transform[3][2] -= m_CamearTransformationSpeed * timestep.GetSeconds();
	  }

	  void OnDestroy(){}
	private:

	};

	m_PerspectiveCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

	m_SceneHierarchyPanel.SetContext(m_Scene);
  }

  void EditorLayer::OnDetach()
  {

  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
	PROFILE_SCOPE("EditorLayer::OnUpdate");

	m_Framebuffer->Bind();

	{
	  PROFILE_SCOPE("Render::Pro");
	  Epoch::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	  Epoch::RenderCommand::Clear();
	  Epoch::RenderCommand::SetRenderModel(m_RenderModel);
	}

	// Begin Rendering
	{
	  PROFILE_SCOPE("Rendering::Begin Scene");

	  m_Scene->OnUpdate(timestep);
	}

	m_Framebuffer->UnBind();
  }

  void EditorLayer::OnEvent(Event& event)
  {
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

	m_SceneHierarchyPanel.OnImGuiRender();

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
	  if (ImGui::Combo("LightType", &lightType, "Default\0Directon\0Point\0Spot\0"))
	  {
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
	  m_ViewPanelFocused = ImGui::IsWindowFocused();
	  m_ViewPanelHovered = ImGui::IsWindowHovered();
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
	  ImGui::DragFloat3("Direction", glm::value_ptr(lightData->Direction), 0.1f);
	  ImGui::DragFloat("Constant", &lightData->Constant, 0.03f);
	  ImGui::DragFloat("Linear", &lightData->Linear, 0.03f);
	  ImGui::DragFloat("Quadratic", &lightData->Quadratic, 0.03f);
	  ImGui::DragFloat("CutOff", &lightData->CutOff, 0.03f);
	  ImGui::ColorEdit4("ObjectColor", glm::value_ptr(m_ObjectColor), 0.03f);
	  ImGui::End();
	}

	{
	  // Texture
	  ImGui::Begin("Texture");
	  ImGui::Text("Default Texture");
	 // if (ImGui::ImageButton((void*)m_DefaultTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	 // {
		//m_DefaultTexture->Bind();
	 // }
	 // if (ImGui::ImageButton((void*)m_DiffuseTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	 // {
		//m_DiffuseTexture->Bind(0);
	 // }
	 // if (ImGui::ImageButton((void*)m_SpecularTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	 // {
		//m_SpecularTexture->Bind(0);
	 // }
	  ImGui::End();
	}

	{
	  //material
	  ImGui::Begin("Material");
	  ImGui::DragFloat3("Ambient", glm::value_ptr(materialData->Ambient), 0.3f);
	  ImGui::DragFloat3("Diffuse", glm::value_ptr(materialData->Diffuse), 0.3f);
	  ImGui::DragFloat3("Specular", glm::value_ptr(materialData->Specular), 0.3f);
	  ImGui::DragFloat("Shininess", &(materialData->Shininess), 0.5f);

	  ImGui::Separator();
	  ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_PerspectiveCameraEntity.GetComponent<TransformComponent>().Transform[3]));
	  if (ImGui::Checkbox("PerspectiveCamer", &Perspective))
	  {
		m_PerspectiveCameraEntity.GetComponent<CameraComponent>()._Perspective = Perspective;
		m_OrthographicCameraEntity.GetComponent<CameraComponent>()._Perspective = !Perspective;
	  }
	  ImGui::End();
	}

	
	{
	  ImGui::Begin("FileDialog");
	  // open Dialog Simple
	  if (ImGui::Button("Open File Dialog"))
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj,.cpp,.h,.hpp", ".");
	  // display
	  ImGui::ProgressBar(GetPro());
	  ImGui::ProgressBar(Mesh::GetReadPro());
	  //EP_CORE_TRACE("Editor pro : {0}", Mesh::GetPro());

	  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	  {
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
		  std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
		  std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
		  // action
		  //m_Fu = std::async(Mesh::CreateMesh, filePathName, filePath, true);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	  }
	  ImGui::End();
	}
	

	ImGui::End();
  }

}