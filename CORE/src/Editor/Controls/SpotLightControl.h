#pragma once

#include "IControl.h"
#include "ECS/Components/SpotLight.h"

namespace Orbit::UI {
	class SpotLightControl : public IControl {
	public:
		FORCEINLINE SpotLightControl(Scene* scene) : IControl(scene, TypeOf<ECS::SpotLight>()), _Light(nullptr) {}

		FORCEINLINE bool Init(ECS::Entity entity) {
			if (entity.HasComponent<ECS::SpotLight>()) {
				_Light = &entity.GetComponent<ECS::SpotLight>();
				return true;
			}
			return false;
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_LIGHTBULB, "Spot Light"), 111)) {
				ImGui::Columns(2, "RadianceLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Radiance");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObColorEdit3("##", _Light->Radiance);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "IntensityLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Intensity");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##Intensity", _Light->Intensity, 1.0f, 1.0f, 0.0f);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "InnerCutoffLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("InnerCutOff");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##innercuttoff", _Light->InnerCutOff, 0.1f, -360.0f, 360);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "OuterCutoffLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("OuterCutOff");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##outercuttoff", _Light->OuterCutOff, 0.1f, -360.0f, 360);
				}
				ImGui::EndColumns();

			}
			this->EndControl();
		}

	private:
		ECS::SpotLight* _Light;
	};
}