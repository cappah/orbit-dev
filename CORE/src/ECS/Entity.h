#pragma once

#include "Registry.h"

namespace Orbit::ECS {
	class Entity {
	public:
		FORCEINLINE ~Entity() = default;
		FORCEINLINE Entity() = default;

		FORCEINLINE Entity(EntityID handle, Registry* registry) : _Handle(handle), _TargetRegistry(registry) {
			//ASSERT(_TargetRegistry && _TargetRegistry->IsEntityActive(handle) && "nullptr registry or invalid entity!");			
		}

		FORCEINLINE operator EntityID() const { return _Handle; }

		FORCEINLINE void Destroy() {
			_TargetRegistry->Destroy(_Handle);
		}

		template<typename T, typename... Args>
		FORCEINLINE void AddComponent(Args&&... args) {
			return _TargetRegistry->AddComponent<T>(_Handle, std::forward<Args>(args)...);
		}

		FORCEINLINE void AddComponent(const char* typeName) {
			_TargetRegistry->AddComponent(_Handle, typeName);
		}

		template<typename T>
		FORCEINLINE void RemoveComponent() {
			_TargetRegistry->RemoveComponent<T>(_Handle);
		}

		FORCEINLINE void RemoveComponent(std::string typeName) {
			_TargetRegistry->RemoveComponent(_Handle, typeName);
		}

		FORCEINLINE void RemoveComponent(RuntimeType type) {
			_TargetRegistry->RemoveComponent(_Handle, type);
		}

		template<typename T>
		FORCEINLINE T& GetComponent() {
			return _TargetRegistry->GetComponent<T>(_Handle);
		}

		template<typename T>
		FORCEINLINE bool HasComponent() const {
			return _TargetRegistry->HasComponent<T>(_Handle);
		}

		FORCEINLINE EntityID GetHandle() const {
			return _Handle;
		}

		FORCEINLINE bool IsValid() const {
			return (_Handle != INVALID_HANDLE && _TargetRegistry != nullptr);
		}

	private:
		EntityID _Handle = INVALID_HANDLE;
		Registry* _TargetRegistry = nullptr;
	};

	using EntityList = std::vector<Entity>;
}