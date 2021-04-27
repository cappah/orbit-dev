#pragma once

#include "IPanel.h"
#include "Core/Application.h"
#include "Events/Categories/SceneEvents.h"

#include "Controls/CameraControl.h"
#include "Controls/CommonControl.h"
#include "Controls/SpotLightControl.h"
#include "Controls/TransformControl.h"
#include "Controls/PointLightControl.h"
#include "Controls/DirectLightControl.h"
#include "Controls/MeshRendererControl.h"

namespace Orbit::UI {
	const char* names[] = {
		"Camera",
		"Transform",
		"Spot Light",
		"Point Light",
		"MeshRenderer",
		"SkyboxRenderer",
		"Directional Light",
	};

	class InspektorPanel : public IPanel {
	public:
		FORCEINLINE ~InspektorPanel() = default;

		FORCEINLINE InspektorPanel(Scene* scene) : IPanel(scene), _SelectedEntityID(ECS::INVALID_HANDLE) {
			_RegisteredControls.push_back(MakeUnique<CommonControl>(scene));
			_RegisteredControls.push_back(MakeUnique<TransformControl>(scene));
			_RegisteredControls.push_back(MakeUnique<MeshRendererControl>(scene));
			_RegisteredControls.push_back(MakeUnique<DirectLightControl>(scene));
			_RegisteredControls.push_back(MakeUnique<SpotLightControl>(scene));
			_RegisteredControls.push_back(MakeUnique<PointLightControl>(scene));
			_RegisteredControls.push_back(MakeUnique<CameraControl>(scene));

			auto disp = Application::Dispatcher();
			disp->AddListener<ClearInspectorEvent>(OB_BIND_FN(InspektorPanel::OnClear));
			disp->AddListener<RefreshInspectorEvent>(OB_BIND_FN(InspektorPanel::OnRefresh));
			disp->AddListener<EntitySelectedEvent>(OB_BIND_FN(InspektorPanel::OnEntitySelected));
			disp->AddListener<ComponentAddedEvent>(OB_BIND_FN(InspektorPanel::OnComponentAdded));
			disp->AddListener<EntityDestroyedEvent>(OB_BIND_FN(InspektorPanel::OnEntityDestroyed));
		}

		FORCEINLINE void  Show() {
			ImGui::Begin(ICON_FA_CUBES" Inspektor");
			{					
				for (auto& control : _Controls) { control->Show(); }			

				ImGui::Dummy(ImVec2(0, 4));
				if (ObCenteredButton(ICON_FA_PLUS_CIRCLE " Add Component", GRAY)) {
					ImGui::OpenPopup("CompPopup");
				}

				if (ImGui::BeginPopup("CompPopup")) {
					ImGui::Text("Components");
					ImGui::Separator();
					for (int8 curr = 0; curr < IM_ARRAYSIZE(names); curr++) {
						if (ImGui::Selectable(names[curr])) {
							Application::Dispatcher()->Post<AddComponentEvent>(ECS::Entity(_SelectedEntityID, nullptr), names[curr]);
						}
					}
					ImGui::EndPopup();
				}				
			}
			ImGui::End();
		}

	private:
		FORCEINLINE void OnRefresh(const RefreshInspectorEvent& e) {
			QueryEntityComponents(e.GetEntity());
		}
		
		FORCEINLINE void OnClear(const ClearInspectorEvent& e) {
			_Controls.clear();
			_SelectedEntityID = ECS::INVALID_HANDLE;
		}

		FORCEINLINE void OnEntitySelected(const EntitySelectedEvent& e) {
			QueryEntityComponents(e.GetEntity());
		}

		FORCEINLINE void OnEntityDestroyed(const EntityDestroyedEvent& e) {
			_Controls.clear();
			_SelectedEntityID = ECS::INVALID_HANDLE;
		}
		
		FORCEINLINE void OnComponentAdded(const ComponentAddedEvent& e) {
			QueryEntityComponents(e.GetEntity());
		}

		FORCEINLINE void QueryEntityComponents(ECS::Entity entity) {
			_Controls.clear();			
			_SelectedEntityID = entity.GetHandle();
			for (auto& control : _RegisteredControls) {
				if (control->Init(entity)) {
					_Controls.push_back(control.get());
				}
			}
		}

	private:
		int32 _SelectedEntityID;
		std::vector<IControl*> _Controls;
		std::vector<Unique<IControl>> _RegisteredControls;
	};
}