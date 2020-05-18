#include "kepch.h"
#include "Texture.h"

#include "Kingo/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Kingo {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(width, height);
		}
		KE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		KE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);
		}
		KE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}