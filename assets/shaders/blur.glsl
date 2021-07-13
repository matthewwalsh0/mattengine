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
uniform bool u_Horizontal = true;

uniform float u_Weights[5] =
	float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

in vec2 v_TexturePosition;

void main() {
	vec2 texelSize = 1.0 / textureSize(u_Texture, 0);
	vec3 result = texture(u_Texture, v_TexturePosition).rgb * u_Weights[0];

	for (int offsetIndex = 1; offsetIndex < 5; ++offsetIndex) {
		vec2 offset = u_Horizontal ? vec2(texelSize.x * offsetIndex, 0.0)
								   : vec2(0.0, texelSize.y * offsetIndex);
		float weight = u_Weights[offsetIndex];

		result += texture(u_Texture, v_TexturePosition + offset).rgb * weight;
		result += texture(u_Texture, v_TexturePosition - offset).rgb * weight;
	}

	color = vec4(result, 1.0);
}