#version 330 core

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexturePosition;
layout(location = 3) in ivec4 a_BoneIds;
layout(location = 4) in vec4 a_BoneWeights;

uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_BoneTransforms[MAX_BONES];

out vec3 v_PositionWorld;
out vec3 v_PositionClip;
out vec3 v_Normal;
out mat4 v_Model;
out vec2 v_TexturePosition;

vec3 applyBoneTransforms(vec3 position) {
	vec4 finalPosition = vec4(0.0f);

	if (a_BoneIds == ivec4(-1, -1, -1, -1))
		return position;

	for (int boneIndex = 0; boneIndex < MAX_BONE_INFLUENCE; boneIndex++) {

		if (a_BoneIds[boneIndex] == -1) {
			continue;
		}

		if (a_BoneIds[boneIndex] > MAX_BONES) {
			finalPosition = vec4(position, 1.0f);
			break;
		}

		vec4 localPosition =
			u_BoneTransforms[a_BoneIds[boneIndex]] * vec4(position, 1.0f);

		finalPosition += localPosition * a_BoneWeights[boneIndex];
	}

	return vec3(finalPosition);
}

void main() {
	vec3 positionLocal = applyBoneTransforms(a_Position);
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	v_Model = u_Model;
	v_TexturePosition = a_TexturePosition;
	v_PositionWorld = vec3(u_Model * vec4(positionLocal, 1.0));

	gl_Position = u_Projection * u_View * u_Model * vec4(positionLocal, 1.0);
	v_PositionClip = vec3(gl_Position);
}

#separator

#version 330 core

const float AMBIENT_INTENSITY = 0.1;
const float SPECULAR_INTENSITY = 0.5;
const float SPECULAR_SHININESS = 32;
const int DEPTH_MAP_COUNT = 4;
const float SHADOW_BIAS = 0.05;
const float SHADOW_COLOUR = 0.2;

layout(location = 0) out vec4 color;

uniform vec3 u_Colour;
uniform vec3 u_LightPosition;
uniform vec3 u_LightColour;
uniform bool u_IsLight = false;
uniform int u_TileCount = 1;
uniform vec3 u_ViewPosition;
uniform sampler2D u_Texture;
uniform sampler2D u_DepthMap[DEPTH_MAP_COUNT];
uniform vec3 u_DepthMapFarPlane[DEPTH_MAP_COUNT];
uniform mat4 u_LightSpaceMatrix[DEPTH_MAP_COUNT];

in vec3 v_Normal;
in vec3 v_PositionWorld;
in mat4 v_Model;
in vec2 v_TexturePosition;
in vec3 v_PositionClip;

vec3 calculateLight() {
	vec3 normal = normalize(v_Normal);
	vec3 lightDirection = normalize(u_LightPosition - v_PositionWorld);

	// Ambient
	vec3 ambient = AMBIENT_INTENSITY * vec3(1.0, 1.0, 1.0);

	// Diffuse
	float diffuseStrength = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * u_LightColour;

	// Specular
	vec3 viewDirection = normalize(u_ViewPosition - v_PositionWorld);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularStrength =
		pow(max(dot(viewDirection, reflectDirection), 0.0), SPECULAR_SHININESS);
	vec3 specular = SPECULAR_INTENSITY * specularStrength * u_LightColour;

	return ambient + diffuse + specular;
}

vec3 calculateShadow() {
	int depthMapIndex = v_PositionClip.z <= u_DepthMapFarPlane[0].z	  ? 0
						: v_PositionClip.z <= u_DepthMapFarPlane[1].z ? 1
						: v_PositionClip.z <= u_DepthMapFarPlane[2].z ? 2
																	  : 3;

	vec4 positionLightSpace =
		u_LightSpaceMatrix[depthMapIndex] * vec4(v_PositionWorld, 1.0);

	vec4 lightSpaceProjected = positionLightSpace / positionLightSpace.w;
	lightSpaceProjected = lightSpaceProjected * 0.5 + 0.5;

	float closestDepth =
		texture(u_DepthMap[depthMapIndex], lightSpaceProjected.xy).r;

	float currentDepth = lightSpaceProjected.z;

	float shadow =
		currentDepth < 1.0 && currentDepth - SHADOW_BIAS > closestDepth
			? SHADOW_COLOUR
			: 1.0;

	return vec3(shadow, shadow, shadow);
}

void main() {
	vec3 finalColour = u_Colour;
	float alpha = 1.0;

	if (!u_IsLight) {
		vec3 lightFactor = calculateLight();
		vec3 shadowFactor = calculateShadow();

		vec4 textureColour =
			texture(u_Texture, v_TexturePosition * u_TileCount);

		finalColour =
			shadowFactor * lightFactor * vec3(textureColour) * u_Colour;

		alpha = textureColour.a;
	}

	color = vec4(finalColour, alpha);
}