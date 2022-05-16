#pragma once
#include "Epoch.h"
#include "Epoch/Utility/Timer.h"
#include "Epoch/Events/KeyEvent.h"

#include "Editor/Panel/SceneHierarchyPanel.h"
#include "Editor/Panel/ContentBrowserPanel.h"

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

namespace Epoch {

  class EditorLayer : public Layer
  {
  public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnEvent(Event& event) override;

	virtual void OnImGuiRender() override;

  protected:
	void DockingToolbar(const char* name, ImGuiAxis* p_toolbar_axis);

  private:
	bool OnKeyPressed(KeyPressedEvent& e);

  private:
	Epoch::ShaderLibrary* m_ShaderLibrary;

	SceneCameraController m_CameraController;
	// Scene
	std::shared_ptr<Scene> m_Scene;

	bool Perspective = true;
	// Async Resource
	std::future<int> m_Fu;
	std::future<bool>* m_OfflineFu = nullptr;

	Offline *m_Offline = nullptr;

	glm::vec4 m_ObjectColor = { 1.0, 1.0, 1.0, 1.0 };

	std::shared_ptr<Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	Ref<Texture> m_Texture;

	Ref<Texture> m_PlayBarTexture;
	Ref<Texture> m_PauseBarTexture;
	Ref<Texture> m_StopBarTexture;
	Ref<Texture> m_DownloadBarTexture;
	Ref<Texture> m_OfflineTexture;
	Ref<Texture> m_SettingTexture;
	Ref<Texture> m_TransTexture;
	Ref<Texture> m_RotateTexture;
	Ref<Texture> m_ScaleTexture;

	Ref<Shader> m_shader;
	Ref<Shader> m_colShader;

	MaterialData* materialData = Material::Create();

	int renderModel_idx = 0;
	RenderModel m_RenderModel = RenderModel::Fill;

	int lightType = 0;

	int m_GizmoType = 0;

	bool m_ViewPanelFocused = true;
	bool m_ViewPanelHovered = true;

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	ContentBrowserPanel m_ContentBrowserPanel;

	struct ProfileResult
	{
	  const char* Name;
	  float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	#define  PROFILE_SCOPE(name) Timer timer##_LINE_(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
  };

}
