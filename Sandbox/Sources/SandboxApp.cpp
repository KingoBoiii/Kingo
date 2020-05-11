#include <Kingo.h>
#include <stdio.h>

class Sandbox : public Kingo::Application {
public:
	Sandbox() {
		printf("Welcome to Sandbox!\n");
	}
	~Sandbox() {

	}
};

Kingo::Application* Kingo::CreateApplication() {
	return new Sandbox();
}

