R"(
#version 430 core
#include "BoltLighting.glh"

in vec2 f_TexCoord;
in vec3 f_WorldPosition;
in vec3 f_WorldCameraPosition;
in vec3 f_WorldNormal;
in vec4 f_Color;

uniform LightSource Lights[MAX_LIGHTS];
uniform int u_UsedLights;
uniform Material2D Material;

layout(location = 0) out vec4 FinalColor;

void main()
{
	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);
	for (int i = 0; i < u_UsedLights; i++)
	{
		diffuse += CalculateDiffuseLighting(Material, Lights[i], f_WorldNormal, Lights[i].Position - f_WorldPosition);
		specular += CalculateSpecularLighting(Material, Lights[i], f_WorldNormal, Lights[i].Position - f_WorldPosition, f_WorldCameraPosition - f_WorldPosition);
	}
	FinalColor = f_Color * vec4(diffuse.xyz + specular.xyz, diffuse.a) * texture(Material.Textures[0], f_TexCoord);	
}
)"