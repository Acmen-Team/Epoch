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

	// Temp
	void OnUpdate(Timestep timestep);

  private:
	entt::registry m_Registry;

	friend class Entity;
	friend class SceneHierarchyPanel;

	std::shared_ptr<Shader> m_shader;

	Camera* mainCamera = nullptr;

	glm::mat4* CameraTransform = nullptr;
  };

}