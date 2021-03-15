#include "eppch.h"

#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Epoch {

  OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	: m_Path(path)
  {
	int width, height, comp;
	stbi_set_flip_vertically_on_load(true);

	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &comp, 0);
	EP_CORE_ASSERT(data, "Failed to load image!");
	m_Width = width;
	m_Height = height;

	GLenum internalFormat = 0, dataFormat = 0;
	if (comp == 4)
	{
	  internalFormat = GL_RGBA8;
	  dataFormat = GL_RGBA;
	}
	else if (comp == 3)
	{
	  internalFormat = GL_RGB8;
	  dataFormat = GL_RGB;
	}

	EP_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
  }

  OpenGLTexture2D::~OpenGLTexture2D()
  {
	glDeleteTextures(1, &m_RendererID);
  }

  void OpenGLTexture2D::Bind(uint32_t unit /*= 0*/) const
  {
	glBindTextureUnit(unit, m_RendererID);
  }

}