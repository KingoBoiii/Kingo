#pragma once

#ifdef KE_PLATFORM_WINDOWS

extern Kingo::Application* Kingo::CreateApplication();

int main(int argc, char** argv) {
	Kingo::Log::Init();
	
	KE_PROFILE_BEGIN_SESSION("Startup", "KingoProfile-Startup.json");
	Kingo::Application* app = Kingo::CreateApplication();
	KE_PROFILE_END_SESSION();

	KE_PROFILE_BEGIN_SESSION("Runtime", "KingoProfile-Runtime.json");
	app->Run();
	KE_PROFILE_END_SESSION();

	KE_PROFILE_BEGIN_SESSION("Shutdown", "KingoProfile-Shutdown.json");
	delete app;
	KE_PROFILE_END_SESSION();
}

#endif
