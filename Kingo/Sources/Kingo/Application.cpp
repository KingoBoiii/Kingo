#include "kepch.h"
#include "Application.h"

#include "Kingo/Events/ApplicationEvent.h"
#include "Kingo/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Kingo {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		KE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Vertex Array
		m_VertexArray.reset(VertexArray::Create());
		// Vertex Buffer
		float vertices[3 * (3 + 4)] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[2 * 3] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Shader
		std::string vertexSrc = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

void main() {
	v_Color = a_Color;
	v_Position = a_Position;
	gl_Position = vec4(a_Position, 1.0);
}
		)";

		std::string fragmentSrc = R"(
#version 330 core
layout(location = 0) out vec4 Color;

in vec3 v_Position;
in vec4 v_Color;

void main() {
	Color = v_Color; //vec4(v_Position * 0.5 + 0.5, 1.0);
}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		// ---------------------------------------------------------------------------------------------------------------
		// Another Shader
		std::string blueVertexSrc = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

void main() {
	v_Position = a_Position;
	gl_Position = vec4(a_Position, 1.0);
}
		)";

		std::string BlueFragmentSrc = R"(
#version 330 core
layout(location = 0) out vec4 Color;

in vec3 v_Position;

void main() {
	// Color = vec4(v_Position * 0.5 + 0.5, 1.0);
	Color = vec4(0.2, 0.3, 0.8, 1.0);
}
		)";

		m_BlueShader.reset(new Shader(blueVertexSrc, BlueFragmentSrc));
	}

	Application::~Application() { }

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			Renderer::BeginScene();
			Renderer::Submit();
			Renderer::EndScene();

			m_BlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack) layer->OnUpdate(); 

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) layer->OnImGuiRender(); 
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

}