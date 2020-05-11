#include <Kingo.h>

class Sandbox : public Kingo::Application {
public:
	Sandbox() { }
	~Sandbox() { }
};

Kingo::Application* Kingo::CreateApplication() {
	return new Sandbox();
}

