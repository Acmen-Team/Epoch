#include "eppch.h"

#include "Epoch.h"
#include "Editor/EditorLayer.h"

namespace Epoch {

  class EpochEditor :public Application
  {
  public:
	EpochEditor() :Application("Epoch Editor")
	{
	  PushLayer(new EditorLayer());
	}
	~EpochEditor()
	{
	}
  };

  Application* CreateApplication()
  {
	return new EpochEditor();
  }

}

