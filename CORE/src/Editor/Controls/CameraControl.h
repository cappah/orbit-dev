#pragma once

#include "IControl.h"
#include "ECS/Components/Camera.h"

namespace Orbit::UI {
	class CameraControl : public IControl {
	public:
		FORCEINLINE ~CameraControl() = default;

		FORCEINLINE CameraControl(Scene* scene) : IControl(scene, TypeOf<ECS::Camera>()), _Camera(nullptr) {}

		FORCEINLINE bool Init(ECS::Entity entity) {
			if (entity.HasComponent<ECS::Camera>()) {
				_Camera = &entity.GetComponent<ECS::Camera>();
				return true;
			}
			return false;
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_VIDEO, "Camera"), 105)) {
				ImGui::Columns(2, "IsActiveLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Active");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObCheckBox("##IsActive", _Camera->IsPrimary);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "FOVLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("FOV");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##FOVValue", _Camera->Handle.FieldOfView, 1.0f, 0.0001f, 360.0f);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "NearLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Near");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##NearPlane", _Camera->Handle.NearPlane);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "FarLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Far");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##FarPlane", _Camera->Handle.FarPlane);
				}
				ImGui::EndColumns();				
			}			
			this->EndControl();
		}

	private:
		ECS::Camera* _Camera;
	};
}