#pragma once

#include "RenderCommand.h"
#include "SceneCamera.h"
#include "Camera.h"
#include "Shader.h"

namespace Epoch {

  class Renderer
  {
  public:
	static void Init();

	static void WindowResize(float width, float height);

	static void BeginScene(SceneCamera& camera);
	static void BeginScene(Camera& camera, const glm::mat4& transform);
	static void EndScene();

	static void SetRenderModel(RenderModel& model);

	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
  private:
	struct SceneData
	{
	  glm::mat4 ViewProjectMatrix;
	};

	static SceneData* m_SceneData;
  };

}