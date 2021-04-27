#pragma once

#include "ECS/Entity.h"
#include "SceneSerializer.h"
#include "Resource/Manager.h"
#include "Core/PlatformRenderer.h"
#include "Events/Categories/KeyEvents.h"
#include "Events/Categories/SceneEvents.h"
#include "Events/Categories/EditorEvents.h"

namespace Orbit {	
	class Scene {
	public:
		void Init();
		void Start();
		void Update();

		// -----------

		template<typename T>
		FORCEINLINE ECS::EntityList GetEntityList() {
			ECS::EntitySignature signature;
			signature.AddComponentSignature<T>();
			return GetEntityList(signature);
		}

		template<typename T1, typename T2>
		FORCEINLINE ECS::EntityList GetEntityList() {
			STATIC_ASSERT(!std::is_same<T1, T2>::value && "repeated template type not alowed!");
			ECS::EntitySignature signature;
			signature.AddComponentSignature<T1>();
			signature.AddComponentSignature<T2>();
			return GetEntityList(signature);
		}

		template<typename T1, typename T2, typename T3>
		FORCEINLINE ECS::EntityList GetEntityList() {
			STATIC_ASSERT(!std::is_same<T1, T2>::value
				&& !std::is_same<T2, T3>::value
				&& !std::is_same<T1, T3>::value
				&& "repeated template type not alowed!");

			ECS::EntitySignature signature;
			signature.AddComponentSignature<T1>();
			signature.AddComponentSignature<T2>();
			signature.AddComponentSignature<T3>();
			signature GetEntityList(signature);
		}

		// -----------

		FORCEINLINE ResourceManager* Loader() { return &_Loader; }

		FORCEINLINE ECS::Registry* Registry() { return _Registry.get(); }

		FORCEINLINE PlatformRenderer* Renderer() { return _Renderer.get(); }

		// -----------

		FORCEINLINE ECS::Entity ToEntity(ECS::EntityID handle) {
			ASSERT(_Registry->IsEntityActive(handle) && "Entity out of range!");
			return ECS::Entity(handle, _Registry.get());
		}

		ECS::EntityList GetEntityList(const ECS::EntitySignature& filter);

		ECS::Entity AddEntity(std::string name = EMPTY_STR, const vec3f& pos = vec3f(0));

		void DestroyEntity(ECS::EntityID handle);

	private:
		void OnOpenScene(const OpenSceneEvent& e);
		void OnKeyPressed(const KeyPressedEvent& e);
		void OnSceneResized(const SceneResizedEvent& e);

		void OnAddEntity(const AddEntityEvent& e);
		void OnDestroyEntity(const DestroyEntityEvent& e);

		void OnAddComponent(const AddComponentEvent& e);
		void OnRemoveComponent(const RemoveComponentEvent& e);

	private:
		ResourceManager _Loader;
		Unique<ECS::Registry> _Registry;
		Unique<PlatformRenderer> _Renderer;
		Unique<SceneSerializer> _Serializer;
	};
}
