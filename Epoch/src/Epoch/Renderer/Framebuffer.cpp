#include "eppch.h"

#include "Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Epoch {

  std::shared_ptr<Epoch::Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
  {
	return std::make_shared<OpenGLFramebuffer>(spec);
  }

}