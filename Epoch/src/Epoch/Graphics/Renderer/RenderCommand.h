#pragma once

#include "RendererAPI.h"

namespace Epoch {

  class RenderCommand
  {
  public:
	inline static void Init()
	{
	  s_RendererAPI->Init();
	}
	inline static void SetViewport(float width, float height)
	{
	  s_RendererAPI->SetViewport(width, height);
	}
	inline static void SetClearColor(const glm::vec4& color)
	{
	  s_RendererAPI->SetClearColor(color);
	}
	inline static void SetRenderModel(RenderModel& model)
	{
	  s_RendererAPI->SetRenderModel(model);
	}
	inline static void Clear()
	{
	  s_RendererAPI->Clear();
	}

	inline static void DrawIndexed(const std::shared_ptr<VertexArray>& VertexArray)
	{
	  s_RendererAPI->DrawIndexed(VertexArray);
	}
  private:
	static RendererAPI* s_RendererAPI;
  };

}