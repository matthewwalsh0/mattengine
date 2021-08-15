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

uniform sampler2D u_Texture;
uniform float u_Exposure;

in vec2 v_TexturePosition;

void main() {
	vec3 hdrColour = texture(u_Texture, v_TexturePosition).rgb;
	vec3 ldrColour = vec3(1.0) - exp(-hdrColour * u_Exposure);

	color = vec4(ldrColour, 1.0);
}