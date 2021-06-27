#include "eppch.h"

#include "Scene.h"
#include "Component.h"

#include "Entity.h"

#include "Epoch/Renderer/Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Epoch {

  Scene::Scene()
  {
	m_shader = Shader::Create("assets/shaders/Color.glsl");
  }

  Scene::~Scene()
  {

  }

  Entity Scene::CreatEntity(const std::string& tagStr)
  {
	Entity entity = { m_Registry.create(), this };
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = tagStr.empty() ? "Entity" : tagStr;

	return entity;
  }

  void Scene::OnUpdate(Timestep timestep)
  {
	// Update script
	{
	  m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
	  {
		// TODO : Move to ::ScenePlaye
		if (!nsc.Instance)
		{
		  nsc.Instance = nsc.InstantiateScript();

		  nsc.Instance->m_Entity = Entity{ entity, this };
		  nsc.Instance->OnCreate();

		}

		nsc.Instance->OnUpdate(timestep);
	  });
	}


	// Render
	std::dynamic_pointer_cast<Shader>(m_shader)->use();
	std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat3("LightColor", glm::vec3(1.0f, 0.5f, 0.3f));


	{
	  auto view = m_Registry.view<TagComponent, CameraComponent, TransformComponent>();
	  for (auto entity : view)
	  {
		auto[tag, camera, transform] = view.get<TagComponent, CameraComponent, TransformComponent>(entity);
		if (camera._Perspective)
		{
		  mainCamera = &camera._Camera;

		  // TODO: recalculate camera entity transform
		  //CameraTransform = &transform.Transform;
		}
	  }
	}

	// TODO: review mainCamera entity in scene 
	if (!mainCamera)
	{
	  //Renderer::BeginScene(*mainCamera, *CameraTransform);


	  auto group = m_Registry.group<TagComponent>(entt::get<MeshConponent, TransformComponent>);

	  for (auto entity : group) {
		// a component at a time ...
		//auto &position = group.get<position>(entity);
		//auto &velocity = group.get<velocity>(entity);

		// ... multiple components ...
		//auto[pos, vel] = group.get<position, velocity>(entity);

		// ... all components at once
		auto[tag, mesh, trans] = group.get(entity);

		// calculate transform matrix
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), trans.Rotation.x, { 1, 0, 0 })
		  * glm::rotate(glm::mat4(1.0f), trans.Rotation.y, { 0, 1, 0 })
		  * glm::rotate(glm::mat4(1.0f), trans.Rotation.z, { 0, 0, 1 });

		glm::mat4 transformation = glm::translate(glm::mat4(1.0f), trans.Translation) 
		  * rotation
		  * glm::scale(glm::mat4(1.0f), trans.Scale);

		Renderer::Submit(m_shader, mesh._Mesh->GetVertexArray(), transformation);
		// ...
	  }

	  //Renderer::EndScene();

	}

  }

}