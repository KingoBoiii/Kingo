#include "kepch.h"
#include "VertexArray.h"

#include "Kingo/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Kingo {

	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexArray>();
		}
		KE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}