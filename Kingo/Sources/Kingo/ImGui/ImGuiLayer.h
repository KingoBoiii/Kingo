#pragma once
#include "Kingo/Layer.h"

#include "Kingo/Events/ApplicationEvent.h"
#include "Kingo/Events/KeyEvent.h"
#include "Kingo/Events/MouseEvent.h"

namespace Kingo {

	class KINGO_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}
