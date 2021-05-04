#pragma once
#include "Epoch.h"

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
	std::shared_ptr<Epoch::VertexArray> m_TestVertexArray;

	Epoch::ShaderLibrary m_ShaderLibrary;

	std::shared_ptr<Epoch::Texture2D> m_Texture;
	std::shared_ptr<Epoch::Texture2D> m_CheckerboardTex;
	std::shared_ptr<Epoch::Texture2D> m_FaceTexture;
	std::shared_ptr<Epoch::Texture2D> m_sTexture;
	std::shared_ptr<Epoch::Texture2D> m_StareTexture;
	std::shared_ptr<Epoch::Texture2D> m_ToonTexture;


	glm::vec3 m_SquarePosition = { -1.0f, -0.3f, 1.3f };

	//Light
	glm::vec4 m_LightColor = { 1.0f, 0.3f, 1.0f, 1.0f };
	glm::vec3 m_LightPosition = { 1.0f, 0.0f, 1.0f };
	//ambientStrength
	float ambientStrength = 0.1f;
	float specularStrength = 0.3f;

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

	std::shared_ptr<Framebuffer> m_Framebuffer;

	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
  };

}
