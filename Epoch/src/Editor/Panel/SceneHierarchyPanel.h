#pragma once

#include "Epoch/Core/Core.h"
#include "Epoch/Scene/Scene.h"
#include "Epoch/Scene/Entity.h"

namespace Epoch {

  class SceneHierarchyPanel
  {
  public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Ref<Scene>& context);

	void SetContext(const Ref<Scene>& context);

	void OnImGuiRender();
  protected:
	void DrawEntityNode(Entity& entity);
  private:
	Ref<Scene> m_Context;
	Entity m_SelectionContext;
  };

}