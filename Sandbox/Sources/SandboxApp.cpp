#include <Kingo.h>
#include "imgui/imgui.h"

class ExampleLayer : public Kingo::Layer {
public:
	ExampleLayer() : Layer("Example") { }

	void OnUpdate() override {
		if (Kingo::Input::IsKeyPressed(KE_KEY_TAB)) {
			KE_INFO("Tab key is pressed! (Poll)");
		}
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello, world");
		ImGui::End();
	}

	void OnEvent(Kingo::Event& e) override {
		if (e.GetEventType() == Kingo::EventType::KeyPressed) {
			Kingo::KeyPressedEvent& kpe = (Kingo::KeyPressedEvent&)e;
			if (kpe.GetKeyCode() == KE_KEY_TAB) {
				KE_INFO("Tab key is pressed! (Event)");
			}
			KE_INFO("{0}", (char)kpe.GetKeyCode());
		}
	}
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

