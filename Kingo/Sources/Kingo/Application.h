#pragma once
#include "Core.h"

#include "Window.h"
#include "Kingo/LayerStack.h"
#include "Kingo/Events/Event.h"
#include "Kingo/Events/ApplicationEvent.h"

#include "Kingo/ImGui/ImGuiLayer.h"

#include "Kingo/Renderer/Shader.h"
#include "Kingo/Renderer/Buffer.h"
#include "Kingo/Renderer/VertexArray.h"

namespace Kingo {

	class KINGO_API Application {
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
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_BlueShader;
	private:
		static Application* s_Instance;
	};

	// To be defined in clients
	Application* CreateApplication();

}
