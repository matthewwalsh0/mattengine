#version 330 core

layout(location = 0) in vec3 a_PositionWorld;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
	gl_Position = u_Projection * u_View * vec4(a_PositionWorld, 1.0);
}

#separator

#version 330 core

uniform vec3 u_Colour;

layout(location = 0) out vec4 color;

void main() { color = vec4(u_Colour, 1.0); }