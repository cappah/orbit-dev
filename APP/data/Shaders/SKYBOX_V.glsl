#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 a_Position;

layout(location = 0) out vec3 TexCoord;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	TexCoord = a_Position;
	vec4 position = u_Projection * mat4(mat3(u_View)) * vec4(a_Position, 1.0f);
	gl_Position = position.xyww;
}

