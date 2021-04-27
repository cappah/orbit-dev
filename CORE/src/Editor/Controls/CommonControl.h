#pragma once

#include "IControl.h"
#include "ECS/Components/Common.h"

namespace Orbit::UI {
	class CommonControl : public IControl {
	public:
		FORCEINLINE ~CommonControl() = default;

		FORCEINLINE CommonControl(Scene* scene) : IControl(scene, TypeOf<ECS::Common>()), _Common(nullptr), _Buffer(nullptr) {
			Application::Dispatcher()->AddListener<EntitySelectedEvent>(OB_BIND_FN(CommonControl::OnEntitySelected));
		}

		FORCEINLINE bool Init(ECS::Entity entity) {	
			if (entity.HasComponent<ECS::Common>()) {
				_Common = &entity.GetComponent<ECS::Common>();
				return true;
			}
			return false;
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_TAG, "Common"), 39)) {
				ImGui::Columns(2, "ActiveLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField(FIELD(ICON_FA_FINGERPRINT, "Entity"));
					ImGui::NextColumn();					
					ImGui::PushItemWidth(-1);

					ObCheckBox("##isActive", _Common->IsActive);
					ImGui::SameLine();

					ImGui::PushItemWidth(-1);
					if(ObTextInput(_Buffer)) {
						_Common->Name.assign(_Buffer, TEXT_BUFFER_SIZE);
						_Buffer = const_cast<char*>(EMPTY_STR);
						_Buffer = const_cast<char*>(_Common->Name.c_str());
						Application::Dispatcher()->Prioritize<EntityNameChangedEvent>(_Common->Name);
					}
				}
				ImGui::EndColumns();				
			}
			this->EndControl();
		}

		FORCEINLINE void OnEntitySelected(const EntitySelectedEvent& e) {
			_Buffer = const_cast<char*>(e.GetEntity().GetComponent<ECS::Common>().Name.c_str());
		}

	private:
		char* _Buffer;
		ECS::Common* _Common;
	};
}