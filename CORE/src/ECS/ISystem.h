#pragma once

#include "Core/Application.h"

namespace Orbit::ECS {
	class ISystem {
	public:
		FORCEINLINE ISystem() = default;
		FORCEINLINE virtual ~ISystem() = default;

		virtual void Init() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;

	protected:
		Scene* _Scene = Application::GetScene();
		ResourceManager* _Loader = _Scene->Loader();
		EventDispatcher* _Dispatcher = Application::Dispatcher();
	};
}