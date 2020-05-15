#include <Kingo.h>
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Kingo::Layer {
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

		// Vertex Array
		m_VertexArray.reset(Kingo::VertexArray::Create());
		// Vertex Buffer
		float vertices[3 * (3 + 4)] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Kingo::Ref<Kingo::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Kingo::VertexBuffer::Create(vertices, sizeof(vertices)));
		Kingo::BufferLayout layout = {
			{ Kingo::ShaderDataType::Float3, "a_Position" },
			{ Kingo::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Index Buffer
		uint32_t indices[3] = { 0, 1, 2 };
		Kingo::Ref<Kingo::IndexBuffer> indexBuffer;
		indexBuffer.reset(Kingo::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Kingo::VertexArray::Create());

		float squareVertices[4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		Kingo::Ref<Kingo::VertexBuffer> squareVB;
		squareVB.reset(Kingo::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Kingo::ShaderDataType::Float3, "a_Position" },
			{ Kingo::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[2 * 3] = { 0, 1, 2, 2, 3, 0 };
		Kingo::Ref<Kingo::IndexBuffer> squareIB;
		squareIB.reset(Kingo::IndexBuffer::Create(squareIndices, 6));
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

		m_Shader.reset(Kingo::Shader::Create(vertexSrc, fragmentSrc));

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
	// Color = vec4(v_Position * 0.5 + 0.5, 1.0);
	//Color = vec4(0.2, 0.3, 0.8, 1.0);
	Color = vec4(u_Color, 1.0);
}
		)";

		m_FlatColorShader.reset(Kingo::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		m_TextureShader.reset(Kingo::Shader::Create("Assets/Shaders/Texture.glsl"));

		m_KingoLogoTexture = Kingo::Texture2D::Create("Assets/Textures/KingoLogo.png");

		m_Texture = Kingo::Texture2D::Create("Assets/Textures/Checkerboard.png");

		std::dynamic_pointer_cast<Kingo::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Kingo::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Kingo::Timestep ts) override {
		if (Kingo::Input::IsKeyPressed(KE_KEY_LEFT)) {
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (Kingo::Input::IsKeyPressed(KE_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}

		if (Kingo::Input::IsKeyPressed(KE_KEY_UP)) {
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		else if (Kingo::Input::IsKeyPressed(KE_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}

		if (Kingo::Input::IsKeyPressed(KE_KEY_A)) {
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}
		else if (Kingo::Input::IsKeyPressed(KE_KEY_D)) {
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		Kingo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Kingo::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Kingo::Renderer::BeginScene(m_Camera);
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

			m_Texture->Bind();
			Kingo::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			m_KingoLogoTexture->Bind();
			Kingo::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

			// Triangle
			// Kingo::Renderer::Submit(m_Shader, m_VertexArray);
		}
		Kingo::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Kingo::Event& e) override {
	}
private:
	Kingo::Ref<Kingo::Shader> m_Shader;
	Kingo::Ref<Kingo::VertexArray> m_VertexArray;

	Kingo::Ref<Kingo::Shader> m_FlatColorShader, m_TextureShader;
	Kingo::Ref<Kingo::VertexArray> m_SquareVA;

	Kingo::Ref<Kingo::Texture2D> m_Texture, m_KingoLogoTexture;

	Kingo::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotationSpeed = 180.0f;
	float m_CameraRotation = 0.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Kingo::Application {
public:
	Sandbox() { 
		PushLayer(new ExampleLayer());
	}
	~Sandbox() { }
};

Kingo::Application* Kingo::CreateApplication() {
	return new Sandbox();
}

