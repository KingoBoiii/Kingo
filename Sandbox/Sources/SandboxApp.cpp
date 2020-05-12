#include <Kingo.h>
#include <string>

class ExampleLayer : public Kingo::Layer {
public:
	ExampleLayer() : Layer("Example") {

	}

	void OnUpdate() override {
		KE_INFO("ExampleLayer::Update");
	}

	void OnEvent(Kingo::Event& e) override {
		KE_TRACE("{0}", e);
	}
};

class Sandbox : public Kingo::Application {
public:
	Sandbox() { 
		PushLayer(new ExampleLayer());
		PushOverlay(new Kingo::ImGuiLayer());
	}
	~Sandbox() { }
};

Kingo::Application* Kingo::CreateApplication() {
	return new Sandbox();
}

