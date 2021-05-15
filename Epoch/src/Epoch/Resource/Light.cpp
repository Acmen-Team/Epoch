#include "eppch.h"
#include "Light.h"

namespace Epoch {

  LightData* Light::Create()
  {
	return new LightData();
  }

}