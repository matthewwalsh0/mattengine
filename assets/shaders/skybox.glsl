#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 v_TexturePosition;

void main() {
	v_TexturePosition = a_Position;
	gl_Position = u_Projection * mat4(mat3(u_View)) * vec4(a_Position, 1.0);
}

---

#version 330 core

  out vec4 f_Colour;

in vec3 v_TexturePosition;

uniform samplerCube u_Texture;

void main() {
	float skyboxBrightness = 0.3;
	f_Colour = texture(u_Texture, v_TexturePosition) * skyboxBrightness;
}