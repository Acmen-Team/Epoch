#pragma once

namespace Epoch {

  class Input
  {
  public:
	static bool IsKeyPressed(int keycode);
	static bool IsMouseButtonPressed(int buttton);

	static std::pair<float, float> GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
  };

}