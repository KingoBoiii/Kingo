#include "kepch.h"
#include "WindowsWindow.h"

#include "Kingo/Events/ApplicationEvent.h"
#include "Kingo/Events/MouseEvent.h"
#include "Kingo/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Kingo {

	static int s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description) {
		KE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(const WindowProps& props) { return CreateScope<WindowsWindow>(props); }

	WindowsWindow::WindowsWindow(const WindowProps& props) { 
		KE_PROFILE_FUNCTION();

		Init(props);
	}
	WindowsWindow::~WindowsWindow() { 
		KE_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		KE_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		KE_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0) {
			KE_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			KE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			KE_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), 0, 0);
			++s_GLFWWindowCount;
		}
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* win, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* win) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			WindowCloseEvent event;
			data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* win, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* win, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* win, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* win, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(win);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::Shutdown() {
		KE_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0) {
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate() {
		KE_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled) {
		KE_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else 
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

}
