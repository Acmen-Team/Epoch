#pragma once

#include "Epoch/Core/Layer.h"

#include "Epoch/Events/ApplicationEvent.h"
#include "Epoch/Events/KeyEvent.h"
#include "Epoch/Events/MouseEvent.h"

namespace Epoch {

  class ImGuiLayer : public Layer
  {
  public:
	ImGuiLayer();
	~ImGuiLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;

	void Begin();
	void End();
  private:
	float m_Time = 0.0f;
  };

}