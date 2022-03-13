#pragma once

#include "Epoch.h"

#include <map>
#include <memory>
#include <string>

namespace Epoch {

template<typename ResourceType>
class ResourceAllocator
{
public:
  ResourceAllocator() = default;
  ~ResourceAllocator() = default;

  int AddRes(const std::string& filePath, const std::string& basePath)
  {
	auto it = m_Resources.find(filePath);
	if (it != m_Resources.end())
	{
	  return it->second.first;
	}

	std::shared_ptr<ResourceType> resource = std::make_shared<ResourceType>();

	if (!resource->LoadResFromeFile(filePath, basePath))
	{
	  return -1;
	}

	m_Resources.insert(std::make_pair(filePath, std::make_pair(m_CurrentID, resource)));
	
	return m_CurrentID++;
  }

  void RemoveRes(int id)
  {
	for (auto it = m_Resources.begin(); it != m_Resources.end(); ++it)
	{
	  if (id == it->second.first)
		m_Resources.erase(it->first);
	}
  }

  int GetResId(const std::shared_ptr<ResourceType> mesh)
  {
	for (auto res : m_Resources)
	{
	  if (mesh == res.second.second)
		return res.second.first;
	}
	
	return -1;
  }

  std::shared_ptr<ResourceType> GetRes(int id)
  {
	for (auto it = m_Resources.begin(); it != m_Resources.end(); ++it)
	{
	  if (id == it->second.first)
	  {
		return it->second.second;
	  }

	}
	return nullptr;
  }

  std::shared_ptr<ResourceType> GetRes(const std::string& filePath)
  {
	auto it = m_Resources.find(filePath);
	if (it != m_Resources.end())
	  return it->second.second;
	else
	  return nullptr;
  }

  bool HasRes(int id)
  {
	return (GetRes(id) != nullptr);
  }

  int GetResSize() { return m_CurrentID; }

private:
  // Resource id
  int m_CurrentID = 0;
  // Resources
  std::map<std::string, std::pair<int, std::shared_ptr<ResourceType>>> m_Resources;
};

class ResourceManager
{
public:
  static ResourceManager& Get() { return m_Instance; }

  std::shared_ptr<ResourceAllocator<Mesh>> GetAllocator() { return m_MeshRes; }

protected:
  ResourceManager() { m_MeshRes = std::make_shared<ResourceAllocator<Mesh>>(); }
  ~ResourceManager() {}
private:
  static ResourceManager m_Instance;

  //ResourceAllocator<Texture> m_TextureRes;
  std::shared_ptr<ResourceAllocator<Mesh>> m_MeshRes;
};

}