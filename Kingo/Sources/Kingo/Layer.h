#pragma once
#include "Kingo/Core.h"
#include "Kingo/Events/Event.h"

namespace Kingo {

	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected: 
		std::string m_DebugName;
	};

}
