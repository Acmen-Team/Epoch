#include "eppch.h"
#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Epoch {

  float GetPro()
  {
	return tinyobj::LoadPro;
  }

  float Mesh::pro = 0.0f;

  Mesh::Mesh(const std::string& file_path, const std::string& base_path)
  {
	CreatVertexArray(ObjLoad(file_path, base_path));

	// can be greate?
	auto lastSlash = file_path.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = file_path.rfind('.');
	auto count = lastDot == std::string::npos ? file_path.size() - lastSlash : lastDot - lastSlash;
	m_Name = file_path.substr(lastSlash, count);
  }

  Mesh::Mesh(const std::string& file_path, const std::string& base_path, bool triangle)
  {
	CreatVertexArray(ObjLoad(file_path, base_path, triangle));

	// can be greate?
	auto lastSlash = file_path.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = file_path.rfind('.');
	auto count = lastDot == std::string::npos ? file_path.size() - lastSlash : lastDot - lastSlash;
	m_Name = file_path.substr(lastSlash, count);
  }

  std::shared_ptr<MeshData> Mesh::ObjLoad(const std::string& file_path, const std::string& base_path, bool triangle)
  {
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = base_path; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(file_path, reader_config)) {
	  if (!reader.Error().empty()) {
		std::cerr << "TinyObjReader: " << reader.Error();
	  }
	  exit(1);
	}

	if (!reader.Warning().empty()) {
	  std::cout << "TinyObjReader: " << reader.Warning();
	}

	std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>();
	glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
	glm::vec2 texCoord = { 0.0f, 0.0f };

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
	  // Loop over faces(polygon)
	  size_t index_offset = 0;
	  for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {

		pro = (float)f / (float)shapes[s].mesh.num_face_vertices.size();

		//EP_CORE_TRACE("PRO : {0}", pro);

		size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

		// Loop over vertices in the face.
		for (size_t v = 0; v < fv; v++) {
		  // access to vertex
		  tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
		  pos.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
		  pos.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
		  pos.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

		  // Check if `normal_index` is zero or positive. negative = no normal data
		  if (idx.normal_index >= 0) {
			normal.x = attrib.normals[3 * size_t(idx.normal_index) + 0];
			normal.y = attrib.normals[3 * size_t(idx.normal_index) + 1];
			normal.z = attrib.normals[3 * size_t(idx.normal_index) + 2];
		  }

		  // Check if `texcoord_index` is zero or positive. negative = no texcoord data
		  if (idx.texcoord_index >= 0) {
			texCoord.x = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
			texCoord.y = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
		  }

		  // Optional: vertex colors
		  // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
		  // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
		  // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

		  Vertex vertices(pos, normal, texCoord);
		  meshData->vertices_list.push_back(vertices);
		  meshData->indices_list.push_back(index_offset + v);
		}
		index_offset += fv;

		// per-face material
		shapes[s].mesh.material_ids[f];
	  }

	}
	pro = 0.0f;

	return meshData;
  }

  void Mesh::CreatVertexArray(std::shared_ptr<MeshData>& mehsData)
  {
	m_VertexArray.reset(VertexArray::Create());

	std::shared_ptr<Epoch::VertexBuffer> m_VertexBuffer;

	m_VertexBuffer.reset(Epoch::VertexBuffer::Create((float*)&mehsData->vertices_list[0], sizeof(float) * 8 * mehsData->vertices_list.size()));

	BufferLayout MeshLayout = {
	  { ShaderDataType::Float3, "a_Pos" },
	  { ShaderDataType::Float3, "a_Normal" },
	  { ShaderDataType::Float2, "a_TexCoord" }
	};

	m_VertexBuffer->SetLayout(MeshLayout);
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	std::shared_ptr<Epoch::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Epoch::IndexBuffer::Create((uint32_t*)&mehsData->indices_list[0], mehsData->indices_list.size()));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
  }

}