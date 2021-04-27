#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

out Vertex {
	mat3 TBN;
	vec3 Normal;
	vec2 TexCoords;
	vec3 FragPosition;
	vec4 LightSpacePosition;
} Vout;

uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_LightSpaceMatrix;

void main() {		
	Vout.TexCoords = a_TexCoords;
	Vout.FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
	Vout.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	Vout.TBN = mat3(u_Model) * mat3(a_Tangent, a_Bitangent, a_Normal);
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    Vout.LightSpacePosition = u_LightSpaceMatrix * vec4(Vout.FragPosition, 1.0);
}
