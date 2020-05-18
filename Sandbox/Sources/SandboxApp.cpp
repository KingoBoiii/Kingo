#include <Kingo.h>
#include <Kingo/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Kingo::Layer {
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f) {
		// Vertex Array
		m_VertexArray = Kingo::VertexArray::Create();
		// Vertex Buffer
		float vertices[3 * (3 + 4)] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Kingo::Ref<Kingo::VertexBuffer> vertexBuffer = Kingo::VertexBuffer::Create(vertices, sizeof(vertices));
		Kingo::BufferLayout layout = {
			{ Kingo::ShaderDataType::Float3, "a_Position" },
			{ Kingo::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		Kingo::Ref<Kingo::IndexBuffer> indexBuffer = Kingo::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Kingo::VertexArray::Create();

		float squareVertices[4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		Kingo::Ref<Kingo::VertexBuffer> squareVB = Kingo::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		squareVB->SetLayout({
			{ Kingo::ShaderDataType::Float3, "a_Position" },
			{ Kingo::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[2 * 3] = { 0, 1, 2, 2, 3, 0 };
		Kingo::Ref<Kingo::IndexBuffer> squareIB = Kingo::IndexBuffer::Create(squareIndices, 6);
		m_SquareVA->SetIndexBuffer(squareIB);

		// Shader
		std::string vertexSrc = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	v_Color = a_Color;
	v_Position = a_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_TriangleShader = Kingo::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		// ---------------------------------------------------------------------------------------------------------------
		// Flat Color Shader
		std::string flatColorShaderVertexSrc = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	v_Position = a_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
		)";

		std::string flatColorShaderFragmentSrc = R"(
#version 330 core
layout(location = 0) out vec4 Color;

in vec3 v_Position;

uniform vec3 u_Color;

void main() {
	//Color = vec4(0.2, 0.3, 0.8, 1.0);
	Color = vec4(u_Color, 1.0);
}
		)";

		m_FlatColorShader = Kingo::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");

		m_KingoLogoTexture = Kingo::Texture2D::Create("Assets/Textures/KingoLogo.png");

		m_Texture = Kingo::Texture2D::Create("Assets/Textures/Checkerboard.png");

		std::dynamic_pointer_cast<Kingo::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Kingo::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Kingo::Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		Kingo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Kingo::RenderCommand::Clear();

		Kingo::Renderer::BeginScene(m_CameraController.GetCamera());
		{
			static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

			std::dynamic_pointer_cast<Kingo::OpenGLShader>(m_FlatColorShader)->Bind();
			std::dynamic_pointer_cast<Kingo::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 20; x++) {
					glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
					Kingo::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
				}
			}

			auto textureShader = m_ShaderLibrary.Get("Texture");

			m_Texture->Bind();
			Kingo::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_KingoLogoTexture->Bind();
			Kingo::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			// Kingo::Renderer::Submit(m_TriangleShader, m_VertexArray);
		}
		Kingo::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Kingo::Event& e) override {
		m_CameraController.OnEvent(e);
	}
private:
	Kingo::ShaderLibrary m_ShaderLibrary;

	Kingo::Ref<Kingo::Shader> m_TriangleShader;
	Kingo::Ref<Kingo::VertexArray> m_VertexArray;

	Kingo::Ref<Kingo::Shader> m_FlatColorShader;
	Kingo::Ref<Kingo::VertexArray> m_SquareVA;

	Kingo::Ref<Kingo::Texture2D> m_Texture, m_KingoLogoTexture;

	Kingo::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Kingo::Application {
public:
	Sandbox() { 
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() { }
};

Kingo::Application* Kingo::CreateApplication() {
	return new Sandbox();
}

