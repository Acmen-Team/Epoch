#pragma once
#include "Epoch/Core/Timestep.h"

#include "Epoch/Graphics/Renderer/Shader.h"
#include "Epoch/Graphics/Renderer/Camera.h"

#include "entt.hpp"

namespace Epoch {

  class Entity;

  class Scene
  {
  public:
	Scene();
	~Scene();

	template<typename EntityT>
	EntityT CreatEntity(const std::string& tagStr = std::string())
	{
	  EntityT entity = { m_Registry.create(), this };
	  auto& tag = entity.AddComponent<TagComponent>();
	  tag.Tag = tagStr.empty() ? "Entity" : tagStr;

	  entity.AddComponent<TransformComponent>();

	  return entity;
	}

	void DestroyEntity(Entity entity);
	// Temp
	void OnUpdate(Timestep timestep);

	void SetShader(Ref<Shader>& shader) { m_shader = shader; }
	void SetSelectEntity(Entity entity);
  private:
	entt::registry m_Registry;

	friend class Entity;
	friend class SceneHierarchyPanel;

	Epoch::ShaderLibrary* m_ShaderLibrary;
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<Shader> m_ColorShader;

	Camera* mainCamera = nullptr;

	glm::mat4* CameraTransform = nullptr;

  };

}