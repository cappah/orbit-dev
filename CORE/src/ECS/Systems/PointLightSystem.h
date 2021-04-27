#pragma once

#include "ECS/ISystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/PointLight.h"

namespace Orbit::ECS {
	class PointLightSystem : public ISystem {
	public:
		FORCEINLINE ~PointLightSystem() = default;

		FORCEINLINE PointLightSystem() = default;

		FORCEINLINE void Init() {
			_PbrShader = GLShader(_Loader->GetShader("PBR").Program);
			_DepthShader = GLShader(_Loader->GetShader("DEPTH").Program);
		}

		FORCEINLINE void Start() { }

		FORCEINLINE void Update() {
			auto entities = _Scene->GetEntityList<Transform, PointLight>();		

			_PbrShader.Bind();
			_PbrShader.Set1i("u_PointLightCount", entities.size());

			for (uint32 i = 0; i < entities.size(); ++i) {
				auto& light = entities[i].GetComponent<PointLight>();
				auto& transform = entities[i].GetComponent<Transform>();

				// set light shadow
				if (light.CastShadow) {
					matrix4 projection = light.Projection.GetMatrix(transform.Position);

					_DepthShader.Bind();
					_DepthShader.SetMat4f("u_LightSpaceMatrix", VAL_PTR(projection));

					_PbrShader.Bind();
					_PbrShader.SetMat4f("u_LightSpaceMatrix", VAL_PTR(projection));
				}

				_PbrShader.Set1f(Attribute("Intensity", i).c_str(), light.Intensity);
				_PbrShader.SetV3f(Attribute("Radiance", i).c_str(), light.Radiance);
				_PbrShader.SetV3f(Attribute("Position", i).c_str(), transform.Position);
			}		
		}

	private:
		FORCEINLINE std::string Attribute(const char* name, int index) {
			std::string uName("u_PointLights");
			uName.append("[" + std::to_string(index) + "]." + name);
			return uName;
		}

	private:
		GLShader _PbrShader;
		GLShader _DepthShader;
	};
}

