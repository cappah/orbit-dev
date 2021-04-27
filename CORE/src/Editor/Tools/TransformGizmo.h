#pragma once

#include <ImGui/ImGuizmo.h>
#include "Core/Application.h"
#include "ECS/Components/Transform.h"
#include "Events/Categories/AppEvents.h"
#include "Events/Categories/SceneEvents.h"
#include "Events/Categories/EditorEvents.h"
#include "Events/Categories/CameraEvents.h"

namespace Orbit::UI {
	class TransformGizmo {
	public:
		FORCEINLINE ~TransformGizmo() = default;

		FORCEINLINE TransformGizmo(): _Model(0), _View(0), _Projection(0) {
			ImGuizmo::SetOrthographic(false);
			auto disp = Application::Dispatcher();
			disp->AddListener<CameraEvent>(OB_BIND_FN(TransformGizmo::OnCameraEvent));
			disp->AddListener<EntitySelectedEvent>(OB_BIND_FN(TransformGizmo::OnEntitySelected));
			disp->AddListener<EntityDestroyedEvent>(OB_BIND_FN(TransformGizmo::OnEntityDestroyed));
			disp->AddListener<TransformControlEvent>(OB_BIND_FN(TransformGizmo::OnTargetTransformChanged));
		}

		FORCEINLINE void Draw() {
			if (!_CanUse) { return; }

			ImGuizmo::Manipulate(VAL_PTR(_View), VAL_PTR(_Projection), _Operation,
				_Mode, VAL_PTR(_Model), nullptr, (_IsSnapActive) ? &_UseSnap.x : nullptr);

			if (ImGuizmo::IsUsing()) {
				_IsSnapActive = false;
				vec3f rotation = _Transform->Rotation;
				ImGuizmo::DecomposeMatrixToComponents(VAL_PTR(_Model), &_Transform->Position.x, &rotation.x, &_Transform->Scale.x);	
				_Transform->Rotation = rotation;

				if (Application::Inputs()->IsKeypress(KEY_LEFT_CONTROL)) {
					_IsSnapActive = true;
					_UseSnap = vec3f(0.5f);

					if (_Operation == ImGuizmo::ROTATE) {
						_UseSnap = vec3f(45.0f);
					}
				}
			}
		}		

		FORCEINLINE void SetMode(ImGuizmo::MODE mode) {
			_Mode = mode;
		}

		FORCEINLINE void Update(float x, float y, float w, float h) {
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(x, y, w, h);
		}

		FORCEINLINE void SetOperation(ImGuizmo::OPERATION operation) {
			_Operation = operation;
		}

		// EVENT CALLBACKS
		
		FORCEINLINE void OnCameraEvent(const CameraEvent& e) {
			_View = e.GetView();
			_Projection = e.GetProjection();		
		}

		FORCEINLINE void OnEntitySelected(const EntitySelectedEvent& e) {
			ECS::Entity entity = e.GetEntity();

			if (entity.IsValid()) {				
				if (entity.HasComponent<ECS::Transform>()) {
					_CanUse = true;
					_Transform = &entity.GetComponent<ECS::Transform>();
					_Model = _Transform->TrMatrix();
				}
			}
		}
		
		FORCEINLINE void OnEntityDestroyed(const EntityDestroyedEvent& e) {
			_CanUse = false;
			_Model = matrix4(0);
			_Transform = nullptr;
		}

		FORCEINLINE void OnTargetTransformChanged(const TransformControlEvent& e) {	
			_Model = _Transform->TrMatrix();
			OB_INFO("gizmo target transform changed!");
		}


	private:
		matrix4 _View;
		matrix4 _Model;
		matrix4 _Projection;

		bool _CanUse = false;
		bool _IsSnapActive = false;
		vec3f _UseSnap = glm::vec3(0);
		ECS::Transform* _Transform = nullptr;
		ImGuizmo::MODE _Mode = ImGuizmo::LOCAL;
		ImGuizmo::OPERATION _Operation = ImGuizmo::TRANSLATE;
	};
}