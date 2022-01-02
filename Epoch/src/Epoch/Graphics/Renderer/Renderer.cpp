#include "eppch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Epoch {

  Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

  void Renderer::Init()
  {
	RenderCommand::Init();
  }

  void Renderer::WindowResize(float width, float height)
  {
	RenderCommand::SetViewport(width, height);
  }

  void Renderer::BeginScene(SceneCamera& camera)
  {
	m_SceneData->ViewProjectMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::BeginScene(Camera& camera, const glm::mat4& transform)
  {
	m_SceneData->ViewProjectMatrix = camera.GetProjection() * transform;
  }

  void Renderer::EndScene()
  {
  }

  void Renderer::SetRenderModel(RenderModel& model)
  {
	RenderCommand::SetRenderModel(model);
  }

  void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
  {
	shader->use();

	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectMatrix);
	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

	vertexArray->Bind();
	RenderCommand::DrawIndexed(vertexArray);
  }

}