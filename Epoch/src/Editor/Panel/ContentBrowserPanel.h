#pragma once

#include <filesystem>

#include "Epoch/Renderer/Texture.h"

namespace Epoch {

  class ContentBrowserPanel
  {
  public:
	ContentBrowserPanel();

	void OnImGuiRender();
  private:
	std::filesystem::path m_CurrentDirectory;

	Ref<Texture> m_DirectoryIcon;
  };

}