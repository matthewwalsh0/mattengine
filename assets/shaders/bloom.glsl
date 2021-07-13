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
uniform sampler2D u_TextureBloom;

in vec2 v_TexturePosition;

void main() {
	vec4 textureColour = texture(u_Texture, v_TexturePosition);
	vec4 bloomColour = texture(u_TextureBloom, v_TexturePosition);
	color = textureColour + bloomColour;
}