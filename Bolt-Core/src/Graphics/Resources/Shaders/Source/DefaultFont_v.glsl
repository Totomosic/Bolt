R"(
#version 430 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec4 in_Color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_TexTransforms[1];

out vec2 f_TexCoord;
out vec4 f_Color;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0);

	f_TexCoord = (u_TexTransforms[0] * vec3(in_TexCoord, 1.0)).xy;
	f_Color = in_Color;
}
)"