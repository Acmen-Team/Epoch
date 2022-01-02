#include "eppch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Epoch {

  RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}