#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Projection;

out vec3 v_FragmentPosition;
out vec3 v_Position;
out vec3 v_Normal;
out mat4 v_Model;

void main() {
	v_Position = a_Position;
	v_Normal = a_Normal;
	v_Model = u_Model;
	v_FragmentPosition = vec3(u_Model * vec4(a_Position, 1.0));

	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

---

#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_Colour;
uniform vec3 u_LightPosition = vec3(1.0, 1.0, 1.0);
uniform vec3 u_LightColour = vec3(1.0, 1.0, 1.0);
uniform bool u_IsLight = false;

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_FragmentPosition;
in mat4 v_Model;

void main() {
	vec3 normal = vec3(v_Model * vec4(v_Normal, 1.0));
	vec3 lightDirection = normalize(u_LightPosition - v_FragmentPosition);
	float diffuseFactor =
		u_IsLight ? 1.0 : max(dot(normal, lightDirection), 0.15);
	vec3 diffuseColour = diffuseFactor * u_LightColour;
	vec3 finalColour = diffuseColour * u_Colour;

	color = vec4(finalColour, 1.0);
}