#include "eppch.h"

#include "SceneCameraController.h"

#include "Epoch/Core/Input.h"
#include "Epoch/Core/KeyCodes.h"
#include "Epoch/Core/MouseButtonCodes.h"
#include "Epoch/Core/Log.h"

#include <glm/ext/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Epoch {

  SceneCameraController::SceneCameraController(float aspectRatio)
	: m_AspectRatio(aspectRatio), m_Camera()
  {
	UpdateCameraVectors();
  }

  void SceneCameraController::OnUpdate(Timestep timestep)
  {
	if (Epoch::Input::IsKeyPressed(EP_KEY_W))
	  m_CameraPosition += m_CamearTransformationSpeed * timestep.GetSeconds() * m_CameraFront;
	if (Epoch::Input::IsKeyPressed(EP_KEY_S))
	  m_CameraPosition -= m_CamearTransformationSpeed * timestep.GetSeconds() * m_CameraFront;
	if (Epoch::Input::IsKeyPressed(EP_KEY_A))
	  m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CamearTransformationSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_D))
	  m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CamearTransformationSpeed * timestep.GetSeconds();
	if (Epoch::Input::IsKeyPressed(EP_KEY_E))
	  m_CameraPosition += m_CamearTransformationSpeed * timestep.GetSeconds() * m_CameraUp;
	if (Epoch::Input::IsKeyPressed(EP_KEY_Q))
	  m_CameraPosition -= m_CamearTransformationSpeed * timestep.GetSeconds() * m_CameraUp;

	UpdateCameraVectors();
  }

  void SceneCameraController::OnEvent(Event& e)
  {
	if (m_Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
	{
	  EventDispatcher dispatcher(e);
	  dispatcher.Dispatch<MouseScrolledEvent>(EP_BIND_EVENT_FN(SceneCameraController::OnMouseScrolledEvent));
	  dispatcher.Dispatch<MouseMovedEvent>(EP_BIND_EVENT_FN(SceneCameraController::OnMouseMovedEvent));
	  dispatcher.Dispatch<MouseButtonPressedEvent>(EP_BIND_EVENT_FN(SceneCameraController::OnMouseButtonPressedEvent));
	  dispatcher.Dispatch<MouseButtonReleasedEvent>(EP_BIND_EVENT_FN(SceneCameraController::OnMouseButtonReleasedEvent));
	  dispatcher.Dispatch<WindowResizeEvent>(EP_BIND_EVENT_FN(SceneCameraController::OnWindowResizeEvent));
	}
  }

  void SceneCameraController::OnViewportResize(uint32_t width, uint32_t height)
  {
	if (!m_Camera.fixedAspectration)
	  m_Camera.OnViewportResize(width, height);
  }

  bool SceneCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
  {
	if (RightMouseButtonPressed)
	{
	  if (firstMouse)
	  {
		lastX = e.GetX();
		lastY = e.GetY();


		firstMouse = false;
	  }

	  float xoffset = e.GetX() - lastX;
	  float yoffset = lastY - e.GetY();

	  lastX = e.GetX();
	  lastY = e.GetY();

	  ProcessMouseMovement(xoffset, yoffset);
	}

	return false;
  }

  bool SceneCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
  {
	if (RightMouseButtonPressed)
	{
	  //if (m_Fov >= 1.0f && m_Fov <= 45.0f)
	  //	m_Fov -= e.GetYOffset();
	  //if (m_Fov <= 1.0f)
	  //	m_Fov = 1.0f;
	  //if (m_Fov >= 45.0f)
	  //	m_Fov = 45.0f;
	  //m_Camera.SetProjectionMatrix(m_Fov, m_AspectRatio, m_NearPlan, m_FarPlan);
	}
	else
	{
	  m_CameraPosition += (0.1f * e.GetYOffset()) * m_CameraFront;
	  UpdateCameraVectors();
	}

	return false;
  }

  bool SceneCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
  {
	if (e.GetMouseButton() == EP_MOUSE_BUTTON_RIGHT)
	  RightMouseButtonPressed = true;

	return false;
  }

  bool SceneCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
  {
	RightMouseButtonPressed = false;
	firstMouse = true;
	return false;
  }

  bool SceneCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
  {
	//m_AspectRatio = 1.6 / 0.9;
	//m_AspectRatio = e.GetWidth() / e.GetHeight();
	//if (m_Camera.fixedAspectration)
	//{
	//
	//}
	//m_Camera.SetProjectionMatrix(45.0f, m_AspectRatio, 0.1f, 100.0f);

	return false;
  }

  void SceneCameraController::ProcessMouseMovement(float xOffset, float yOffset)
  {
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset;

	if (m_Pitch > 89.0f)
	  m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
	  m_Pitch = -89.0f;

	UpdateCameraVectors();
  }


  void SceneCameraController::UpdateCameraVectors()
  {
	m_CameraFront.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront.y = sin(glm::radians(m_Pitch));
	m_CameraFront.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront = glm::normalize(m_CameraFront);

	glm::vec3 right = glm::normalize(glm::cross(m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	m_CameraUp = glm::normalize(glm::cross(right, m_CameraFront));

	m_Camera.SetViewMatrix(glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp));
  }

}