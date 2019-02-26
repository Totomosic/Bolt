R"(
#version 430 core

struct ClipPlane
{
    vec3 Normal;
    float Distance;
};

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec4 in_Color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_TexTransforms[1];

uniform ClipPlane u_ClippingPlane;

out vec2 f_TexCoord;
out vec3 f_WorldPosition;
out vec3 f_WorldCameraPosition;
out vec3 f_WorldNormal;
out vec4 f_Color;

void main()
{
	vec4 worldPosition = u_ModelMatrix * vec4(in_Position, 1.0);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPosition;
	gl_ClipDistance[0] = dot(u_ModelMatrix * vec4(in_Position, 1.0), vec4(u_ClippingPlane.Normal, u_ClippingPlane.Distance));

	f_TexCoord = (u_TexTransforms[0] * vec3(in_TexCoord, 1.0)).xy;
	f_WorldPosition = worldPosition.xyz;
	f_WorldCameraPosition = inverse(u_ViewMatrix)[3].xyz;
	f_WorldNormal = (u_ModelMatrix * vec4(in_Normal, 0.0)).xyz;
	f_Color = in_Color;
}
)"