#pragma once

#include "Hittable.h"

namespace Epoch {

  class HittableList : public Hittable
  {
  public:
	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object);

	void Clear();
	void Add(std::shared_ptr<Hittable> object);

	virtual bool Hit(const Ray& ray, float timeMin, float timeMax, HitRecord& rec) const override;
  public:
	std::vector<Ref<Hittable>> m_Objects;
  };

}