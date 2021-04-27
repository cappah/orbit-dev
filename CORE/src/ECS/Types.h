#pragma once

#include "Core/RuntimeType.h"

namespace Orbit::ECS {

	using EntityID = uint32;
	using DataIndex = uint32;
	using EntityIdList = std::vector<EntityID>;	

	const EntityID INVALID_HANDLE = 0;

	// COMPONENT ARRAY

	// stores component with target entity
	template<typename CompType>
	struct EntityToComponent {
		FORCEINLINE EntityToComponent() = default;

		FORCEINLINE ~EntityToComponent() = default;

		FORCEINLINE EntityToComponent(const EntityToComponent& o) :
			EntityHandle(o.EntityHandle), Component(o.Component){}

		FORCEINLINE EntityToComponent(EntityID handle, const CompType& component) :
			EntityHandle(handle), Component(component) { }

		FORCEINLINE operator EntityID() const { return EntityHandle; }

		CompType Component;
		EntityID EntityHandle = INVALID_HANDLE;
	};

	// abstract array
	class IComponentList {
	public:
		virtual ~IComponentList() = default;

		FORCEINLINE IComponentList() = default;

		FORCEINLINE IComponentList(RuntimeType typeId, std::string typeName):
			_TypeID(typeId), _TypeName(typeName) { }

		virtual void Clear() = 0;
		virtual bool IsEmpty() = 0;
		virtual void Push(EntityID handle) = 0;
		virtual void Erase(EntityID handle) = 0;
		virtual void* GetHandle(EntityID handle) = 0;

		FORCEINLINE RuntimeType DataType() { return _TypeID; }

		FORCEINLINE std::string DataTypeName() { return _TypeName; }

	protected:
		RuntimeType _TypeID = INVALID_RUNTIME_TYPE;
		std::string _TypeName = EMPTY_STR;
	};

	// component array
	template<typename T>
	class ComponentList : public IComponentList {
	public:
		FORCEINLINE ComponentList(std::string typeName = EMPTY_STR) : 
			IComponentList(TypeOf<T>(), typeName) { }

		FORCEINLINE ~ComponentList() { Clear(); }

		FORCEINLINE void Push(EntityID handle, const T& component) {
			if (!this->Exits(handle)) {
				_Data.push_back(EntityToComponent(handle, component));
				return;
			}
			OB_ERROR("entity allready has component!");
		}

		FORCEINLINE T& Get(EntityID handle) {
			ASSERT(this->Exits(handle) && "component out of range!");
			return (*std::find(_Data.begin(), _Data.end(), handle)).Component;			
		}

		FORCEINLINE void* GetHandle(EntityID handle) {
			ASSERT(this->Exits(handle) && "component out of range!");
			return &((*std::find(_Data.begin(), _Data.end(), handle)).Component);
		}

		FORCEINLINE void Push(EntityID handle) {
			this->Push(handle, T());
		}

		FORCEINLINE void Erase(EntityID handle) {	
			auto it = std::find(_Data.begin(), _Data.end(), handle);
			if (it != _Data.end()) { _Data.erase(it); }
		}		

		FORCEINLINE bool Exits(EntityID handle) {	
			auto it = std::find(_Data.begin(), _Data.end(), handle);
			if (it != _Data.end()) { return true; }
			return false;
		}

		FORCEINLINE bool IsEmpty() {
			return _Data.empty();
		}

		FORCEINLINE void Clear() {
			_Data.clear();
		}

	private:
		std::vector<EntityToComponent<T>> _Data;
	};

	class EntitySignature {
	public:
		FORCEINLINE EntitySignature() : _ComponentSignatures({}) { }

		FORCEINLINE operator const std::set<RuntimeType>() const { return _ComponentSignatures; }

		FORCEINLINE ~EntitySignature() { 
			_ComponentSignatures.clear();
		}

		template<typename T>
		FORCEINLINE void AddComponentSignature() { 
			_ComponentSignatures.insert(TypeOf<T>());
		}

		FORCEINLINE void AddComponentSignature(RuntimeType type) {
			_ComponentSignatures.insert(type);
		}

		template<typename T>
		FORCEINLINE void RemoveComponentSignature() {
			_ComponentSignatures.erase(TypeOf<T>()); 
		}

		FORCEINLINE void RemoveComponentSignature(RuntimeType type) {
			_ComponentSignatures.erase(type);
		}

		template<typename T>
		FORCEINLINE bool HasComponentSignature() const {
			return (_ComponentSignatures.count(TypeOf<T>()) > 0);
		}

		FORCEINLINE bool HasComponentSignature(RuntimeType type) const {
			return (_ComponentSignatures.count(type) > 0);
		}

		FORCEINLINE void Clear() { 
			_ComponentSignatures.clear();
		}

		FORCEINLINE const std::set<RuntimeType> GetData() {
			return _ComponentSignatures;
		}

	private:
		friend class Registry;
		std::set<RuntimeType> _ComponentSignatures;
	};

	// stores component with target entity	
	struct EntityToSignature {
		FORCEINLINE EntityToSignature() = default;

		FORCEINLINE ~EntityToSignature() = default;

		FORCEINLINE EntityToSignature(EntityID handle) : EntityHandle(handle) { }

		FORCEINLINE operator EntityID() const { return EntityHandle; }

		EntitySignature Signature;
		EntityID EntityHandle = INVALID_HANDLE;
	};
}