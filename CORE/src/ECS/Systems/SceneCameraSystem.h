#pragma once

#include "ECS/ISystem.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Transform.h"

namespace Orbit::ECS {
	class SceneCameraSystem : public ISystem {
	public:
		FORCEINLINE ~SceneCameraSystem() = default;

		FORCEINLINE SceneCameraSystem() = default;

		FORCEINLINE void Init() {
			_Shaders.push_back(GLShader(_Loader->GetShader("PBR").Program));
			_Shaders.push_back(GLShader(_Loader->GetShader("FLAT").Program));
			_Shaders.push_back(GLShader(_Loader->GetShader("SKYBOX").Program));
		}

		FORCEINLINE void Start() {
			
		}

		FORCEINLINE void Update() {
			auto cameras = _Scene->GetEntityList<Transform, Camera>();

			for (auto& entity : cameras) {
				auto& camera = entity.GetComponent<Camera>();

				if (camera.IsPrimary) {
					auto& transform = entity.GetComponent<Transform>();
					camera.Handle.Location = transform.Position;
					camera.Handle.Orientation = transform.Rotation;

					for (auto& shader : _Shaders) {
						camera.Handle.SetUniform(shader);
					}
					
					return;
				}
			}
		}

	private:
		FORCEINLINE void OnEntityDestroyed(const EntityDestroyedEvent& e) {
		
		}

	private:
		std::vector<GLShader> _Shaders;
	};
}