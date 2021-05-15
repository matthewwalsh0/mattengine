#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexturePosition;

uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_LightSpaceMatrix;

out vec3 v_FragmentPosition;
out vec4 v_FragmentPositionLightSpace;
out vec3 v_Position;
out vec3 v_Normal;
out mat4 v_Model;
out vec2 v_TexturePosition;

void main() {
	v_Position = a_Position;
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	v_Model = u_Model;
	v_TexturePosition = a_TexturePosition;
	v_FragmentPosition = vec3(u_Model * vec4(a_Position, 1.0));
	v_FragmentPositionLightSpace =
		u_LightSpaceMatrix * vec4(v_FragmentPosition, 1.0);

	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

#separator

#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_Colour;
uniform vec3 u_LightPosition = vec3(1.0, 1.0, 1.0);
uniform vec3 u_LightColour = vec3(1.0, 1.0, 1.0);
uniform bool u_IsLight = false;
uniform int u_TileCount = 1;
uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;
uniform vec3 u_ViewPosition;

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_FragmentPosition;
in vec4 v_FragmentPositionLightSpace;
in mat4 v_Model;
in vec2 v_TexturePosition;

void main() {
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);

	vec3 normal = normalize(v_Normal);
	vec3 lightDirection = normalize(u_LightPosition - v_FragmentPosition);
	float diffuseStrength = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * u_LightColour;

	float specularIntensity = 0.5;
	float shininess = 32;
	vec3 viewDirection = normalize(u_ViewPosition - v_FragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularStrength =
		pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
	vec3 specular = specularIntensity * specularStrength * u_LightColour;

	vec3 ligthSpaceProjected =
		v_FragmentPositionLightSpace.xyz / v_FragmentPositionLightSpace.w;
	ligthSpaceProjected = ligthSpaceProjected * 0.5 + 0.5;

	float closestDepth = texture(u_ShadowMap, ligthSpaceProjected.xy).r;
	float currentDepth = ligthSpaceProjected.z;
	float shadow =
		currentDepth < 1.0 && currentDepth - 0.05 > closestDepth ? 1.0 : 0.0;

	vec4 textureColour = texture(u_Texture, v_TexturePosition * u_TileCount);
	vec3 texturedColour = vec3(textureColour) * u_Colour;
	vec3 finalColour = u_IsLight
						   ? u_Colour
						   : (ambient + (1.0 - shadow) * (diffuse + specular)) *
								 texturedColour;

	color = vec4(finalColour, textureColour.a);
}