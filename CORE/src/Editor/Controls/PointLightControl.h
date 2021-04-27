#pragma once

#include "IControl.h"
#include "ECS/Components/PointLight.h"

namespace Orbit::UI {
	class PointLightControl : public IControl {
	public:
		FORCEINLINE ~PointLightControl() = default;

		FORCEINLINE PointLightControl(Scene* scene) : IControl(scene, TypeOf<ECS::PointLight>()), _Light(nullptr) {}

		FORCEINLINE bool Init(ECS::Entity entity) {
			if (entity.HasComponent<ECS::PointLight>()) {
				_Light = &entity.GetComponent<ECS::PointLight>();
				return true;
			}
			return false;
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_LIGHTBULB, "Point Light"), 151)) {
				ImGui::Columns(2, "PColorLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Radiance");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObColorEdit3("##" ,_Light->Radiance);				
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "PSIntensityLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Intensity");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##PLightIntensity", _Light->Intensity, 1.0f, 1.0f);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "CastShadowLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Cast Shadow");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObCheckBox("##CastShadow", _Light->CastShadow);
				}
				ImGui::EndColumns();

				// if light casting enable
				if (_Light->CastShadow) {

					ImGui::Columns(2, "FarLayout", false);
					{
						ImGui::SetColumnWidth(0, COLUMN_WIDTH);
						ObTextField("Far Plane");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						ObDragFloat("##FarPlane", _Light->Projection.FarPlane);
					}
					ImGui::EndColumns();

					ImGui::Columns(2, "NearLayout", false);
					{
						ImGui::SetColumnWidth(0, COLUMN_WIDTH);
						ObTextField("Near Plane");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						ObDragFloat("##NearPlane", _Light->Projection.NearPlane);
					}
					ImGui::EndColumns();

					ImGui::Columns(2, "FOVLayout", false);
					{
						ImGui::SetColumnWidth(0, COLUMN_WIDTH);
						ObTextField("FOV");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						ObDragFloat("##fov", _Light->Projection.FieldOfView, 1.0f, -360.0f, 360.0f);
					}
					ImGui::EndColumns();					
				}
			}
			this->EndControl();
		}

	private:
		ECS::PointLight* _Light;
	};
}