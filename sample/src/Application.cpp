#include "Application.h"
#include <functional>
#include <iostream>

namespace FluidEngine
{
	void error_call_back(int error, const char *description)
	{
		Log::GetCoreLogger()->error("Error: {}", description);
	}

	int Application::Init(int majorVer, int minorVer)
	{
		glfwSetErrorCallback(error_call_back);
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVer);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVer);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		WindowProps props(800, 600, "Cumrun");
		m_Window = std::make_unique<Window>(props);
		//std bind --> bind a function or a member to a another function
		//in this line we binded applicaion::onevent function, the first aurgument indicates the concept of the function, second argument indicates which function exactly we want to use
		//the third argument indicates we want to pass a variables or some predefined value.
		m_Window->SetEventCallback(std::bind(&Window::OnEvent, m_Window.get(), std::placeholders::_1));
		glfwMakeContextCurrent(m_Window->GetWindow());

		//FPS cap
		glfwSwapInterval(0);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			FluidEngine::Log::GetCoreLogger()->error("Glad was not loaded correctly");
			return -1;
		}

		return 1;
	}

	void Application::MainLoop()
	{
		{
			std::vector<float> vertices = {
				// positions                 // texture coords
				 0.5f,  0.5f, 0.0f, 1.0f,     1.0f, 1.0f,   // top right
				 0.5f, -0.5f, 0.0f, 1.0f,     1.0f, 0.0f,   // bottom right
				-0.5f, -0.5f, 0.0f, 1.0f,     0.0f, 0.0f,   // bottom left
				-0.5f,  0.5f, 0.0f, 1.0f,     0.0f, 1.0f    // top left 
			};

			std::vector<unsigned int> indices = {
				0, 1, 3,
				1, 2, 3};

			ImGuiPanel panel;
			panel.InitiateImgui(m_Window->GetWindow());

			m_Renderer = std::make_unique<Renderer>(vertices, indices);
			//m_Renderer->SetColor("ColorBuffers", std::vector<float>{1, 0, 1, 1});
			m_Renderer->SetTexture("res/image/tex2.png", "SPIRV_Cross_CombinedshaderTexturesampleType", 0);
			while (!glfwWindowShouldClose(m_Window->GetWindow()))
			{
				m_Renderer->Tick();
				glfwPollEvents();
				m_Renderer->CalculateFrameStats();
				panel.RenderImguiFrame(m_Window->GetWindow());
				glfwSwapBuffers(m_Window->GetWindow());
				m_Renderer->Clear();
				m_Renderer->Draw(panel);
			}
			panel.TerminateImgui();
		}
		Terminate();
	}

	void Application::Terminate()
	{
	    m_Renderer.reset();
		m_Window->Terminate();
		glfwTerminate();
	}
} // namespace FluidEngine
