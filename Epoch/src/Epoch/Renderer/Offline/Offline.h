#pragma once
#include "Epoch/Math/Vec.h"
#include "Epoch/Renderer/Offline/Core/Ray.h"

namespace Epoch {

  class Offline
  {
  public:
	Offline()
	{
	}

	bool BeginScene();

	Color RayColor(const Ray& ray);

	char *m_RendererID;

	float Progress = 0;
  };

}