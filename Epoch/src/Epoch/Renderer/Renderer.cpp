#include "eppch.h"

#include "Renderer.h"

namespace Epoch {

  Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

  void Renderer::BeginScene(PrespectiveCamera& camera)
  {
	m_SceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene()
  {
  }

  void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
  {
	shader->use();
	shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectMatrix);
	shader->UploadUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
  }

}