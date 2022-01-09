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

	Entity CreatEntity(const std::string& tagStr = std::string());

	void DestroyEntity(Entity entity);
	// Temp
	void OnUpdate(Timestep timestep);

	void SetShader(Ref<Shader>& shader) { m_shader = shader; }
	void SetSelectEntity(Entity entity);
  private:
	entt::registry m_Registry;

	friend class Entity;
	friend class SceneHierarchyPanel;

	std::shared_ptr<Shader> m_shader;

	Camera* mainCamera = nullptr;

	glm::mat4* CameraTransform = nullptr;

  };

}