#include "Sandbox2D.h"
#include "ImGui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = 
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWDDDDDDDDWWWWWWWW"
"WWWWWDDDDDDDDDDDDDWWWWWW"
"WWWWDDDDDDDDDDDDDDDWWWWW"
"WWWDDDDDDDDDDDDDDDDDDWWW"
"WWDDDDDDDWDDDDDDDDDDDDWW"
"WDDDDDDDWWWWWDDDDDDDDDWW"
"WWDDDDDDWWWWDDCDDDDDDDWW"
"WWWWDDDDDWWDDDDDDDDDDDDW"
"WWWWDDDDDDDDDDDDDDDWWWWW"
"WWWWWWDDDDDDDDDDDDWWWWWW"
"WWWWWWDDDDDDDDDDDDWWWWWW"
"WWWWWWWWWDDDDDDWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) { }

void Sandbox2D::OnAttach() { 
	KE_PROFILE_FUNCTION();

	m_Texture = Kingo::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_SpriteSheet = Kingo::Texture2D::Create("Assets/Game/Textures/RPGpack_sheet_2X.png");

	m_TextureStairs = Kingo::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 });
	m_TextureTree = Kingo::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
	s_TextureMap['D'] = Kingo::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 });
	s_TextureMap['W'] = Kingo::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 });

	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 2.5f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_CameraController.SetZoomLevel(5.0f);
}
void Sandbox2D::OnDetach() { 
	KE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Kingo::Timestep ts) {
	KE_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	Kingo::Renderer2D::ResetStats();
	{
		KE_PROFILE_SCOPE("Renderer Prep");
		Kingo::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Kingo::RenderCommand::Clear();
	}

#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		KE_PROFILE_SCOPE("Renderer Draw");
		Kingo::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Kingo::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Kingo::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.3f, 0.2f, 0.8f, 1.0f });
		Kingo::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture, 10.0f);
		Kingo::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(45.0f), m_Texture, 20.0f);

		Kingo::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(rotation), { 0.2f, 0.8f, 0.3f, 1.0f });

		Kingo::Renderer2D::EndScene();
		
		Kingo::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Kingo::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Kingo::Renderer2D::EndScene();
	}
#endif

	if (Kingo::Input::IsMouseButtonPressed(KE_MOUSE_BUTTON_LEFT)) {
		auto [x, y] = Kingo::Input::GetMousePosition();
		auto width = Kingo::Application::Get().GetWindow().GetWidth();
		auto height = Kingo::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 50; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	Kingo::Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (uint32_t y = 0; y < m_MapHeight; y++) {
		for (uint32_t x = 0; x < m_MapWidth; x++) {
			char tileType = s_MapTiles[x + y * m_MapWidth];
			Kingo::Ref<Kingo::SubTexture2D> texture;
			if (s_TextureMap.find(tileType) != s_TextureMap.end()) {
				texture = s_TextureMap[tileType];
			}
			else {
				texture = m_TextureStairs;
			}
			Kingo::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f, 0.0f }, { 1.0f, 1.0f }, texture);
		}
	}

	Kingo::Renderer2D::EndScene();
}
void Sandbox2D::OnImGuiRender() {
	KE_PROFILE_FUNCTION();

	/*
	ImGui::Begin("Settings");

	auto stats = Kingo::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
	*/

	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen) {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Exit")) Kingo::Application::Get().Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");

	auto stats = Kingo::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	uint32_t textureID = m_Texture->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
	ImGui::End();

	ImGui::End();
}
void Sandbox2D::OnEvent(Kingo::Event& e) {
	m_CameraController.OnEvent(e);
}