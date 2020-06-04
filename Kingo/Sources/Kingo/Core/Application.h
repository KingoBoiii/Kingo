#pragma once
#include "Base.h"

#include "Window.h"
#include "Kingo/Core/LayerStack.h"
#include "Kingo/Events/Event.h"
#include "Kingo/Events/ApplicationEvent.h"

#include "Kingo/Core/Timestep.h"

#include "Kingo/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Kingo {

	class Application {
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void Close();

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		void Run();
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
		friend int ::main(int argc, char** argv);
	};

	// To be defined in clients
	Application* CreateApplication();

}
