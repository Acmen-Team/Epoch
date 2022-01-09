#pragma once

#include "VertexArray.h"
#include "RenderData.h"

#include <glm/glm.hpp>

namespace Epoch {

  class RendererAPI
  {
  public:
	virtual void Init() = 0;
	virtual void SetViewport(float width, float height) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void SetRenderModel(RenderModel& model) = 0;
	virtual void Clear() = 0;

	virtual void SetRenderState() = 0;
	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
  };

}