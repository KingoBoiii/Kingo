#pragma once
#include "Core.h"

#include "Window.h"
#include "Kingo/Core/LayerStack.h"
#include "Kingo/Events/Event.h"
#include "Kingo/Events/ApplicationEvent.h"

#include "Kingo/Core/Timestep.h"

#include "Kingo/ImGui/ImGuiLayer.h"

namespace Kingo {

	class Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in clients
	Application* CreateApplication();

}