#pragma once

#include "ECS/ISystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/DirectionalLight.h"

namespace Orbit::ECS {
	class DirectionalLightSystem : public ISystem {
	public:
		FORCEINLINE ~DirectionalLightSystem() = default;
		FORCEINLINE DirectionalLightSystem() = default;

		FORCEINLINE void Init() {
			_PbrShader = GLShader(_Loader->GetShader("PBR").Program);
			_DepthShader = GLShader(_Loader->GetShader("DEPTH").Program);
		}

		FORCEINLINE void Start() { }

		FORCEINLINE void Update() {
			auto entities = _Scene->GetEntityList<DirectionalLight>();

			for (auto& entity : entities) {
				auto& transform = entity.GetComponent<Transform>();
				auto& light = entity.GetComponent<DirectionalLight>();

				// set light shadow
				if (light.CastShadow) {
					matrix4 projection = light.Projection.GetMatrix(transform.Rotation);
					_DepthShader.Bind();
					_DepthShader.SetMat4f("u_LightSpaceMatrix", VAL_PTR(projection));

					_PbrShader.Bind();
					_PbrShader.SetMat4f("u_LightSpaceMatrix", VAL_PTR(projection));
				}

				_PbrShader.Bind();
				_PbrShader.Set1f("u_DirLight.Intensity", light.Intensity);
				_PbrShader.SetV3f("u_DirLight.Radiance", light.Radiance);
				_PbrShader.SetV3f("u_DirLight.Direction", transform.Rotation);

				return;
			}
		}

	private:
		GLShader _PbrShader;
		GLShader _DepthShader;
	};
}

