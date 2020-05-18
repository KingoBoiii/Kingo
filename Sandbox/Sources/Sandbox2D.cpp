#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) { }

void Sandbox2D::OnAttach() { 
	KE_PROFILE_FUNCTION();

	m_Texture = Kingo::Texture2D::Create("Assets/Textures/Checkerboard.png");
}
void Sandbox2D::OnDetach() { 
	KE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Kingo::Timestep ts) {
	KE_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		KE_PROFILE_SCOPE("Renderer Prep");
		Kingo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Kingo::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		KE_PROFILE_SCOPE("Renderer Draw");
		Kingo::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Kingo::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Kingo::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		Kingo::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture, 10.0f);
		// Kingo::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, 45.0f, m_Texture, 20.0f);

		Kingo::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, rotation, { 0.2f, 0.8f, 0.3f, 1.0f });

		Kingo::Renderer2D::EndScene();
	}
}
void Sandbox2D::OnImGuiRender() {
	KE_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}
void Sandbox2D::OnEvent(Kingo::Event& e) {
	m_CameraController.OnEvent(e);
}