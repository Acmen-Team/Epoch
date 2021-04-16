#pragma once

namespace Epoch {

  struct FramebufferSpecification
  {
	uint32_t Width, Height;

	uint32_t Samples = 1;

	bool SwapChainTarget = false;
  };

  class Framebuffer
  {
  public:
	virtual ~Framebuffer() = default;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;

	virtual uint32_t GetColorAttachmentRendererID() const = 0;

	virtual const FramebufferSpecification& GetSpecification() const = 0;

	static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
  };

}