#pragma once

#include "IControl.h"
#include "MaterialControl.h"
#include "ECS/Components/MeshRenderer.h"
#include "Events/Categories/ResourceEvents.h"

namespace Orbit::UI {
	class MeshRendererControl : public IControl {
	public:
		FORCEINLINE ~MeshRendererControl() = default;

		FORCEINLINE MeshRendererControl(Scene* scene) : IControl(scene, TypeOf<ECS::MeshRenderer>()), _MeshRenderer(nullptr), _CurrentMeshName("select") {
			// meshes
			for (auto& mesh : _TargetScene->Loader()->GetMeshList()) {
				_Meshes.push_back(mesh.Name);
			}

			// materials
			for (auto& mtl : _TargetScene->Loader()->GetMaterialList()) {
				_MaterialNames.push_back(mtl.Name);
			}

			_MaterialControl = MakeUnique<MaterialControl>(scene);			
			Application::Dispatcher()->AddListener<ResourceEvent>(OB_BIND_FN(MeshRendererControl::OnResourceEvent));
		}

		FORCEINLINE bool Init(ECS::Entity entity) {
			if (entity.HasComponent<ECS::MeshRenderer>()) {
				_MeshRenderer = &entity.GetComponent<ECS::MeshRenderer>();		
				_MaterialControl->SetCurrent(_MeshRenderer->Matrial);
				_CurrentMeshName = _MeshRenderer->MeshData->Name.c_str();
				_CurrentMaterialName = _MeshRenderer->Matrial->Name.c_str();
				return true;
			}					
			return false;
		}

		FORCEINLINE void Show() {
			if (this->BeginControl(FIELD(ICON_FA_CUBES, "MeshRenderer"), 61)) {
				// mesh
				ImGui::Columns(2, "MeshLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Mesh");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {
						Application::Dispatcher()->Prioritize<ResourceEvent>("tes mesh", ResourceType::MESH, ResourceAction::LOADED);
					}
					ImGui::SameLine();
					if (ObComboBox("meshes", _Meshes, _CurrentMeshName)) {
						_MeshRenderer->MeshData = _TargetScene->Loader()->GetMesh(_CurrentMeshName);
					}					
				}
				ImGui::EndColumns();

				// material
				ImGui::Columns(2, "MaterialLayout", false);
				{
					ImGui::SetColumnWidth(0, COLUMN_WIDTH);
					ObTextField("Material");
					ImGui::NextColumn();
					if (ObSmallButton(ICON_FA_PENCIL_ALT)) {}
					ImGui::SameLine();
					ImGui::PushItemWidth(-1);
					if (ObComboBox("materials", _MaterialNames, _CurrentMaterialName)) {
						Material* material = _TargetScene->Loader()->MaterialRef(_CurrentMaterialName);
						_MaterialControl->SetCurrent(material);
						_MeshRenderer->Matrial = material;
					}
				}
				ImGui::EndColumns();
			}
			this->EndControl();

			// MATERIAL

			_MaterialControl->Show();
		}

	private:
		FORCEINLINE void OnResourceEvent(const ResourceEvent& e) {
			if (e.GetType() == ResourceType::MESH) {
				_Meshes.push_back(e.GetName());
				OB_INFO("Resource event occured! %s", e.GetName().c_str());
			}
		}

	private:
		ECS::MeshRenderer* _MeshRenderer;
		Unique<MaterialControl> _MaterialControl;

		const char* _CurrentMeshName = EMPTY_STR;
		const char* _CurrentMaterialName = EMPTY_STR;

		std::vector<std::string> _Meshes;
		std::vector<std::string> _MaterialNames;
	};
}