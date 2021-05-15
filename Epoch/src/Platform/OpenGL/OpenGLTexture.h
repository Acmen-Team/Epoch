#pragma once

#include "Epoch/Renderer/Texture.h"

namespace Epoch {

  class OpenGLTexture2D :public Texture2D
  {
  public:
	OpenGLTexture2D(const std::string& path);
	virtual ~OpenGLTexture2D() override;

	virtual uint32_t GetWidth() const override { return m_Width; }
	virtual uint32_t GetHeight() const override { return m_Height; }
	virtual uint32_t GetRendererID() const override { return m_RendererID; }

	virtual void Bind(uint32_t unit = 0) const override;
  private:
	std::string m_Path;

	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
  };

}