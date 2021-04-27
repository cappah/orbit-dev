#pragma once

#include "Scene/Scene.h"
#include "Core/Application.h"
#include "Editor/Widgets/Widgets.h"
#include "Events/Categories/SceneEvents.h"
#include "Events/Categories/EditorEvents.h"

namespace Orbit::UI {	
	class IControl {
	public:
		virtual ~IControl() = default; 

		FORCEINLINE IControl(Scene* scene, RuntimeType type, bool removeable = true):
			_TargetScene(scene), _IsCollapsed(false), _ComponentTypeID(type), 
			_IsRemoveable(removeable), _SelectedEntityID(ECS::INVALID_HANDLE) {

			auto disp = Application::Dispatcher();
			disp->AddListener<EntitySelectedEvent>(OB_BIND_FN(IControl::OnEntitySelected));
		}

		virtual bool Init(ECS::Entity entity) = 0;
		virtual void Show() = 0;

		FORCEINLINE bool BeginControl(const char* title, float height) {
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
			auto width = ImGui::GetWindowWidth();
			_IsCollapsed = ImGui::CollapsingHeader(title, COLLAPSE_HEADER_FLAGS);			

			// remove button
			if (_IsRemoveable) {
				ImGui::SameLine();
				ImGui::PushID(title);
				ImGui::SetCursorPosX(width - 35.0f);
				
				if (ObToolButton(ICON_FA_TRASH_ALT, NONE, ImVec2(24.0f, 24.0f), 2.0f) && _SelectedEntityID != ECS::INVALID_HANDLE) {
					Application::Dispatcher()->Post<RemoveComponentEvent>(_TargetScene->ToEntity(_SelectedEntityID), _ComponentTypeID);
				}
				ImGui::PopID();
			}

			//ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			if (_IsCollapsed) { 
				ImGui::BeginChildFrame(ImGui::GetID(title), ImVec2(0, height)); 
			}
			ImGui::PopStyleVar(2);
			return _IsCollapsed;
		}

		FORCEINLINE void EndControl() {
			if (_IsCollapsed) { ImGui::EndChildFrame(); }
		}

		FORCEINLINE void OnEntitySelected(const EntitySelectedEvent& e) {
			_SelectedEntityID = e.GetEntity();
		}

	private:
		bool _IsCollapsed;	
		bool _IsRemoveable;
		RuntimeType _ComponentTypeID;

	protected:		
		Scene* _TargetScene;	
		uint32 _SelectedEntityID;
	};
}