#pragma once

namespace Epoch {
  
  class Offline
  {
  public:
	Offline()
	{
	  m_RendererID = new char[256 * 256 * 3];
	}

	bool BeginScene();

	char *m_RendererID;

	float Progress = 0;
  };

}