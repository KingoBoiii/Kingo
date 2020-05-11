#pragma once

#ifdef KE_PLATFORM_WINDOWS

extern Kingo::Application* Kingo::CreateApplication();

int main(int argc, char** argv) {
	Kingo::Log::Init();
	KE_CORE_WARN("Initialized Log!");
	int a = 5;
	KE_INFO("Hello! Var={0}", a);

	Kingo::Application* app = Kingo::CreateApplication();
	app->Run();
	delete app;
}

#endif
