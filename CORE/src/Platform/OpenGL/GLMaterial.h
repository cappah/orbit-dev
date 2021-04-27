#pragma once

#include "GLShader.h"

namespace Orbit {
	struct GLMaterial {
		FORCEINLINE GLMaterial() = default;
		FORCEINLINE ~GLMaterial() = default;
		FORCEINLINE GLMaterial(const GLMaterial&) = default;

		float Ao = 0.1f;
		float Opacity = 1.0f;
		float Metallic = 0.4f;
		float Roughness = 0.2f;
		float Shininess = 1.0f;
		float Reflectivity = 0.0f;
		vec3f Albedo = vec3f(1.0f, 0.1f, 0.2f);

		uint32 AOMap = 0;
		uint32 AlbedoMap = 0;
		uint32 NormalMap = 0;
		uint32 MetallicMap = 0;
		uint32 RoughnessMap = 0;
		uint32 SpecularBRDFMap = 0;

		FORCEINLINE void SetUniform(GLShader& shader) {
			shader.Bind();
			shader.Set1f("u_Ao", Ao);
			shader.SetV3f("u_Albedo", Albedo);
			shader.Set1f("u_Opacity", Opacity);
			shader.Set1f("u_Metallic", Metallic);
			shader.Set1f("u_Roughness", Roughness);
			shader.Set1f("u_Shininess", Shininess);
			shader.Set1f("u_Reflectivity", Reflectivity);
			shader.Set1i("u_HasNormalMap", (NormalMap > 0) ? 1 : 0);

			shader.SetSample2D("u_AoMap", AOMap, 0);					
			shader.SetSample2D("u_AlbedoMap", AlbedoMap, 1);
			shader.SetSample2D("u_NormalMap", NormalMap, 2);
			shader.SetSample2D("u_RoughnessMap", RoughnessMap, 3);
			shader.SetSample2D("u_MetallicMap", MetallicMap, 4);
			shader.SetSample2D("u_SpecularBRDFMap", SpecularBRDFMap, 5);
		}
	};
}