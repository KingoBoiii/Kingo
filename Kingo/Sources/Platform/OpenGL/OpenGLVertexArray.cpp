#include "kepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Kingo {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
			case Kingo::ShaderDataType::Float:		return GL_FLOAT;
			case Kingo::ShaderDataType::Float2:		return GL_FLOAT;
			case Kingo::ShaderDataType::Float3:		return GL_FLOAT;
			case Kingo::ShaderDataType::Float4:		return GL_FLOAT;
			case Kingo::ShaderDataType::Int:		return GL_INT;
			case Kingo::ShaderDataType::Int2:		return GL_INT;
			case Kingo::ShaderDataType::Int3:		return GL_INT;
			case Kingo::ShaderDataType::Int4:		return GL_INT;
			case Kingo::ShaderDataType::Mat3:		return GL_FLOAT;
			case Kingo::ShaderDataType::Mat4:		return GL_FLOAT;
			case Kingo::ShaderDataType::Bool:		return GL_BOOL;
		}
		KE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() { 
		KE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
		// glBindVertexArray(m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray() {
		KE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const { 
		KE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const { 
		KE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { 
		KE_PROFILE_FUNCTION();

		KE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		KE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}