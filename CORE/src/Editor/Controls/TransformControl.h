#pragma once

#include "IControl.h"
#include "Editor/Widgets/Buttons.h"
#include "ECS/Components/Transform.h"

namespace Orbit::UI {
	class TransformControl : public IControl {
	public:
		FORCEINLINE ~TransformControl() = default;
		FORCEINLINE TransformControl(Scene* scene): IControl(scene, TypeOf<ECS::Transform>()), transform(nullptr) { }

		FORCEINLINE bool Init(ECS::Entity entity) {
			if (entity.HasComponent<ECS::Transform>()) {
				transform = &entity.GetComponent<ECS::Transform>();
				return true;
			}
			return false;
		}

		FORCEINLINE void Show() {
			if(this->BeginControl(FIELD(ICON_FA_CUBE, "Transform"), 83)) {
				std::bitset<3> hasChanged(0);
				hasChanged.set(0, ObVec3Input("Position", transform->Position, 0.0f, 0.1f));
				hasChanged.set(1, ObVec3Input("Rotation", transform->Rotation, 0.0f, 0.1f));
				hasChanged.set(2, ObVec3Input("Scale", transform->Scale, 1.0f, 0.1f, 0.005f));
				if (hasChanged.any()) { Application::Dispatcher()->Post<TransformControlEvent>(); }
			}
			this->EndControl();			
		}

	private:
		ECS::Transform* transform;
	};
}