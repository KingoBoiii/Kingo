#pragma once

#ifdef KE_PLATFORM_WINDOWS

extern Kingo::Application* Kingo::CreateApplication();

int main(int argc, char** argv) {
	Kingo::Application* app = Kingo::CreateApplication();
	app->Run();
	delete app;
}

#endif
