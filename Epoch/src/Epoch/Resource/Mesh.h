#pragma once

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
	static MeshData* CreateMesh(const std::string& file_path, const std::string& base_path, bool triangle);
	static float GetReadPro() { return pro; }
	static float pro;
  };

}
