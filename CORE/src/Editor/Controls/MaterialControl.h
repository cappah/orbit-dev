#pragma once

#include "IControl.h"
#include "Editor/Widgets/Buttons.h"
#include "Editor/Widgets/Widgets.h"
#include "ECS/Components/MeshRenderer.h"
#include "Editor/Modals/TexturesModal.h"
#include "Events/Categories/ModalEvents.h"

namespace Orbit::UI {

	class MaterialControl : public IControl {
	public:
		FORCEINLINE ~MaterialControl() = default;

		FORCEINLINE MaterialControl(Scene* scene) : IControl(scene, 0, false), _Material(nullptr) {			
			Application::Dispatcher()->AddListener<MaterialTextureSelectedEvent>(OB_BIND_FN(MaterialControl::OnSelected));
		}

		FORCEINLINE bool Init(ECS::Entity entity) {			
			return false;
		}

		FORCEINLINE void SetCurrent(Material* material) {
			if (material) {
				_Material = material;
			}
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_PAINT_BRUSH, "Material"), 304)) {				

				ImGui::Columns(2, "AOLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Ao.");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ObDragFloat("##AO", _Material->Data.Ao, 0.01f, 0.0001f, 1.0f)) {}
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "AlbedoLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Albedo");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ObColorEdit3("##Albedo", _Material->Data.Albedo)) {}
				}
				ImGui::EndColumns();				

				ImGui::Columns(2, "OpacityLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Opacity");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ObDragFloat("##Opacity", _Material->Data.Opacity, 0.01f, 0.0f, 1.0f)) {}
				}
				ImGui::EndColumns();				

				ImGui::Columns(2, "MetalnessLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Metalness");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ObDragFloat("##Metalness", _Material->Data.Metallic, 0.01f, 0.0001f, 1.0f)) {}
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "RoughnessLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Roughness");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ObDragFloat("##Roughness", _Material->Data.Roughness, 0.01f, 0.0001f, 1.0f)) {}
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "ReflectLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Reflectivity");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##Reflectivity", _Material->Data.Reflectivity, 0.01f, 0.0001f, 1.0f);
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "ShininessLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Shininess");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ObDragFloat("##Shininess", _Material->Data.Shininess, 0.01f, 0.0001f, 1.0f);
				}
				ImGui::EndColumns();

				// TEXTURE-MAPS

				ImGui::Columns(2, "AOMapLayout", false);
				{
					ImGui::PushID("AO");
					{
						ImGui::SetColumnWidth(0, COLUMN_WIDTH);
						ObTextField("Ao. Map");
						ImGui::NextColumn();

						if (ObSmallButton(ICON_FA_PENCIL_ALT)) {
							OB_INFO("button clicked!");
							Application::Dispatcher()->Post<ShowMaterialTextureModalEvent>(MaterialTextureType::AO);
						}

						if (_Material->Data.AOMap) {
							ImGui::SameLine();
							ObTextureView(_Material->Data.AOMap);
						}
					}
					ImGui::PopID();
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "AlbedoMapLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Albedo Map");
					ImGui::NextColumn();
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {}

					if (_Material->Data.AlbedoMap) {
						ImGui::SameLine();
						ObTextureView(_Material->Data.AlbedoMap);
					}
				}
				ImGui::EndColumns();	

				ImGui::Columns(2, "NormalMapLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Normal Map");
					ImGui::NextColumn();
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {}

					if (_Material->Data.NormalMap) {
						ImGui::SameLine();
						ObTextureView(_Material->Data.NormalMap);
					}
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "MetelnessMapLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Metalness Map");
					ImGui::NextColumn();
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {}

					if (_Material->Data.MetallicMap) {
						ImGui::SameLine();
						ObTextureView(_Material->Data.MetallicMap);
					}
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "RoughnessMapLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Roughness Map");
					ImGui::NextColumn();
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {}

					if (_Material->Data.RoughnessMap) {
						ImGui::SameLine();
						ObTextureView(_Material->Data.RoughnessMap);
					}
				}
				ImGui::EndColumns();

				ImGui::Columns(2, "SpecBRDFMapLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Spec. BRDF Map");
					ImGui::NextColumn();
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {}

					if (_Material->Data.SpecularBRDFMap) {
						ImGui::SameLine();
						ObTextureView(_Material->Data.SpecularBRDFMap);
					}
				}
				ImGui::EndColumns();
			}
			this->EndControl();
		}

	private:
		FORCEINLINE void OnSelected(const MaterialTextureSelectedEvent& e) {
			_Material->Data.AlbedoMap = e.GetTexture(); 
			OB_INFO("albedo map seted!");
		}

	private:
		Material* _Material;
	};
}