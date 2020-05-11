#include "kepch.h"
#include "Application.h"

#include "Kingo/Events/ApplicationEvent.h"
#include "Kingo/Log.h"

namespace Kingo {

	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			KE_TRACE(e);
		}

		while (true);
	}

}