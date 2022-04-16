#include "eppch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Epoch {
  
  void OpenGLRendererAPI::Init()
  {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void OpenGLRendererAPI::SetViewport(float width, float height)
  {
	glViewport(0, 0, width, height);
  }
  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
  {
	glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRendererAPI::SetRenderModel(RenderModel& model)
  {
	switch (model)
	{
	  case RenderModel::Fill: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
	  case RenderModel::Line: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	  case RenderModel::Point: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
	}
  }

  void OpenGLRendererAPI::Clear()
  {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void OpenGLRendererAPI::SetRenderState()
  {
	glClearStencil(0x00);
	glClear(GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);

	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
  }

  void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
  {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

	glDisable(GL_STENCIL_TEST);
  }

}