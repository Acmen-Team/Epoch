#include "eppch.h"
#include "Material.h"

namespace Epoch {

  MaterialData* Material::Create()
  {
	return new MaterialData();
  }

}