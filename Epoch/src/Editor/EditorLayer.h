#pragma once
#include "Epoch.h"
#include "Panel/SceneHierarchyPanel.h"

#include <chrono>

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

template<typename Fn>
class Timer
{
public:
  Timer(const char* name, Fn&& func)
	: m_Name(name), m_Func(func), m_Stopped(false)
  {
	m_StartTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
	if (!m_Stopped)
	  Stop();
  }

  void Stop()
  {
	auto endTimepoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	m_Stopped = true;

	float duration = (end - start) * 0.001f;

	m_Func({ m_Name, duration });
  }
private:
  const char* m_Name;
  std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
  bool m_Stopped;
  Fn m_Func;
};


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

	void DockingToolbar(const char* name, ImGuiAxis* p_toolbar_axis);
  private:
	Epoch::ShaderLibrary m_ShaderLibrary;

	SceneCameraController m_CameraController;
	// Scene
	std::shared_ptr<Scene> m_Scene;

	bool Perspective = true;
	// Async Resource
	std::future<int> m_Fu;

	Offline *m_Offline;
	//Light
	glm::vec3 m_LightColor = { 1.0f, 0.3f, 1.0f };
	glm::vec3 m_LightPosition = { 1.0f, 0.0f, 1.0f };

	glm::vec4 m_ObjectColor = { 0.8, 0.7, 0.3, 1.0 };

	glm::vec3 m_Position = { 1.0f, 0.0f, 1.0f };

	std::shared_ptr<Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	Ref<Texture> m_Texture;

	Ref<Texture> m_PlayBarTexture;
	Ref<Texture> m_PauseBarTexture;
	Ref<Texture> m_StopBarTexture;
	Ref<Texture> m_DownloadBarTexture;
	Ref<Texture> m_OfflineTexture;

	Ref<Shader> m_shader;

	MaterialData* materialData = Material::Create();
	LightData* lightData = Light::Create();

	int renderModel_idx = 0;
	RenderModel m_RenderModel = RenderModel::Fill;

	int lightType = 0;

	bool m_ViewPanelFocused = true;
	bool m_ViewPanelHovered = true;

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;

	struct ProfileResult
	{
	  const char* Name;
	  float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	#define  PROFILE_SCOPE(name) Timer timer##_LINE_(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
  };

}
