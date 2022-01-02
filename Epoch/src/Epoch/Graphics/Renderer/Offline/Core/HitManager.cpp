#include "eppch.h"

#include "HitManager.h"

namespace Epoch {

  HittableList::HittableList(std::shared_ptr<Hittable> object)
  {
	Add(object);
  }

  void HittableList::Clear()
  {
	m_Objects.clear();
  }

  void HittableList::Add(std::shared_ptr<Hittable> object)
  {
	m_Objects.push_back(object);
  }

  bool HittableList::Hit(const Ray& ray, float timeMin, float timeMax, HitRecord& rec) const
  {
	HitRecord tempRec;
	bool hitAnything = false;
	auto closest_so_far = timeMax;

	for (const auto& object : m_Objects)
	{
	  if (object->Hit(ray, timeMin, closest_so_far, tempRec))
	  {
		hitAnything = true;
		closest_so_far = tempRec.time;
		rec = tempRec;
	  }
	}

	return hitAnything;
  }

}