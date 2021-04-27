#pragma once

#include "Scene/Scene.h"

namespace Orbit::UI {
	class IPanel {
	public:
		IPanel(Scene* scene): _TargetScene(scene) {}
		virtual ~IPanel() = default;

		virtual void Show() = 0;

	protected:
		Scene* _TargetScene;
	};
}