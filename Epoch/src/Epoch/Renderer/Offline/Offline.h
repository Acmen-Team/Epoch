#pragma once
#include "Epoch/Renderer/Offline/Core/Rtweeked.h"
#include "Epoch/Renderer/Offline/Core/HitManager.h"

namespace Epoch {

  class Offline
  {
  public:
	Offline()
	{
	}

	bool BeginScene();

	Color RayColor(const Ray& ray, const Hittable& world, int depth);

	char *m_RendererID;

	float Progress = 0;
  };

}