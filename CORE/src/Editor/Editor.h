#pragma once

#include "Panels/IPanel.h"
#include "Modals/IModal.h"
#include "Events/Categories/ModalEvents.h"

namespace Orbit::UI {
	class Editor {
	public:
		Editor() = default;
		~Editor() = default;

		void Init();
		void Show();

	private:
		void EndFrame();
		void NewFrame();
		void InitStyle();	

		// callbacks
		void OnModal(const ShowMaterialTextureModalEvent& e);

	private:
		int32 _DockFlags;
		std::vector<Unique<IModal>> _PopUps;
		std::vector<Unique<IPanel>> _Panels;
	};
}

