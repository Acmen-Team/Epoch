#pragma once
#include "Epoch.h"

#include <chrono>

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
  private:
	//CameraController
	SceneCameraController m_CameraController;

	std::shared_ptr<Epoch::VertexArray> m_CubeVertexArray;
	std::shared_ptr<Epoch::VertexArray> m_PlaneVertexArray;
	std::shared_ptr<Epoch::VertexArray> m_BulbVertexArray;
	std::shared_ptr<Epoch::VertexArray> m_EarthVertexArray;
	std::shared_ptr<Epoch::VertexArray> m_BunnyVertexArray = nullptr;

	Epoch::ShaderLibrary m_ShaderLibrary;

	std::shared_ptr<Epoch::Shader> TestPhone;

	std::shared_ptr<Epoch::Texture2D> m_DefaultTexture;
	std::shared_ptr<Epoch::Texture2D> m_Texture;
	std::shared_ptr<Epoch::Texture2D> m_EarthTexture;
	std::shared_ptr<Epoch::Texture2D> m_CheckerboardTex;
	std::shared_ptr<Epoch::Texture2D> m_FaceTexture;
	std::shared_ptr<Epoch::Texture2D> m_StareTexture;
	std::shared_ptr<Epoch::Texture2D> m_DiffuseTexture;
	std::shared_ptr<Epoch::Texture2D> m_SpecularTexture;

	// Async Resource
	std::future<MeshData*> m_Fu;

	MeshData* bunnyData;

	//Light
	glm::vec3 m_LightColor = { 1.0f, 0.3f, 1.0f };
	glm::vec3 m_LightPosition = { 1.0f, 0.0f, 1.0f };

	glm::vec4 m_ObjectColor = { 0.8, 0.7, 0.3, 1.0 };

	glm::vec3 m_Position = { 1.0f, 0.0f, 1.0f };

	std::shared_ptr<Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

	MaterialData* materialData = Material::Create();
	LightData* lightData = Light::Create();

	int renderModel_idx = 0;
	RenderModel m_RenderModel = RenderModel::Fill;

	int lightType = 0;

	bool m_ViewPanelFocused = true;
	bool m_ViewPanelHovered = true;

	struct ProfileResult
	{
	  const char* Name;
	  float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	#define  PROFILE_SCOPE(name) Timer timer##_LINE_(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })
  };

}
