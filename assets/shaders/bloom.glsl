#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexturePosition;

out vec2 v_TexturePosition;

void main() {
	v_TexturePosition = a_TexturePosition;
	gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}

#separator

#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture_1;
uniform sampler2D u_Texture_2;

in vec2 v_TexturePosition;

void main() {
	vec3 value_1 = texture(u_Texture_1, v_TexturePosition).rgb;
	vec3 value_2 = texture(u_Texture_2, v_TexturePosition).rgb;
	vec3 total = value_1 + value_2;

	color = vec4(total, 1.0);
}