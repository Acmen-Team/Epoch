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

  struct ShapeData
  {
	ShapeData() = default;

	std::string name;
	std::vector<unsigned int> indices_list;
	std::vector<Vertex> vertices_list;
  };

  struct MeshData
  {
	std::vector<ShapeData*> shapes;
  };

  class Mesh
  {
  public:
	Mesh() = default;
	~Mesh() = default;
	
	static float pro;
	static float GetReadPro() { return pro; }

	std::string GetMeshName() const { return m_Name; }
	std::map<std::string, std::shared_ptr<VertexArray>>& GetMesh();
	std::shared_ptr<VertexArray>& GetVertexArray(std::string& name);

	std::shared_ptr<MeshData> LoadResFromeFile(const std::string& filePath, const std::string& basePath);

	//static GetName() { return m_Name; }
  private:
	std::shared_ptr<MeshData> ObjLoad(const std::string& file_path, const std::string& base_path, bool triangle = true);
	void CreatVertexArrayList(std::shared_ptr<MeshData>& meshData);
	// TODO
	//FbxLoad()
  private:
	std::string m_Name = "M";
	std::shared_ptr<MeshData> m_MeshData;
	std::map<std::string, std::shared_ptr<VertexArray>> m_VertexArrayList;
  };

}