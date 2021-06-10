#pragma once

#include "Epoch/Renderer/VertexArray.h"

#include "glm/glm.hpp"

namespace Epoch {

  float GetPro();

  struct Vertex
  {
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_Texcoord;

	Vertex(glm::vec3& pos, glm::vec3& normal, glm::vec2& texcoord)
	{
	  m_Position = pos;
	  m_Normal = normal;
	  m_Texcoord = texcoord;
	}
  };

  struct MeshData
  {
	std::vector<Vertex> vertices_list;
	std::vector<unsigned int> indices_list;
  };

  class Mesh
  {
  public:
	Mesh(const std::string& file_path, const std::string& base_path);
	Mesh(const std::string& file_path, const std::string& base_path, bool triangle);
	
	~Mesh() = default;

	static Ref<Mesh> CreateMesh(const std::string& file_path, const std::string& base_path) { return CreateRef<Mesh>(file_path, base_path); }

	static float GetReadPro() { return pro; }
	static float pro;

	std::shared_ptr<VertexArray>& GetVertexArray() { return m_VertexArray; }
	//static GetName() { return m_Name; }
  private:
	std::shared_ptr<MeshData> ObjLoad(const std::string& file_path, const std::string& base_path, bool triangle = true);
	void CreatVertexArray(std::shared_ptr<MeshData>& mehsData);
	// TODO
	//FbxLoad()
  private:
	std::string m_Name;

	std::shared_ptr<VertexArray> m_VertexArray;
  };

}
