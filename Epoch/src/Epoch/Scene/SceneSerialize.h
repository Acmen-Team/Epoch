/*************************************************
Copyright © 2020 - 2022 YX.All Rights Reserved

Date:2022-3-28 22:02
Description:Serialization
**************************************************/
#pragma once

#include "Epoch/Scene/Scene.h"

// TODO : SiYu

namespace Epoch {

  class SceneSerialize
  {
  public:
	SceneSerialize(const Scene& scene);

	/*
	* Serialize a scene.
	*/
	bool Serialize(const std::string& filePath);
	bool DeSerialize(const std::string& filePath);

	/*
	* To sequence at runtime, may be used to preserve the dynamic content of a genetically continuous scene.
	* If the timestep is equal to 16ms, ideally the number of scenes we save should be 60.
	*/
	bool SerializeRuntime(const std::string& filePath, unsigned int timeStep);
	bool DeSerializeRuntime(const std::string& filePath);
  private:
	// Do we need these?
	
	/*
	* Serialized into binary files for easier storage or transmission.
	*/
	// T SerializeBinary(...);
	
	/*
	* EM (Tentative:EpochMap) is a map file in engine format. 
	* Reserved for possible future existence and intended to be shared among other engines. 
	* For example: pass a map in Epoch and then import it into Dark for use.
	*/
	// T SerializeEM(...);
  private:
	const Scene& m_Scene;
  };

}