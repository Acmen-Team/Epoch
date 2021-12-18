#pragma once

#include "Epoch/Core/Timestep.h"

#include "Epoch/Renderer/Shader.h"
#include "Epoch/Renderer/Camera.h"

#include "entt.hpp"

namespace Epoch {

  class Entity;

  class Scene
  {
  public:
	Scene();
	~Scene();

	Entity CreatEntity(const std::string& tagStr = std::string());

	void DestroyEntity(Entity entity);
	// Temp
	void OnUpdate(Timestep timestep);

	void SetShader(Ref<Shader>& shader) { m_shader = shader; }
  private:
	entt::registry m_Registry;

	friend class Entity;
	friend class SceneHierarchyPanel;

	std::shared_ptr<Shader> m_shader;

	Camera* mainCamera = nullptr;

	glm::mat4* CameraTransform = nullptr;

  };

}