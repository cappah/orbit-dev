#pragma once

#include "types.h"

namespace Orbit::ECS {
	class Registry {
	public:
		FORCEINLINE Registry() = default;

		FORCEINLINE ~Registry() {
			this->Clear(); 
			_ComponentArrays.clear();
		}

		FORCEINLINE void Clear() {
			_EntitySignatures.clear();
			_UnActiveEntityHandle.clear();			
			_EntitySignatures.shrink_to_fit();
			for (auto& cArray : _ComponentArrays) {  cArray->Clear(); }
		}

		// add new entity in the registry
		FORCEINLINE EntityID AddEntity() {
			EntityID handle = INVALID_HANDLE;

			if(_UnActiveEntityHandle.empty()) {
				handle = (EntityID)_EntitySignatures.size() + 1;
			}
			else {
				handle = *_UnActiveEntityHandle.begin();
				_UnActiveEntityHandle.erase(handle);
			}

			_EntitySignatures.push_back(EntityToSignature(handle));
			return handle;
		}

		// destroys entity & it's components
		FORCEINLINE void Destroy(EntityID handle) {
			if (IsEntityActive(handle)) {
				for (auto&& cArray : _ComponentArrays) {
					cArray->Erase(handle);
				}		

				EraseEntitySignature(handle);
				_UnActiveEntityHandle.insert(handle);
			}
		}

		// destroys entity & it's components
		FORCEINLINE void DestroyAll() {
			for (auto& entity : _EntitySignatures) {
				this->Destroy(entity);
			}
		}

		// returns true if entity is not destroyed
		FORCEINLINE bool IsEntityActive(EntityID handle) {
			return (std::find(_EntitySignatures.begin(), _EntitySignatures.end(), handle) != _EntitySignatures.end());
		}

		// returns entity count
		FORCEINLINE uint32 EntityCount() const { return (EntityID)_EntitySignatures.size(); }

		// returns a group of entities with the given component types
		FORCEINLINE const std::vector<EntityToSignature> GetAllEntities() {		
			return _EntitySignatures;
		}

		// returns a group of entities with the given component types
		FORCEINLINE const EntityIdList GetEntityHandleList(const EntitySignature& signature) {
			EntityIdList entities;
			for (auto& entity : _EntitySignatures) {

				bool entity_has_signature = true;

				for (RuntimeType type : signature._ComponentSignatures) {
					if (!entity.Signature.HasComponentSignature(type)) {
						entity_has_signature = false;
						break;
					}
				}

				if (entity_has_signature) {
					entities.push_back(entity);
				}
			}
						
			return entities;
		}

		// ---------------------------------------------------------------------------------

		// adds component in compile time
		template<typename T, typename... Args>
		FORCEINLINE void AddComponent(EntityID handle, Args&&... args) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			this->RegisterComponent<T>();
			T component(std::forward<Args>(args)...);
			GetComponentArray<T>()->Push(handle, component);
			GetEntitySignature(handle).AddComponentSignature<T>();			
		}

		template<typename T>
		FORCEINLINE void AddComponent(EntityID handle, const T& component) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			this->RegisterComponent<T>();
			GetComponentArray<T>()->Push(handle, component);
			GetEntitySignature(handle).AddComponentSignature<T>();
		}

		// adds component in rutime
		FORCEINLINE void AddComponent(EntityID handle, std::string typeName) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			auto cArray = GetComponentArray(typeName);
			GetEntitySignature(handle).AddComponentSignature(cArray->DataType());
			cArray->Push(handle);
		}

		// removes component
		FORCEINLINE void RemoveComponent(EntityID handle, std::string typeName) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			auto cArray = GetComponentArray(typeName);
			GetEntitySignature(handle).RemoveComponentSignature(cArray->DataType());
			cArray->Erase(handle);
		}

		// removes component
		FORCEINLINE void RemoveComponent(EntityID handle, RuntimeType type) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			auto cArray = GetComponentArray(type);
			GetEntitySignature(handle).RemoveComponentSignature(cArray->DataType());
			cArray->Erase(handle);
		}

		template<typename T>
		FORCEINLINE void RemoveComponent(EntityID handle) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			GetEntitySignature(handle).RemoveComponentSignature<T>();
			GetComponentArray<T>()->Erase(entity);
		}

		// returns component of the entity specified by T
		template<typename T>
		FORCEINLINE T& GetComponent(EntityID handle) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			return GetComponentArray<T>()->Get(handle);
		}

		FORCEINLINE void* GetComponent(EntityID handle, std::string typeName) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			return GetComponentArray(typeName)->GetHandle(handle);
		}

		// returns true if has component runtime
		FORCEINLINE bool HasComponent(EntityID handle, RuntimeType type) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			return GetEntitySignature(handle).HasComponentSignature(type);
		}

		// returns true if has component
		template<typename T>
		FORCEINLINE bool HasComponent(EntityID handle) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			return GetEntitySignature(handle).HasComponentSignature<T>();
		}

		//---------------------------------------------------------------------------------------

		// register component array with typeName
		template<typename T>
		FORCEINLINE void RegisterComponent(std::string typeName) {
			ASSERT(!FindComponentArray<T>() && "component array allready registered!");			
			_ComponentArrays.push_back(std::move(MakeUnique<ComponentList<T>>(typeName)));			
		}

		// register component with template type
		template<typename T>
		FORCEINLINE void RegisterComponent() {
			if (!FindComponentArray<T>()) {
				 static const RuntimeType typeId = TypeOf<T>();
				_ComponentArrays.push_back(std::move(MakeUnique<ComponentList<T>>()));				 
			}
		}

	private:
		FORCEINLINE IComponentList* FindComponentArray(const RuntimeType type) {
			for (auto& cArray : _ComponentArrays) {
				if (cArray->DataType() == type) { return cArray.get(); }
			}
			return nullptr;
		}

		FORCEINLINE IComponentList* FindComponentArray(std::string typeName) {
			for (auto& cArray : _ComponentArrays) {
				if (cArray->DataTypeName() == typeName) {
					return cArray.get();
				}
			}
			return nullptr;
		}

		template<typename T>
		FORCEINLINE ComponentList<T>* FindComponentArray() {
			for (auto& cArray : _ComponentArrays) {
				if (cArray->DataType() == TypeOf<T>()) {
					return static_cast<ComponentList<T>*>(cArray.get());
				}
			}
			return nullptr;
		}

		// -------------------------------------------------------------------------------------

		FORCEINLINE IComponentList* GetComponentArray(const RuntimeType type) {
			auto cArray = FindComponentArray(type);
			ASSERT(cArray && "component array not found or not registered");
			return cArray;
		}

		FORCEINLINE IComponentList* GetComponentArray(std::string typeName) {
			auto cArray = FindComponentArray(typeName);
			ASSERT(cArray && "component array not found or not registered");
			return cArray;
		}

		template<typename T>
		FORCEINLINE ComponentList<T>* GetComponentArray() {
			auto cArray = FindComponentArray<T>();
			ASSERT(cArray && "component array not found or not registered");			
			return static_cast<ComponentList<T>*>(cArray);
		}

		// --------------------------------------------------------------------------------------

		FORCEINLINE EntitySignature& GetEntitySignature(EntityID handle) {
			ASSERT(IsEntityActive(handle) && "entity out of range!");
			auto it = std::find(_EntitySignatures.begin(), _EntitySignatures.end(), handle);
			ASSERT(it != _EntitySignatures.end() && "entity out of range!");
			return it->Signature;
		}

		FORCEINLINE void EraseEntitySignature(EntityID handle) {
			for (auto it = _EntitySignatures.begin(); it != _EntitySignatures.end(); it++) {
				if ((*it).EntityHandle == handle) {
					_EntitySignatures.erase(it);
					break;
				}
			}
		}

	private:
		std::set<EntityID> _UnActiveEntityHandle;
		std::vector<EntityToSignature> _EntitySignatures;
		std::vector<Unique<IComponentList>> _ComponentArrays;
	};
}