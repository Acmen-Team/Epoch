#include "eppch.h"

#include "SceneHierarchyPanel.h"
#include "Epoch/Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

namespace Epoch {

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
  {
	SetContext(context);
  }

  void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
  {
	m_Context = context;
  }

  void SceneHierarchyPanel::OnImGuiRender()
  {
	ImGui::Begin("Scene Hierarchy");

	m_Context->m_Registry.each([&](auto entityID)
	{
	  Entity entity{ entityID, m_Context.get() };

	  DrawEntityNode(entity);
	});

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
	  m_SelectionContext = {};
	}

	ImGui::End();

	ImGui::Begin("Properties");
	if(m_SelectionContext)
	  DrawCommponents(m_SelectionContext);
	ImGui::End();
  }

  void SceneHierarchyPanel::DrawEntityNode(Entity entity)
  {
	auto& tag = entity.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
	  m_SelectionContext = entity;
	}

	if (opened)
	{
	  ImGui::TreePop();
	}
  }

  void SceneHierarchyPanel::DrawCommponents(Entity entity)
  {
	if (entity.HasComponent<TagComponent>())
	{
	  auto& tag = entity.GetComponent<TagComponent>().Tag;

	  char buffer[256];
	  memset(buffer, 0, sizeof(buffer));
	  strcpy_s(buffer, sizeof(buffer), tag.c_str());
	  if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
	  {
		tag = std::string(buffer);
	  }
	}

	if (entity.HasComponent<TransformComponent>())
	{
	  if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
	  {
		auto& transform = entity.GetComponent<TransformComponent>().Transform;
		ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

		ImGui::TreePop();
	  }
	}
  }

}