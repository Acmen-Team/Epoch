#include "eppch.h"

#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "FileDialog.h"

#include "Epoch/Resource/ResourceManager.h"
#include "Epoch/Core/Math/Math.h"
#include "Epoch/Core/KeyCodes.h"
//#include "Epoch/Scene/SceneSerialize.h"

#include "ImGuizmo.h"

#include <thread>

namespace Epoch {

  extern const std::filesystem::path g_AssetPath;

  EditorLayer::EditorLayer() : Layer("Example"), m_CameraController(1.6f / 0.9f)
  {

  }

  void EditorLayer::OnAttach()
  {
	m_Offline = new Offline();

	// Load module resource
	m_Fu = std::async(&ResourceAllocator<Mesh>::AddRes, ResourceManager::Get().GetAllocator(), "assets/models/cube.obj", "assets/models/");
	m_Fu.wait();
	m_Fu = std::async(&ResourceAllocator<Mesh>::AddRes, ResourceManager::Get().GetAllocator(), "assets/models/file.obj", "assets/models/");
	m_Fu.wait();

	// Load shader resource
	m_ShaderLibrary.Load("Phone", "assets/shaders/Phone.glsl");
	m_ShaderLibrary.Load("TestShading", "assets/shaders/Phone.glsl");
	m_ShaderLibrary.Load("ColorShading", "assets/shaders/Color.glsl");
	m_ShaderLibrary.Load("PhoneShading", "assets/shaders/PhongShading.glsl");

	// Create Framebuffer
	Epoch::FramebufferSpecification fbSpec;
	fbSpec.Width = 1366;
	fbSpec.Height = 768;
	m_Framebuffer = Epoch::Framebuffer::Create(fbSpec);

	m_Texture = Texture2D::Create("assets/textures/default_w.jpg");
	m_PlayBarTexture = Texture2D::Create("assets/textures/toolbar/PlayBar.png");
	m_PauseBarTexture = Texture2D::Create("assets/textures/toolbar/PauseBar.png");
	m_StopBarTexture = Texture2D::Create("assets/textures/toolbar/StopBar.png");
	m_DownloadBarTexture = Texture2D::Create("assets/textures/toolbar/DownloadBar.png");

	m_SettingTexture = Texture2D::Create("assets/Resource/Icon/Setting.png");
	m_TransTexture = Texture2D::Create("assets/Resource/Icon/TransIcon.png");
	m_RotateTexture = Texture2D::Create("assets/Resource/Icon/RotateIcon.png");
	m_ScaleTexture = Texture2D::Create("assets/Resource/Icon/ScaleIcon.png");
	//m_PlayBarTexture = Texture2D::Create("assets/textures/face.png");

	m_Scene = std::make_shared<Scene>();

	Entity redCube = m_Scene->CreatEntity("CubeA");
	redCube.AddComponent<MeshComponent>(ResourceManager::Get().GetAllocator()->GetRes("assets/models/cube.obj"));

	Entity greeCube = m_Scene->CreatEntity("CubeB");

	m_SceneHierarchyPanel.SetContext(m_Scene);

	m_shader = Shader::Create("assets/shaders/Phone.glsl");
	m_colShader = m_ShaderLibrary.Get("ColorShading");
  }

  void EditorLayer::OnDetach()
  {

  }

  void EditorLayer::OnUpdate(Timestep timestep)
  {
	PROFILE_SCOPE("EditorLayer::OnUpdate");

	if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
	  m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
	  (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	{
	  m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

	  m_CameraController.OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}
	// Resize
	//if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
	//	m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
	//	(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
	//{
	//  
	//}

	if (m_ViewPanelFocused)
	  m_CameraController.OnUpdate(timestep);

	m_Framebuffer->Bind();

	{
	  PROFILE_SCOPE("Render::Pro");
	  Epoch::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	  Epoch::RenderCommand::Clear();
	  Epoch::RenderCommand::SetRenderModel(m_RenderModel);
	}

	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->use();
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformInt("u_Texture1", 0);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("ObjectColor", m_ObjectColor);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("ViewPosition", m_CameraController.GetCameraPosition());
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("material.ambient", materialData->Ambient);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("material.diffuse", materialData->Diffuse);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("material.specular", materialData->Specular);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat("material.shininess", materialData->Shininess);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("light.ambient", lightData->Ambient);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("light.diffuse", lightData->Diffuse);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("light.specular", lightData->Specular);
	std::dynamic_pointer_cast<Epoch::Shader>(m_shader)->UploadUniformFloat3("light.position", lightData->Position);

	// Begin Rendering
	{
	  PROFILE_SCOPE("Rendering::Begin Scene");
	  Renderer::BeginScene(m_CameraController.GetCamera());

	  Entity selectEntity = m_SceneHierarchyPanel.GetSelectEntity();

	  m_Texture->Bind();
	  m_Scene->SetShader(m_shader);
	  m_Scene->SetSelectEntity(selectEntity);
	  m_Scene->OnUpdate(timestep);

	  if (!selectEntity.IsNullEntity() && selectEntity.HasComponent<MeshComponent>())
	  {
		auto mesh = selectEntity.GetComponent<MeshComponent>();
		auto trans = selectEntity.GetComponent<TransformComponent>();

		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // 禁止模板缓冲的写入
		glDisable(GL_DEPTH_TEST);
		m_colShader->use();
		std::dynamic_pointer_cast<Shader>(m_colShader)->UploadUniformMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
		trans.Scale += glm::vec3(0.02, 0.02, 0.02);

		std::dynamic_pointer_cast<Shader>(m_colShader)->UploadUniformMat4("u_Transform", trans.GetTransform());

		for (auto shap : mesh._Mesh->GetMesh())
		{
		  shap.second->Bind();
		  glDrawElements(GL_TRIANGLES, shap.second->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		glDisable(GL_STENCIL_TEST);
	  }

	  Renderer::EndScene();
	}

	m_Framebuffer->UnBind();
  }

  void EditorLayer::OnEvent(Event& event)
  {
	//EP_CORE_INFO("{0}", event.GetName());

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(EP_BIND_EVENT_FN(EditorLayer::OnKeyPressed));

	m_CameraController.OnEvent(event);
  }

  template<typename UIFunction>
  static void SceneToolbar(ImGuiDockNode* node, const float DISTANCE, int* corner, UIFunction uiFunc)
  {
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	ImVec2 work_area_pos = node->Pos;
	ImVec2 work_area_size = node->Size;

	if (*corner != -1)
	{
	  window_flags |= ImGuiWindowFlags_NoMove;
	  ImGuiViewport* viewport = ImGui::GetMainViewport();

	  ImVec2 window_pos = ImVec2((*corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (*corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
	  ImVec2 window_pos_pivot = ImVec2((*corner & 1) ? 1.0f : 0.0f, (*corner & 2) ? 1.0f : 0.0f);
	  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	  ImGui::SetNextWindowViewport(node->ID);
	}
	ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background

	uiFunc(corner, work_area_size, window_flags);
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
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 250.0f;

	auto menubareHeight = ImGui::GetCurrentWindow()->MenuBarHeight();

	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
	  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
	  if (ImGui::BeginMenu("File"))
	  {
		// Disabling fullscreen would allow the window to be moved to the front of other windows,
		// which we can't undo at the moment without finer window depth/z control.
		if (ImGui::MenuItem("Open"));
		  //SceneSerialize.DeSerialize();

		if (ImGui::MenuItem("Save"));
		  //SceneSerialize.Serialize();

		if (ImGui::MenuItem("Exit"))
		  Epoch::Application::Get().Exit();

		ImGui::EndMenu();
	  }

	  if (ImGui::BeginMenu("Edit"))
	  {
		// Disabling fullscreen would allow the window to be moved to the front of other windows,
		// which we can't undo at the moment without finer window depth/z control.
		//if (ImGui::MenuItem(u8"�˳�"))
		  //Epoch::Application::Get().Exit();

		ImGui::EndMenu();
	  }

	  if (ImGui::BeginMenu("Options"))
	  {
		// Disabling fullscreen would allow the window to be moved to the front of other windows,
		// which we can't undo at the moment without finer window depth/z control.
		//if (ImGui::MenuItem(u8"�˳�"))
		  //Epoch::Application::Get().Exit();

		ImGui::EndMenu();
	  }

	  if (ImGui::BeginMenu("Help"))
	  {
		// Disabling fullscreen would allow the window to be moved to the front of other windows,
		// which we can't undo at the moment without finer window depth/z control.
		//if (ImGui::MenuItem(u8"�˳�"))
		  //Epoch::Application::Get().Exit();

		ImGui::EndMenu();
	  }

	  ImGuiAxis toolbar1_axis = ImGuiAxis_X;
	  DockingToolbar("Toolbar1", &toolbar1_axis);

	  // ImGuiViewport* viewport = ImGui::GetMainViewport();
	  // ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + ImGui::GetWindowHeight()));
	  // ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - ImGui::GetWindowHeight()));
	  // ImGui::SetNextWindowViewport(viewport->ID);

	  // ImGuiWindowFlags flags = 0
		 //| ImGuiWindowFlags_NoDocking
		 //| ImGuiWindowFlags_NoTitleBar
		 //| ImGuiWindowFlags_NoResize
		 //| ImGuiWindowFlags_NoMove
		 //| ImGuiWindowFlags_NoScrollbar
		 //| ImGuiWindowFlags_NoSavedSettings
		 //;
	  // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	  // ImGui::Begin("TOOLBAR", NULL, flags);
	  // ImGui::PopStyleVar();

	  // ImGui::SameLine();
	  // ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
	  // if (ImGui::ImageButton((void*)m_PlayBarTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight())))
	  // {
		 ////make_visible();
	  // }
	  // ImGui::PopStyleColor();

	  // ImGui::End();

	  ImGui::EndMenuBar();
	}


	//if (1) { ImGui::ShowMetricsWindow(); }
	//if (1) { ImGui::Begin("Style Editor", nullptr, ImGuiWindowFlags_NoDocking); ImGui::ShowStyleEditor(); ImGui::End(); }
	//if (1) { ImGui::ShowDemoWindow(); }

	m_SceneHierarchyPanel.OnImGuiRender();

	m_ContentBrowserPanel.OnImGuiRender();

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

	  ImGuiDockNode* node = ImGui::GetWindowDockNode();
	  node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	
	  m_ViewPanelFocused = ImGui::IsWindowFocused();
	  m_ViewPanelHovered = ImGui::IsWindowHovered();
	  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	  m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	  ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

	  // ImGuizmo
	  Entity selectEntity = m_SceneHierarchyPanel.GetSelectEntity();
	  if (selectEntity)
	  {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		const glm::mat4& cameraProjection = m_CameraController.GetCamera().GetProjectionMatrix();
		glm::mat4 cameraView = m_CameraController.GetCamera().GetViewMatrix();
		//glm::mat4 cameraView = glm::inverse(m_CameraController.GetCamera().GetTransform());

		auto& tc = selectEntity.GetComponent<TransformComponent>();

		glm::mat4 transformation = tc.GetTransform();

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transformation));

		if (ImGuizmo::IsUsing())
		{
		  glm::vec3 translation, rotation, scale;
		  Math::DecomposeTransform(transformation, translation, rotation, scale);

		  tc.Translation = translation;
		  glm::vec3 deltaRotation = rotation - tc.Rotation;
		  tc.Rotation += deltaRotation;
		  tc.Scale = scale;
		  
		}
	  }

	  static int settingCorner = 0;

	  SceneToolbar(node, 10.0f, &settingCorner, [&](int* corner, const ImVec2& work_area_size, const ImGuiWindowFlags m_window_flags) {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		float buttonSize = 25.0f;
		if (ImGui::Begin("Setting Toolbar", p_open, m_window_flags))
		{
		  if (work_area_size.x < 100)
		  {
			buttonSize = work_area_size.x / 4;
		  }

		  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		  if (ImGui::ImageButton((void*)m_SettingTexture->GetRendererID(), ImVec2(m_SettingTexture->GetWidth(), m_SettingTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 0 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		  {
			m_GizmoType = 0;
		  }
		  ImGui::PopStyleColor(3);
		  // Debug
		  /*
		  ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		  ImGui::Separator();
		  if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		  else
			ImGui::Text("Mouse Position: <invalid>");
		  if (ImGui::BeginPopupContextWindow())
		  {
			if (ImGui::MenuItem("Custom", NULL, *corner == -1)) *corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, *corner == 0)) *corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, *corner == 1)) *corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, *corner == 2)) *corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, *corner == 3)) *corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		  }
		  */
		}

		ImGui::End();
		ImGui::PopStyleVar();
	  });

	  static int transCorner = 1;
	  SceneToolbar(node, 10.0f, &transCorner, [&](int* corner, const ImVec2& work_area_size, const ImGuiWindowFlags m_window_flags) {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		float buttonSize = 25.0f;
		if (ImGui::Begin("Transform Toolbar", p_open, m_window_flags))
		{
		  if (work_area_size.x < 100)
		  {
			buttonSize = work_area_size.x / 4;
		  }

		  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		  if (ImGui::ImageButton((void*)m_TransTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 0 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		  {
			m_GizmoType = 0;
		  }
		  ImGui::SameLine();
		  if (ImGui::ImageButton((void*)m_RotateTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 1 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		  {
			m_GizmoType = 1;
		  }
		  ImGui::SameLine();
		  if (ImGui::ImageButton((void*)m_ScaleTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 2 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		  {
			m_GizmoType = 2;
		  }
		  ImGui::PopStyleColor(3);

		  // Debug
		  /*
		  ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		  ImGui::Separator();
		  if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		  else
			ImGui::Text("Mouse Position: <invalid>");
		  if (ImGui::BeginPopupContextWindow())
		  {
			if (ImGui::MenuItem("Custom", NULL, *corner == -1)) *corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, *corner == 0)) *corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, *corner == 1)) *corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, *corner == 2)) *corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, *corner == 3)) *corner = 3;
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::EndPopup();
		  }
		  */
		}

		ImGui::End();
		ImGui::PopStyleVar();
	  });

	  // Setting(Top-left)
	  /*
	  const float DISTANCE = 10.0f;
	  static int corner = 0;
	  ImGuiIO& io = ImGui::GetIO();
	  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	  ImVec2 work_area_pos = node->Pos;
	  ImVec2 work_area_size = node->Size;

	  if (corner != -1)
	  {
		window_flags |= ImGuiWindowFlags_NoMove;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(node->ID);
	  }
	  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

	  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	  float buttonSize = 25.0f;
	  if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
	  {
		if (work_area_size.x < 100)
		{
		  buttonSize = work_area_size.x / 4;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((void*)m_SettingTexture->GetRendererID(), ImVec2(m_SettingTexture->GetWidth(), m_SettingTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 0 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		{
		  m_GizmoType = 0;
		}
		ImGui::PopStyleColor(3);

		ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
		  ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
		  ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
		  if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
		  if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
		  if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
		  if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
		  if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
		  if (p_open && ImGui::MenuItem("Close")) *p_open = false;
		  ImGui::EndPopup();
		}
	  }
	  */

	  // Transform(Top-right)
	  /*
	  const float DISTANCE = 10.0f;
	  static int corner = 1;
	  ImGuiIO& io = ImGui::GetIO();
	  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	  ImVec2 work_area_pos = node->Pos;
	  ImVec2 work_area_size = node->Size;

	  if (corner != -1)
	  {
		window_flags |= ImGuiWindowFlags_NoMove;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		
		ImVec2 window_pos = ImVec2((corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE) : (work_area_pos.x + DISTANCE), (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE) : (work_area_pos.y + DISTANCE));
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowViewport(node->ID);
	  }
	  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

	  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	  float buttonSize = 25.0f;
	  if (ImGui::Begin("Example: Simple overlay", p_open, window_flags))
	  {
		if (work_area_size.x < 100)
		{
		  buttonSize = work_area_size.x / 4;
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((void*)m_TransTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 0 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		{
		  m_GizmoType = 0;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)m_RotateTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 1 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		{
		  m_GizmoType = 1;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)m_ScaleTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), m_GizmoType == 2 ? ImVec4(1.0f, 1.0f, 0.0f, 0.9f) : ImVec4(1.0f, 1.0f, 0.0f, 0.5f)))
		{
		  m_GizmoType = 2;
		}
		ImGui::PopStyleColor(3);

		// Debug
		
		ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
		  ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
		  ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow())
		{
		  if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
		  if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
		  if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
		  if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
		  if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
		  if (p_open && ImGui::MenuItem("Close")) *p_open = false;
		  ImGui::EndPopup();
		}
		
	  }
	  */

	  //ImGui::End();
	  //ImGui::PopStyleVar();
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

	//{
	//  // Texture
	//  ImGui::Begin("Texture");
	//  ImGui::Text("Default Texture");
	//  if (ImGui::ImageButton((void*)m_DefaultTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	//  {
	//	m_DefaultTexture->Bind();
	//  }
	//  if (ImGui::ImageButton((void*)m_DiffuseTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	//  {
	//	m_DiffuseTexture->Bind(0);
	//  }
	//  if (ImGui::ImageButton((void*)m_SpecularTexture->GetRendererID(), ImVec2{ 64.0f, 64.0f }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
	//  {
	//	m_SpecularTexture->Bind(0);
	//  }
	//  ImGui::End();
	//}
	{
	  ImGui::Begin("Offline");
	  ImGuiDockNode* node = ImGui::GetWindowDockNode();
	  node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	  if (m_OfflineTexture)
		ImGui::Image((void*)m_OfflineTexture->GetRendererID(), ImVec2(m_OfflineTexture->GetWidth(), m_OfflineTexture->GetHeight()), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
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

	{
	  ImGui::Begin("Camera");
	  const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
	  const char* currentProjectionTypeString = projectionTypeStrings[(int)m_CameraController.GetCamera().GetProjectionType()];

	  ImGui::Checkbox("Fixed Aspect Ratio", &(m_CameraController.GetCamera().fixedAspectration));

	  if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
	  {
		for (int i = 0; i < 2; i++)
		{
		  bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
		  if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
		  {
			currentProjectionTypeString = projectionTypeStrings[i];
			m_CameraController.GetCamera().SetProjectionType((SceneCamera::ProjectionType)i);
		  }

		  if (isSelected)
			ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	  }
	  // Setting perspective camera
	  if (m_CameraController.GetCamera().GetProjectionType() == SceneCamera::ProjectionType::Perspective)
	  {
		float persFov = m_CameraController.GetCamera().GetPerspectiveFov();
		if (ImGui::DragFloat("Fov", &persFov, 0.05f, 0.1f))
		  m_CameraController.GetCamera().SetPerspectiveFov(persFov);

		float persNear = m_CameraController.GetCamera().GetPerspectiveNear();
		if (ImGui::DragFloat("Near", &persNear))
		  m_CameraController.GetCamera().SetPerspectiveNear(persNear);

		float persFar = m_CameraController.GetCamera().GetPerspectiveFar();
		if (ImGui::DragFloat("Far", &persFar))
		  m_CameraController.GetCamera().SetPerspectiveFar(persFar);
	  }
	  // Setting ortho camera
	  if (m_CameraController.GetCamera().GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
	  {
		float orthoSize = m_CameraController.GetCamera().GetOrthographicSize();
		if (ImGui::DragFloat("Size", &orthoSize, 0.05f, 0.1f, 50.0f))
		  m_CameraController.GetCamera().SetOrthographicSize(orthoSize);

		float orthoNear = m_CameraController.GetCamera().GetOrthographicNear();
		if (ImGui::DragFloat("Near", &orthoNear))
		  m_CameraController.GetCamera().SetOrthographicNear(orthoNear);

		float orthoFar = m_CameraController.GetCamera().GetOrthographicFar();
		if (ImGui::DragFloat("Far", &orthoFar))
		  m_CameraController.GetCamera().SetOrthographicFar(orthoFar);
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

	  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	  {
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
		  std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
		  std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
		  // action
		  m_Fu = std::async(&ResourceAllocator<Mesh>::AddRes, ResourceManager::Get().GetAllocator(), filePathName, filePath);

		  if (m_Fu._Is_ready())
		  {
			int id = m_Fu.get();
			if (-1 != 0)
			{
			  EP_CORE_INFO("Finish Load Resource");
			}
		  }
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	  }
	  ImGui::End();
	}


	ImGui::End();
  }

  bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
  {
	if (e.GetRepeatCount() > 0)
	  return false;

	switch (e.GetKeyCode())
	{
	  case EP_KEY_SPACE:
	  {
	    if (m_GizmoType < 2)
	  	  ++m_GizmoType;
	    else
	  	  m_GizmoType = 0;

		break;
	  }
	}
  }

  void EditorLayer::DockingToolbar(const char* name, ImGuiAxis* p_toolbar_axis)
  {
	// [Option] Automatically update axis based on parent split (inside of doing it via right-click on the toolbar)
	// Pros:
	// - Less user intervention.
	// - Avoid for need for saving the toolbar direction, since it's automatic.
	// Cons: 
	// - This is currently leading to some glitches.
	// - Some docking setup won't return the axis the user would expect.
	const bool TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED = true;

	// ImGuiAxis_X = horizontal toolbar
	// ImGuiAxis_Y = vertical toolbar
	ImGuiAxis toolbar_axis = *p_toolbar_axis;

	// 1. We request auto-sizing on one axis
	// Note however this will only affect the toolbar when NOT docked.
	ImVec2 requested_size = (toolbar_axis == ImGuiAxis_X) ? ImVec2(-1.0f, 0.0f) : ImVec2(0.0f, -1.0f);
	ImGui::SetNextWindowSize(requested_size);

	// 2. Specific docking options for toolbars.
	// Currently they add some constraint we ideally wouldn't want, but this is simplifying our first implementation
	ImGuiWindowClass window_class;
	window_class.DockingAllowUnclassed = true;
	window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
	window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar; // ImGuiDockNodeFlags_NoTabBar // FIXME: Will need a working Undock widget for _NoTabBar to work
	window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
	window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
	window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
	if (toolbar_axis == ImGuiAxis_X)
	  window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
	else
	  window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
	ImGui::SetNextWindowClass(&window_class);

	// 3. Begin into the window
	const float font_size = ImGui::GetFontSize();
	const ImVec2 icon_size(ImFloor(font_size * 1.7f), ImFloor(font_size * 1.7f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin(name, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	// 4. Overwrite node size
	ImGuiDockNode* node = ImGui::GetWindowDockNode();
	if (node != NULL)
	{
	  // Overwrite size of the node
	  ImGuiStyle& style = ImGui::GetStyle();
	  const ImGuiAxis toolbar_axis_perp = (ImGuiAxis)(toolbar_axis ^ 1);
	  const float TOOLBAR_SIZE_WHEN_DOCKED = style.WindowPadding[toolbar_axis_perp] * 2.0f + icon_size[toolbar_axis_perp];
	  node->WantLockSizeOnce = true;
	  node->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] = TOOLBAR_SIZE_WHEN_DOCKED;

	  if (TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
		if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None)
		  toolbar_axis = (ImGuiAxis)(node->ParentNode->SplitAxis ^ 1);
	}

	// 5. Dummy populate tab bar
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
	//UndockWidget(icon_size, toolbar_axis);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.9f, 0.2f, 0.2f, 0.0f });
	//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

	static int current = 0;
	uint32_t currentBar;
	if (current % 2)
	{
	  currentBar = m_PauseBarTexture->GetRendererID();
	}
	else
	{
	  currentBar = m_PlayBarTexture->GetRendererID();
	}

	ImGui::SameLine(contentRegionAvailable.x / 2 - m_PlayBarTexture->GetWidth());
	//ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
	if (ImGui::ImageButton((void*)m_StopBarTexture->GetRendererID(), ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f)))
	{
	  //current++;
	  //m_OfflineFu = new std::future(std::async(std::launch::async, &Offline::BeginScene, m_Offline));
	}

	ImGui::SameLine();

	//ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
	if (ImGui::ImageButton((void*)currentBar, ImVec2(m_PlayBarTexture->GetWidth(), m_PlayBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f)))
	{
	  current++;
	  m_OfflineFu = new std::future(std::async(std::launch::async, &Offline::BeginScene, m_Offline));
	}

	if (m_OfflineFu && m_OfflineFu->_Is_ready())
	{
	  m_OfflineTexture = Texture2D::Create("assets/textures/Offline.png");
	  current++;
	  m_OfflineFu->~future();
	  delete m_OfflineFu;
	  m_OfflineFu = nullptr;
	}

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4{ 0.35f, 0.85f, 0.15f, 0.3f });
	int ProgressBarWidth = contentRegionAvailable.x / 2 - m_PlayBarTexture->GetWidth() - m_StopBarTexture->GetWidth() - 10;
	ImGui::ProgressBar(m_Offline->Progress, ImVec2(ProgressBarWidth, m_PlayBarTexture->GetWidth()), "");
	ImGui::PopStyleColor(2);

	ImGui::SameLine(contentRegionAvailable.x - m_DownloadBarTexture->GetWidth());
	if (ImGui::ImageButton((void*)m_DownloadBarTexture->GetRendererID(), ImVec2(m_DownloadBarTexture->GetWidth(), m_DownloadBarTexture->GetHeight()), ImVec2(0, 1), ImVec2(1, 0), 0.0f, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(0.0f, 1.0f, 0.0f, std::max(0.3f, m_Offline->Progress))))
	{

	}

	//ImGui::PopStyleColor(2);
	ImGui::PopStyleColor(3);

	// for (int icon_n = 0; icon_n < 10; icon_n++)
	// {
	   //char label[32];
	   //ImFormatString(label, IM_ARRAYSIZE(label), "%02d", icon_n);
	   //if (icon_n > 0 && toolbar_axis == ImGuiAxis_X)
	   //  ImGui::SameLine();
	   //ImGui::Button(label, icon_size);
	// }
	ImGui::PopStyleVar(2);

	// 6. Context-menu to change axis
	if (node == NULL || !TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
	{
	  if (ImGui::BeginPopupContextWindow())
	  {
		ImGui::TextUnformatted(name);
		ImGui::Separator();
		if (ImGui::MenuItem("Horizontal", "", (toolbar_axis == ImGuiAxis_X)))
		  toolbar_axis = ImGuiAxis_X;
		if (ImGui::MenuItem("Vertical", "", (toolbar_axis == ImGuiAxis_Y)))
		  toolbar_axis = ImGuiAxis_Y;
		ImGui::EndPopup();
	  }
	}

	ImGui::End();
	ImGui::PopStyleVar();

	// Output user stored data
	*p_toolbar_axis = toolbar_axis;
  }

}