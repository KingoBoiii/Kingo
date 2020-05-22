#pragma once
#include <Kingo.h>

#include "ParticleSystem.h"

class Sandbox2D : public Kingo::Layer {
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Kingo::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Kingo::Event& e) override;
private:
	Kingo::OrthographicCameraController m_CameraController;

	Kingo::Ref<Kingo::VertexArray> m_SquareVA;
	Kingo::Ref<Kingo::Shader> m_FlatColorShader;

	Kingo::Ref<Kingo::Texture2D> m_Texture;
	Kingo::Ref<Kingo::Texture2D> m_SpriteSheet;
	Kingo::Ref<Kingo::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Kingo::Ref<Kingo::SubTexture2D>> s_TextureMap;
};