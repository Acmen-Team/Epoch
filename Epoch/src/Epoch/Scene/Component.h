#pragma once

#include <glm/glm.hpp>

#include "Epoch/Renderer/VertexArray.h"
#include "Epoch/Renderer/Camera.h"

#include "Epoch/Resource/Mesh.h"
#include "Epoch/Renderer/Shader.h"

#include "Epoch/Scene/ScriptableEntity.h"

namespace Epoch {

  struct MeshConponent
  {
	Ref<Mesh> _Mesh;

	MeshConponent() = default;
	MeshConponent(const MeshConponent&) = default;
	MeshConponent(const Ref<Mesh>& mesh)
	  : _Mesh(mesh) {}
	MeshConponent(const std::string& file_path, const std::string& base_path)
	  : _Mesh(Mesh::CreateMesh(file_path, base_path))
	{
	}

	//operator std::shared_ptr<VertexArray>& () { return m_VertexArray; }
	//operator const std::shared_ptr<VertexArray>& () const { return m_VertexArray; }
  };

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
	glm::mat4 Transform{ 1.0f };

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const glm::mat4& transform)
	  : Transform(transform) {}

	operator glm::mat4& () { return Transform; }
	operator const glm::mat4& () const { return Transform; }
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

 // struct MaterialComponent
 // {
	//Ref<Shader> m_Shader = CreateRef<Shader>();

	//MaterialComponent(const ShaderLibrary& shaderlibrary, const std::string& name)
	//{
	//  m_Shader.reset();
	//}

	//operator std::shared_ptr<Shader>& () { return m_Shader; }
 // };

}