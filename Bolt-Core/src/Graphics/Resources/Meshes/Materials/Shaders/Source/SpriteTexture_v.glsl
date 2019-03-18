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
layout(location = 4) in float in_TID;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_TexTransforms[16];

uniform ClipPlane u_ClippingPlane;

out vec2 f_TexCoord;
out vec4 f_Color;
out float f_TID;
out vec3 f_WorldPosition;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0);
	gl_ClipDistance[0] = dot(u_ModelMatrix * vec4(in_Position, 1.0), vec4(u_ClippingPlane.Normal, u_ClippingPlane.Distance));

	f_TexCoord = in_TexCoord;//(u_TexTransforms[in_TID] * vec3(in_TexCoord, 1.0)).xy;
	f_Color = in_Color;
	f_TID = in_TID;
	f_WorldPosition = (u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0)).xyz;
}
)"