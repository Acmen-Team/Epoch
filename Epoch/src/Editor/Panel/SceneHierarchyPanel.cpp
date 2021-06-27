#include "eppch.h"

#include "SceneHierarchyPanel.h"
#include "Epoch/Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_internal.h>

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

  static void DrawVec3Control(const std::string& label, glm::vec3& value, float resetValue = 0.0f, float columnWidth = 100.0f)
  {
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	if (ImGui::Button("X", buttonSize))
	  value.x = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	if (ImGui::Button("Y", buttonSize))
	  value.y = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	if (ImGui::Button("Z", buttonSize))
	  value.z = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	ImGui::Columns(1);

	ImGui::PopID();
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
		auto& Translation = entity.GetComponent<TransformComponent>().Translation;
		auto& Rotation = entity.GetComponent<TransformComponent>().Rotation;
		auto& Scale = entity.GetComponent<TransformComponent>().Scale;

		DrawVec3Control("Position", Translation);
		glm::vec3 rotation = glm::degrees(Rotation);
		DrawVec3Control("Rotation", rotation);
		Rotation = glm::radians(rotation);
		DrawVec3Control("Scale", Scale, 1.0f);

		ImGui::TreePop();
	  }
	}
  }

}