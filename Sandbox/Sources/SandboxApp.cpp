#include <Kingo.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class ExampleLayer : public Kingo::Layer {
public:
	ExampleLayer() : Layer("Example") { }

	void OnUpdate() override {
		if (Kingo::Input::IsKeyPressed(KE_KEY_TAB)) {
			KE_INFO("Tab key is pressed! (Poll)");
		}
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

