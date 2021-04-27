#pragma once

#include "Maths/Pespective.h"

namespace Orbit::ECS {
	struct SpotLight {
		FORCEINLINE SpotLight() = default;
		FORCEINLINE ~SpotLight() = default;
		FORCEINLINE SpotLight(const SpotLight&) = default;

		Pespective Projection;
		bool CastShadow = true;

		float Bias = 0.5f;
		float Intensity = 500.0f;
		float InnerCutOff = 55.0f;
		float OuterCutOff = 140.5f;
		vec3f Radiance = vec3f(1.0f);
	};
}