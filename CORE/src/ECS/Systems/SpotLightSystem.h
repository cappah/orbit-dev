#pragma once

#include "ECS/ISystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/SpotLight.h"

namespace Orbit::ECS {
	class SpotLightSystem : public ISystem {
	public:
		FORCEINLINE void Init() {
			_PbrShader = GLShader(_Loader->GetShader("PBR").Program);
		}

		FORCEINLINE void Start() { }


		FORCEINLINE void Update() {
			auto entities = _Scene->GetEntityList<Transform, SpotLight>();

			_PbrShader.Bind();
			_PbrShader.Set1i("u_SpotLightCount", entities.size());

			for (uint32 i = 0; i < entities.size(); ++i) {
				auto& light = entities[i].GetComponent<SpotLight>();
				auto& transform = entities[i].GetComponent<Transform>();

				_PbrShader.Set1f(Attribute("Bias", i).c_str(), light.Bias);
				_PbrShader.Set1f(Attribute("Intensity", i).c_str(), light.Intensity);

				_PbrShader.Set1f(Attribute("InnerCutOff", i).c_str(), Maths::Cos(Maths::Radians(light.InnerCutOff)));
				_PbrShader.Set1f(Attribute("OuterCutOff", i).c_str(), Maths::Cos(Maths::Radians(light.OuterCutOff)));

				_PbrShader.SetV3f(Attribute("Radiance", i).c_str(), light.Radiance);
				_PbrShader.SetV3f(Attribute("Position", i).c_str(), transform.Position);
				_PbrShader.SetV3f(Attribute("Direction", i).c_str(), transform.Rotation);
			}
		}

	private:
		FORCEINLINE std::string Attribute(const char* name, int index) {
			std::string uName("u_SpotLights[");
			uName.append(std::to_string(index));
			uName.append("].");
			uName.append(name);
			return uName;
		}

	private:
		GLShader _PbrShader;
	};
}

