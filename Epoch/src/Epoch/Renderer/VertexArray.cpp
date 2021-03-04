#include "eppch.h"

#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Epoch {

  VertexArray* VertexArray::Create()
  {
	return new OpenGLVertexArray();
  }

}