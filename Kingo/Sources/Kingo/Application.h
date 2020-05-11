#pragma once
#include "Core.h"

namespace Kingo {

	class KINGO_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in clients
	Application* CreateApplication();

}
