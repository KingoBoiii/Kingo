#include "kepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Kingo {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) { 
		KE_CORE_ASSERT(windowHandle, "Window Handle is Null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		KE_CORE_ASSERT(status, "Failed to initialize GLAD!");

		KE_CORE_INFO("OpenGL Info:");
		KE_CORE_INFO("      Vendor:   {0}", glGetString(GL_VENDOR));
		KE_CORE_INFO("      Renderer: {0}", glGetString(GL_RENDERER));
		KE_CORE_INFO("      Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {


		glfwSwapBuffers(m_WindowHandle);
	}

}