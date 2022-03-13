#include "eppch.h"

#include "Component.h"

#include "Epoch/Resource/ResourceManager.h"

namespace Epoch {


  MeshComponent::MeshComponent(const Ref<Mesh>& mesh)
  {
	_Id = ResourceManager::Get().GetAllocator()->GetResId(mesh);
	_Mesh = mesh;
  }

}