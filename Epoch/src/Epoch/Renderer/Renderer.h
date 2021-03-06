#pragma once

#include "RenderCommand.h"
#include "PrespectiveCamera.h"
#include "Shader.h"

namespace Epoch {

  class Renderer
  {
  public:
	static void BeginScene(PrespectiveCamera& camera);
	static void EndScene();

	static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
  private:
	struct SceneData
	{
	  glm::mat4 ViewProjectMatrix;
	};

	static SceneData* m_SceneData;
  };

}