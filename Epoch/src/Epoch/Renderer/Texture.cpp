#include "eppch.h"

#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Epoch {

  std::shared_ptr<Epoch::Texture2D> Texture2D::Create(const std::string& path)
  {
    return std::make_shared<OpenGLTexture2D>(path);
  }

}