#include "eppch.h"

#include "CameraController.h"

#include "Epoch/Events/ApplicationEvent.h"
#include "Epoch/Events/MouseEvent.h"

#include "Epoch/Core/Input.h"
#include "Epoch/Core/KeyCodes.h"
#include "Epoch/Core/MouseButtonCodes.h"
#include "Epoch/Core/Log.h"

#include <glm/ext/matrix_transform.hpp>

#include "Epoch/Scene/Entity.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Epoch {

  void CameraController::OnCreate()
  {
	
	//GetComponent<TransformComponent>();
  }

  void CameraController::OnUpdate(Timestep timestep)
  {
	//auto& transform = GetComponent<TransformComponent>().Transform;

	//float m_CamearTransformationSpeed = 1.0;

	//if (Input::IsKeyPressed(EP_KEY_W))
	//  transform[3][1] += m_CamearTransformationSpeed * timestep.GetSeconds();
	//if (Input::IsKeyPressed(EP_KEY_S))
	//  transform[3][1] -= m_CamearTransformationSpeed * timestep.GetSeconds();
  }

  void CameraController::OnDestroy()
  {
	
  }

}