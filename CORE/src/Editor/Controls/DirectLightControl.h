#pragma once

#include "IControl.h"
#include "ECS/Components/DirectionalLight.h"

namespace Orbit::UI {
	class DirectLightControl : public IControl {
	public:
		FORCEINLINE ~DirectLightControl() = default;

		FORCEINLINE DirectLightControl(Scene* scene) : IControl(scene, TypeOf<ECS::DirectionalLight>()), _Light(nullptr) {}

		FORCEINLINE bool Init(ECS::Entity entity) {
			if (entity.HasComponent<ECS::DirectionalLight>()) {
				_Light = &entity.GetComponent<ECS::DirectionalLight>();
				return true;
			}
			return false;
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_SUN, "Global Light"), 171)) {
				ImGui::Columns(2, "DColorLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Radiance");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObColorEdit3("##", _Light->Radiance);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "DSIntensityLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Intensity");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##DLightIntensity", _Light->Intensity, 1.0f, 1.0f);
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

					ImGui::Columns(2, "WidthLayout", false);
					{
						ImGui::SetColumnWidth(0, COLUMN_WIDTH);
						ObTextField("Rect Width");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						ObDragFloat("##RectWidth", _Light->Projection.Width);
					}
					ImGui::EndColumns();

					ImGui::Columns(2, "HeightLayout", false);
					{
						ImGui::SetColumnWidth(0, COLUMN_WIDTH);
						ObTextField("Rect Height");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						ObDragFloat("##RectHeight", _Light->Projection.Height);
					}
					ImGui::EndColumns();
				}
			}
			this->EndControl();
		}

	private:
		ECS::DirectionalLight* _Light;
	};
}