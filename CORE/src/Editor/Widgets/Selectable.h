#pragma once

#include "Editor/Colors.h"
#include "Events/EventDispatcher.h"
#include "Events/Categories/SelectablesEvents.h"

namespace Orbit::UI {

	struct ObTextureSelectable {

		ObTextureSelectable(Texture texture, EventDispatcher* disp = nullptr, ImVec2 size = ImVec2(80, 80)) :
			_Dispatcher(disp), _Texture(texture), _Size(size){}

		void Show() {
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, LIGHTGRAY);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, BLUE);
			ImGui::PushStyleColor(ImGuiCol_Button, NONE);
			
			if (ImGui::ImageButton((ImTextureID)_Texture.TextureID, _Size) && _Dispatcher) {				
				_Dispatcher->Post<MaterialTextureSelectedEvent>(_Texture);
			}

			ImGui::SetNextItemWidth(_Size.x);
			ImGui::Text(_Texture.Name.c_str());

			ImGui::PopStyleColor(3);
		}

	private:
		ImVec2 _Size;
		Texture _Texture;
		EventDispatcher* _Dispatcher = nullptr;
	};
}