#pragma once

#include <glm/glm.hpp>

#include "Epoch/Renderer/VertexArray.h"
#include "Epoch/Renderer/Camera.h"

#include "Epoch/Resource/Mesh.h"
#include "Epoch/Renderer/Shader.h"

#include "Epoch/Scene/ScriptableEntity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Epoch {

  struct TagComponent
  {
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
	  : Tag(tag) {}

  };

  struct TransformComponent
  {
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
	  :Translation(translation), Rotation(rotation), Scale(scale)
	{
	}

	glm::mat4 GetTransform() const
	{
	  // calculate transform matrix
	  glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

	  glm::mat4 transformation = glm::translate(glm::mat4(1.0f), Translation)
		* rotation
		* glm::scale(glm::mat4(1.0f), Scale);

	  return transformation;
	}
  };

  struct MeshComponent
  {
	uint32_t _Id;
	Ref<Mesh> _Mesh;

	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
	MeshComponent(const Ref<Mesh>& mesh)
	  : _Mesh(mesh), _Id(0) {}

	//operator std::shared_ptr<VertexArray>& () { return m_VertexArray; }
	//operator const std::shared_ptr<VertexArray>& () const { return m_VertexArray; }
  };

  struct MaterialComponent
  {
	uint32_t _Id;
	Ref<Shader> _Material;

	MaterialComponent() = default;
	MaterialComponent(const MaterialComponent&) = default;
	MaterialComponent(const Ref<Shader>& material)
	  : _Material(material), _Id(0) {}
	//Ref<Shader> m_Shader = CreateRef<Shader>();

	//MaterialComponent(const ShaderLibrary& shaderlibrary, const std::string& name)
	//{
	//  m_Shader.reset();
	//}

	//operator std::shared_ptr<Shader>& () { return m_Shader; }
  };

  struct CameraComponent
  {
	Camera _Camera;
	bool _Perspective = true;

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent(const glm::mat4& projection)
	  : _Camera(projection) {}
  };

  struct NativeScriptComponent
  {
	ScriptableEntity* Instance = nullptr;

	// Using function ptr replace std::function
	ScriptableEntity*(*InstantiateScript)();
	void(*DestroyScript)(NativeScriptComponent*);

	template<typename T>
	void Bind()
	{
	  InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
	  DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
	}

  };

}