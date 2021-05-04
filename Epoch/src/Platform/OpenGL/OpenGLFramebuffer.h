#pragma once

#include "Epoch/Renderer/Framebuffer.h"

namespace Epoch {

  class OpenGLFramebuffer :public Framebuffer
  {
  public:
	OpenGLFramebuffer(const FramebufferSpecification& spec);
	virtual ~OpenGLFramebuffer();

	void Invalidate();

	virtual void Resize(uint32_t width, uint32_t height) override;

	virtual void Bind() override;
	virtual void UnBind() override;

	virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

	virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
  private:
	uint32_t m_RendererID = 0;
	uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
	FramebufferSpecification m_Specification;
  };

}