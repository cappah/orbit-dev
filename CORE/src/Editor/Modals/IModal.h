#pragma once

#include "Scene/Scene.h"
#include "Core/Application.h"
#include "Editor/Widgets/Widgets.h"
#include "Editor/Widgets/Buttons.h"

namespace Orbit::UI {
	class IModal {
	public:
		virtual ~IModal() = default;
		IModal(): _Open(false) {
			flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | 
				ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize;
		}

		virtual void Show() = 0;

	protected:
		bool _Open;
		ImGuiWindowFlags flags;
	};
}