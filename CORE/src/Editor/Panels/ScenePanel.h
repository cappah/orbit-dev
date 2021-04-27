#pragma once

#include "IPanel.h"
#include "Core/Application.h"
#include "Editor/Widgets/Widgets.h"
#include "Editor/Tools/TransformGizmo.h"
#include "Events/Categories/KeyEvents.h"
#include "Events/Categories/MouseEvents.h"
#include "Events/Categories/EditorEvents.h"
#include "Events/Categories/CameraEvents.h"

namespace Orbit::UI {

	enum GameMode {
		IN_EDITOR = 0,
		FULLSCREEN = 1
	};	

	class ScenePanel : public IPanel {
	public:
		FORCEINLINE ScenePanel(Scene* scene) : IPanel(scene), _GizmoModeLocal(true), SceneMapID(0),
			_SceneSize(0), _IsPanelActive(false), _SelectedEntityID(ECS::INVALID_HANDLE) {
			_Flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | 
				ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus;

			SceneMapID = _TargetScene->Renderer()->GetFrameBuffer()->GetFrameMap();

			// REGISTER CALLBACK EVENTS
			auto disp = Application::Dispatcher();
			disp->AddListener<KeyPressedEvent>(OB_BIND_FN(ScenePanel::OnKeyPressed));
			disp->AddListener<MouseDragEvent>(OB_BIND_FN(ScenePanel::OnMouseDragged));
			disp->AddListener<MouseScrollEvent>(OB_BIND_FN(ScenePanel::OnMouseScrolled));
			disp->AddListener<EntitySelectedEvent>(OB_BIND_FN(ScenePanel::OnEntitySelected));
		}

		FORCEINLINE ~ScenePanel() = default;	

		FORCEINLINE void Show() {	
			ImGui::Begin("debugg");
				ObImageView(_TargetScene->Renderer()->GetDepthBuffer()->GetDepthMap(), ImVec2(400, 300));
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));			
			ImGui::Begin(ICON_FA_MAP" Scene", nullptr, _Flags);
			{													
				ImGui::BeginChild("##sceneframe");
				{
					auto pos = ImGui::GetWindowPos();
					auto size = ImGui::GetWindowContentRegionMax();
					_Gizmo.Update(pos.x, pos.y, size.x, size.y);

					ObImageView(SceneMapID, size);
					_Gizmo.Draw();

					this->ToolBar(pos);					

					CheckSizeChanged((int32)size.x, (int32)size.y);
					_IsPanelActive = ImGui::IsWindowHovered() && ImGui::IsWindowFocused() && !ImGuizmo::IsUsing();
				}
				ImGui::EndChild();
			}
			ImGui::End();
			ImGui::PopStyleVar();			
		}

	private:
		FORCEINLINE void ToolBar(ImVec2 pos) {				
			ImGui::SetNextWindowPos(pos);
			//ImGui::SetNextWindowBgAlpha(0.8f);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, GRAY);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 3));
			if (ImGui::BeginChild("##Toolbuttons", ImVec2(0, 30), OVERLAY_FLAGS)) {

				ImGui::Dummy(ImVec2(0, 0));
				ImGui::Dummy(ImVec2(0, 0));
				{
					ImGui::SameLine();
					if (ObToolButton(ICON_FA_TRASH_ALT)) { Application::Dispatcher()->Post<DestroyEntityEvent>(ECS::Entity(_SelectedEntityID, nullptr)); }

					ImGui::SameLine();
					if (ObToolButton(ICON_FA_EYE_SLASH)) { Application::Dispatcher()->Post<DesableEntityEvent>(); }

					ImGui::SameLine();
					if (ObToolButton(ICON_FA_PLUS)) { Application::Dispatcher()->Post<AddEntityEvent>(); }

					ImGui::SameLine();
					ImGui::Dummy(ImVec2(20, 0));
				}

				{
					ImGui::SameLine();
					if (ObToolButton(ICON_FA_EXPAND_ARROWS_ALT)) { _Gizmo.SetOperation(ImGuizmo::TRANSLATE); }

					ImGui::SameLine();
					if (ObToolButton(ICON_FA_SYNC_ALT)) { _Gizmo.SetOperation(ImGuizmo::ROTATE); }

					ImGui::SameLine();
					if (ObToolButton(ICON_FA_EXPAND)) { _Gizmo.SetOperation(ImGuizmo::SCALE); }

					ImGui::SameLine();
					if (ObCheckBox("LOCAL", _GizmoModeLocal)) {
						if (_GizmoModeLocal) {
							_Gizmo.SetMode(ImGuizmo::LOCAL);
							OB_INFO("gizmo mode set to 'local'!");
						}
						else {
							_Gizmo.SetMode(ImGuizmo::WORLD);
							OB_INFO("gizmo mode set to 'world'!");
						}
					}
				}

				ImGui::SameLine();
				ImGui::Dummy(ImVec2(20, 0));

				{
					ImGui::SameLine();
					if (ObToolButton(ICON_FA_STOP)) { Application::Dispatcher()->Post<StopGameEvent>(); }

					ImGui::SameLine();
					if (ObToolButton(ICON_FA_PLAY)) { Application::Dispatcher()->Prioritize<StartGameEvent>(IN_EDITOR); }

					ImGui::SameLine();
					if (ObToolButton(ICON_FA_GAMEPAD)) { Application::Dispatcher()->Prioritize<StartGameEvent>(FULLSCREEN); }
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();			
			ImGui::PopStyleColor();			
		}

		FORCEINLINE void CheckSizeChanged(int32 width, int32 height) {				
			if (width != _SceneSize.x || height != _SceneSize.y) {
				_SceneSize = vec2i(width, height);				
				Application::Dispatcher()->Post<SceneResizedEvent>(width, height);
			}
		}

	private:

		FORCEINLINE void OnKeyPressed(const KeyPressedEvent& e) {
			if (_IsPanelActive) {
				if (e.KeyCode() == KEY_1) { 
					_Gizmo.SetOperation(ImGuizmo::TRANSLATE); 
					OB_INFO("gizmo operation changed to translate");
				}
				if (e.KeyCode() == KEY_2) { 
					_Gizmo.SetOperation(ImGuizmo::ROTATE); 
					OB_INFO("gizmo operation changed to rotate");
				}
				if (e.KeyCode() == KEY_3) { 
					_Gizmo.SetOperation(ImGuizmo::SCALE); 
					OB_INFO("gizmo operation changed to scale");
				}	

				// focus cam on object
				if (e.KeyCode() == KEY_F) {
					auto entity =_TargetScene->ToEntity(_SelectedEntityID);
					auto& transform = entity.GetComponent<ECS::Transform>();					
					vec3f target = transform.Position + vec3f(0, 0, 3 * transform.Scale.z);
					Application::Dispatcher()->Prioritize<CameraFocusEvent>(target);
				}
			}
		}

		FORCEINLINE void OnMouseDragged(const MouseDragEvent& e) {
			if (_IsPanelActive) {
				double dt = 0.016;
				Application::Dispatcher()->Prioritize<CameraMovedEvent>(e.DeltaX() * dt, e.DeltaY() * dt);
			}
		}

		FORCEINLINE void OnMouseScrolled(const MouseScrollEvent& e) {
			if (_IsPanelActive) {
				if (Application::Inputs()->IsKeypress(KEY_LEFT_CONTROL)) {
					Application::Dispatcher()->Prioritize<CameraZoomEvent>(5.0 * e.ScrollY());
				}
				else {
					Application::Dispatcher()->Prioritize<CameraZoomEvent>(e.ScrollY());
				}
			}
		}

		FORCEINLINE void OnEntitySelected(const EntitySelectedEvent& e) {
			auto entity = e.GetEntity();
			_SelectedEntityID = entity.IsValid() ? entity.GetHandle() : ECS::INVALID_HANDLE;
			OB_INFO("entt selected %lu", entity.GetHandle());
		}

	private:
		bool _IsPanelActive;
		bool _GizmoModeLocal;
		int32 _SelectedEntityID;

		vec2i _SceneSize;
		uint32 SceneMapID;
		TransformGizmo _Gizmo;
		ImGuiWindowFlags _Flags;
	};
}