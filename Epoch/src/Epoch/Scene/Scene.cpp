#include "eppch.h"

#include "Scene.h"
#include "Component.h"

#include "Entity.h"

#include "Epoch/Graphics/Renderer/Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"

namespace Epoch {

  Entity m_SelectEntity;

  Scene::Scene()
  {
	m_ShaderLibrary = ShaderLibrary::Get();

	// Load shader resource
	m_ColorShader = m_ShaderLibrary->GetShader("ColorShader");
	//m_ColorShader = Shader::Create("assets/shaders/Color.glsl");
  }

  Scene::~Scene()
  {

  }

  void Scene::DestroyEntity(Entity entity)
  {
	m_Registry.destroy(entity);
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

	{
	  auto view = m_Registry.view<TransformComponent, MeshComponent, LightPropertyComponent>();

	  int LightNums{0};

	  for (auto entity : view)
	  {
		auto [trans, mesh, lightProperty] = view.get<TransformComponent, MeshComponent, LightPropertyComponent>(entity);
		std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat3("lights[" + std::to_string(LightNums) + "].position", trans.Translation);
		std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat3("lights[" + std::to_string(LightNums) + "].direction", trans.Rotation);
		std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat4("lights[" + std::to_string(LightNums) + "].color", lightProperty._Property->Color);
		std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat("lights[" + std::to_string(LightNums) + "].range", lightProperty._Property->Range);
		std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat("lights[" + std::to_string(LightNums) + "].spotAngle", lightProperty._Property->SpotAngle);

		std::dynamic_pointer_cast<Shader>(m_shader)->UploadUniformFloat("lights[" + std::to_string(LightNums) + "].type", lightProperty._Property->LightType);

		LightNums++;
	  }
	}

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

	  auto group = m_Registry.group<TagComponent>(entt::get<MeshComponent, TransformComponent, MaterialComponent>);

	  for (auto entity : group) {
		// a component at a time ...
		//auto &position = group.get<position>(entity);
		//auto &velocity = group.get<velocity>(entity);

		// ... multiple components ...
		//auto[pos, vel] = group.get<position, velocity>(entity);

		// ... all components at once

		if (entity == m_SelectEntity.GetHandle())
		  continue;

		auto[tag, mesh, trans] = group.get<TagComponent, MeshComponent, TransformComponent>(entity);

		for (auto shap : mesh._Mesh->GetMesh())
		{
		  Renderer::Submit(m_shader, shap.second, trans.GetTransform());
		}
		// ...
	  }

	  if (!m_SelectEntity.IsNullEntity() && m_SelectEntity.HasComponent<MeshComponent>())
	  {
		Renderer::SetRenderState();

		auto mesh = m_SelectEntity.GetComponent<MeshComponent>();
		auto trans = m_SelectEntity.GetComponent<TransformComponent>();

		for (auto shap : mesh._Mesh->GetMesh())
		{
		  Renderer::Submit(m_shader, shap.second, trans.GetTransform());
		}
	  }

	  {
		auto view = m_Registry.view<TransformComponent, MeshComponent, LightPropertyComponent>();
		int LightNums{ 0 };
		for (auto entity : view)
		{
		  auto [trans, mesh, lightProperty] = view.get<TransformComponent, MeshComponent, LightPropertyComponent>(entity);
		  std::dynamic_pointer_cast<Shader>(m_ColorShader)->use();
		  std::dynamic_pointer_cast<Shader>(m_ColorShader)->UploadUniformFloat4("u_Color", lightProperty._Property->Color);

		  for (auto shap : mesh._Mesh->GetMesh())
		  {
			Renderer::Submit(m_ColorShader, shap.second, trans.GetTransform());
		  }

		  LightNums++;
		}
	  }
	  //Renderer::EndScene();

	}

  }

  void Scene::SetSelectEntity(Entity entity)
  {
	m_SelectEntity = entity;
  }

}