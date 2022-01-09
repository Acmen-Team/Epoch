#pragma once

#include "Epoch/Graphics/Renderer/RendererAPI.h"

namespace Epoch {

  class OpenGLRendererAPI : public RendererAPI
  {
  public:
	OpenGLRendererAPI() {}
	~OpenGLRendererAPI() = default;

	virtual void Init() override;
	virtual void SetViewport(float width, float height) override;
	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void SetRenderModel(RenderModel& model) override;
	virtual void Clear() override;

	virtual void SetRenderState() override;
	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
  };

}