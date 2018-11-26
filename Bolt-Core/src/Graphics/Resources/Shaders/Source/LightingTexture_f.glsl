R"(
#version 430 core
#include "BoltLighting.glh"

in vec2 f_TexCoord;
in vec3 f_WorldPosition;
in vec3 f_WorldNormal;
in vec4 f_Color;

uniform LightSource Lights[MAX_LIGHTS];
uniform int u_UsedLights;
uniform Material2D Material;

layout(location = 0) out vec4 FinalColor;

void main()
{
	vec4 diffuse = CalculateDiffuseLighting(Material, Lights[0], f_WorldNormal, Lights[0].Position - f_WorldPosition);
	FinalColor = f_Color * Material.MeshColor * diffuse * texture(Material.Textures[0], f_TexCoord);	
}
)"