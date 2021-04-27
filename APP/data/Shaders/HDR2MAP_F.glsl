#version 330 core

out vec4 FragColor;
in vec3 WorldPosition;

uniform sampler2D u_EquirectangularMap;
const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 direction) {
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {		
    vec2 uv = SampleSphericalMap(normalize(WorldPosition)); 
    FragColor = vec4(texture(u_EquirectangularMap, uv).rgb, 1.0);
}