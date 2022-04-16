#include "eppch.h"
#include "LightEntity.h"

namespace Epoch {

  LightEntity::~LightEntity()
  {
	delete m_LightData;
	m_LightData = nullptr;
  }

}