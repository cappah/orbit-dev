#version 330 core

in vec3 TexCoord;
out vec4 FragColor;
uniform samplerCube u_SkyBoxCubeMap;

void main() {
	vec3 texel = pow(texture(u_SkyBoxCubeMap, normalize(TexCoord)).rgb, vec3(2.2));
	
    // HDR tonemapping
    texel = texel / (texel + vec3(1.0));
    // gamma correct
    texel = pow(texel, vec3(1.0/2.2)); 
	// output color
	FragColor = vec4(texel, 1.0);  
}

