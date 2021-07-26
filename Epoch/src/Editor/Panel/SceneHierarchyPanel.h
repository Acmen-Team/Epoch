#pragma once

#include "Epoch/Core/Core.h"
#include "Epoch/Scene/Scene.h"
#include "Epoch/Scene/Entity.h"

namespace Epoch {

  static int m_CurrentMesh;

  class SceneHierarchyPanel
  {
  public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const Ref<Scene>& context);

	void SetContext(const Ref<Scene>& context);

	void OnImGuiRender();

	Entity GetSelectEntity() { return m_SelectionContext; }
  protected:
	void DrawEntityNode(Entity entity);
	void DrawCommponents(Entity entity);
  private:
	Ref<Scene> m_Context;
	Entity m_SelectionContext;
  };

}