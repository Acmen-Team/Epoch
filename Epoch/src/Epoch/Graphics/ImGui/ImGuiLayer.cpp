#include "eppch.h"
#include "ImGuiLayer.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Epoch/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Epoch {

  ImGuiLayer::ImGuiLayer() :Layer("ImGuiLayer")
  {
  }

  ImGuiLayer::~ImGuiLayer()
  {
  }

  void ImGuiLayer::OnAttach()
  {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Imgui Fonts
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-Regular.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-Bold.ttf", 18.0f);

	//io.Fonts->AddFontFromFileTTF("assets/fonts/Noto_Serif_SC/chinese.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
	  style.WindowRounding = 0.0f;
	  style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetDarkThemeColors();

	Application& app = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui_ImplOpenGL3_Init("#version 410");
  }

  void ImGuiLayer::OnDetach()
  {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
  }

  void ImGuiLayer::OnEvent(Event& event)
  {
	ImGuiIO& io = ImGui::GetIO();

	//event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	//event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
  }

  void ImGuiLayer::Begin()
  {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
  }

  void ImGuiLayer::End()
  {
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
	  GLFWwindow* backup_current_context = glfwGetCurrentContext();
	  ImGui::UpdatePlatformWindows();
	  ImGui::RenderPlatformWindowsDefault();
	  glfwMakeContextCurrent(backup_current_context);
	}
  }

  void ImGuiLayer::SetDarkThemeColors()
  {
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
  }

  void ImGuiLayer::OnImGuiRender()
  {
	ImGui::Begin("Renderer Info");

	ImGui::Text("  Vendor: %s", glGetString(GL_VENDOR));
	ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
	ImGui::Text(" Version: %s", glGetString(GL_VERSION));

	int monitorCount;
	GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);

	const GLFWvidmode * mode = glfwGetVideoMode(pMonitor[0]);

	ImGui::Text("Screen resolution: Width(%d), Height(%d) , RGB(%d, %d, %d)", mode->width, mode->height, mode->redBits, mode->greenBits, mode->blueBits);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
	//static bool show_demo_window = false;
	//static bool show_another_window = true;
	//ImGui::ShowDemoWindow(&show_demo_window);
	//ImGui::ShowMetricsWindow(&show_demo_window);

	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//ImGui::Checkbox("Another Window", &show_another_window);
	//
	//float color[3] = { 0.3f, 0.2f, 0.5f };
	//
	//float f = 0.4f;
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//ImGui::ColorEdit3("clear color", (float*)color); // Edit 3 floats representing a color
	//
	//unsigned int counter = 0;
	//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return //true when edited/activated)
	//  counter++;
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", counter);
	//
	//ImGui::End();
  }

}