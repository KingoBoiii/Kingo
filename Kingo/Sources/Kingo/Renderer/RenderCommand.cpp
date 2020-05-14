#include "kepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Kingo {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}