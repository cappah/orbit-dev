#version 330 core
#extension GL_ARB_texture_query_levels : enable

const float LINEAR = 0.1f;
const float CONSTANT = 1.0f;
const float QUADRATIC = 0.02f;
const float EPSILON = 0.00001;
const float PI = 3.14159265359;
const int MAX_LIGHT_COUNT = 10;
const vec3 BASE_REF = vec3(0.04);

// point light
struct PointLight  {
    vec3 Radiance;
    vec3 Position;
	
	float Bias;
	float Intensity;
};

// dir light
struct DirLight {
	vec3 Radiance;
    vec3 Direction;
	
	float Bias;
	float Intensity;
};

// spot light
struct SpotLight {
	vec3 Radiance;
    vec3 Position;
    vec3 Direction;
	
	float Bias;
	float Intensity;
	float InnerCutOff;
	float OuterCutOff;
};

// vertex
in Vertex {
	mat3 TBN;
	vec3 Normal;
	vec2 TexCoords;
	vec3 FragPosition;
	vec4 LightSpacePosition;
} Vin;

out vec4 FragColor;

// material props
uniform float u_Ao;
uniform vec3  u_Albedo;
uniform float u_Opacity;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Shininess;
uniform float u_Reflectivity;
uniform bool u_HasNormalMap;

// material maps
uniform sampler2D u_AoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_SpecularBRDFMap;

// environment maps
uniform sampler2D u_ShadowMap; 
uniform samplerCube u_IrradianceCubeMap;
uniform samplerCube u_SpecularIrradianceCubeMap;

// camera position
uniform vec3 u_CameraPosition;

// dir light
uniform DirLight u_DirLight;

// point lights
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_LIGHT_COUNT];

// spot lights
uniform int u_SpotLightCount;
uniform SpotLight u_SpotLights[MAX_LIGHT_COUNT];

// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------

// compute shadow
float ComputeShadow(vec4 fragPosLightSpace, float LdotN) {
   // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - LdotN), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

// compute point lights
vec3 ComputePointlights(vec3 N, vec3 V, vec3 F0, vec3 albedo, float metallic, float roughness) {
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < u_PointLightCount; ++i) {
		PointLight light = u_PointLights[i];
		vec3 L = normalize(light.Position - Vin.FragPosition);
		float distance = length(light.Position - Vin.FragPosition);
		float attenuation = light.Intensity /  (distance * distance);

		vec3 H = normalize(V + L);
		float NDF = DistributionGGX(N, H, roughness);   
		float G   = GeometrySmith(N, V, L, roughness);      
		vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
			
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); 
		vec3 kD = vec3(1.0) - F;
		kD *= 1.0 - metallic;	  

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);        
		Lo += (kD * albedo / PI + specular) * light.Radiance * attenuation * NdotL; 
		Lo *= (1 - ComputeShadow(Vin.LightSpacePosition, dot(L, N)));
	}
	return Lo;
}

// compute directional light
vec3 ComputeDirlight(vec3 N, vec3 V, vec3 F0, vec3 albedo, float metallic, float roughness) {
	vec3 L = -normalize(u_DirLight.Direction);
	vec3 H = normalize(V + L);

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);   
	float G   = GeometrySmith(N, V, L, roughness);      
	vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
		
	vec3 nominator    = NDF * G * F; 
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = nominator / max(denominator, 0.001); 
	vec3 kD = vec3(1.0) - F;
	kD *= 1.0 - metallic;	  

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);        
	vec3 Lo = (kD * albedo / PI + specular) * u_DirLight.Radiance * u_DirLight.Intensity * NdotL;     
    Lo *= (1 - ComputeShadow(Vin.LightSpacePosition, dot(L, N)));   
	return Lo;
}

// compute spot lights
vec3 ComputeSpotlights(vec3 N, vec3 V, vec3 F0, vec3 albedo, float metallic, float roughness) {
	vec3 Lo = vec3(0.0);

	for(int i = 0; i < u_SpotLightCount; ++i) {
		SpotLight light = u_SpotLights[i];
		vec3 L = normalize(light.Position - Vin.FragPosition);
		float theta = dot(L, normalize(-light.Direction));
		float epsilon = (light.OuterCutOff - light.InnerCutOff);
		float spotFactor = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

		// attenuation
		float distance = length(light.Position - Vin.FragPosition);
		float attenuation = light.Intensity /  (CONSTANT + LINEAR * distance + QUADRATIC * (distance * distance));

		// Cook-Torrance BRDF
		vec3 H = normalize(V + L);
		float NDF = DistributionGGX(N, H, roughness);   
		float G   = GeometrySmith(N, V, L, roughness);      
		vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
			
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001);
		
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= (1.0 - metallic);	  

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);        
		Lo += (kD * albedo / PI + specular) * light.Radiance * NdotL * attenuation * spotFactor; 
		Lo *= (1 - ComputeShadow(Vin.LightSpacePosition, dot(L, N)));   
	}
	return Lo;
}

// compute ambient light IBL
vec3 ComputeAmbientLight(vec3 N, vec3 V, vec3 F0, vec3 albedo, float metallic, float roughness, float ao) {
	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, V));
	
	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * N - V;

	// Sample diffuse irradiance at normal direction.
	vec3 irradiance = texture(u_IrradianceCubeMap, N).rgb;
	vec3 F = fresnelSchlick(cosLo, F0);

	// Get diffuse contribution factor (as with direct lighting).
	vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metallic);

	// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
	vec3 diffuseIBL = kd * albedo * irradiance;

	// Sample pre-filtered specular reflection environment at correct mipmap level.
	int specularTextureLevels = textureQueryLevels(u_SpecularIrradianceCubeMap);
	vec3 specularIrradiance = textureLod(u_SpecularIrradianceCubeMap, Lr, roughness * specularTextureLevels).rgb;

	// Split-sum approximation factors for Cook-Torrance specular BRDF.
	vec2 specularBRDF = texture(u_SpecularBRDFMap, vec2(cosLo, roughness)).rg;

	// Total specular IBL contribution.
	vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

	// Total ambient lighting contribution.
	vec3 ambient = (diffuseIBL + specularIBL) * ao;
	return ambient;
}

void main() {	
	float ao = u_Ao + texture(u_AoMap, Vin.TexCoords).r;
	float metallic = u_Metallic + texture(u_MetallicMap, Vin.TexCoords).r;
	float roughness = u_Roughness + texture(u_RoughnessMap, Vin.TexCoords).r;
	vec3 albedo = u_Albedo + pow(texture(u_AlbedoMap, Vin.TexCoords).rgb, vec3(2.2));

	vec3 N = normalize(Vin.Normal);
    vec3 V = normalize(u_CameraPosition - Vin.FragPosition);
	if(u_HasNormalMap) { N = texture(u_NormalMap, Vin.TexCoords).rgb; }	
   
    vec3 F0 = mix(BASE_REF, albedo, metallic);
    vec3 Lo = vec3(0.0);

	// DIR LIGHT
	Lo += ComputeDirlight(N, V, F0, albedo, metallic, roughness);
	// POINT LIGHTS
	Lo += ComputePointlights(N, V, F0, albedo, metallic, roughness);
	// SPOT LIGHTS
	Lo += ComputeSpotlights(N, V, F0, albedo, metallic, roughness);
	// AMBIENT LIGHT
    Lo += ComputeAmbientLight(N, V, F0, albedo, metallic, roughness, ao);
	
    // HDR tonemapping
    Lo = Lo / (Lo + vec3(1.0));
    // gamma correct
    Lo = pow(Lo, vec3(1.0/2.2)); 
	// output color
    FragColor = vec4(Lo, u_Opacity);
}