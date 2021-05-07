#pragma once
#include "Epoch.h"

namespace Epoch {

  class EditorLayer : public Layer
  {
  public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	void LoadModel(const char* filename, const char* basepath,
	  bool triangulate);

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
	std::shared_ptr<Epoch::VertexArray> m_DragonVertexArray;

	Epoch::ShaderLibrary m_ShaderLibrary;

	std::shared_ptr<Epoch::Texture2D> m_Texture;
	std::shared_ptr<Epoch::Texture2D> m_CheckerboardTex;
	std::shared_ptr<Epoch::Texture2D> m_FaceTexture;
	std::shared_ptr<Epoch::Texture2D> m_StareTexture;

	//Light
	glm::vec4 m_LightColor = { 1.0f, 0.3f, 1.0f, 1.0f };
	glm::vec3 m_LightPosition = { 1.0f, 0.0f, 1.0f };

	glm::vec3 m_Position = { 1.0f, 0.0f, 1.0f };

	//ambientStrength
	float ambientStrength = 0.1f;
	float specularStrength = 0.3f;

	std::shared_ptr<Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
  };

}
