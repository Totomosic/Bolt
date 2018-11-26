R"(
#version 430 core
#include "BoltMaterials.glh"

in vec2 f_TexCoord;
in vec4 f_Color;
in float f_TID;
in vec3 f_WorldPosition;

uniform Material2D Material;

layout(location = 0) out vec4 FinalColor;

void main()
{
	vec4 texColor = vec4(1.0);
	if (f_TID > 15)
	{
		texColor = texture(Material.Textures[15], f_TexCoord);
	}
	else if (f_TID > 14)
	{
		texColor = texture(Material.Textures[14], f_TexCoord);
	}
	else if (f_TID > 13)
	{
		texColor = texture(Material.Textures[13], f_TexCoord);
	}
	else if (f_TID > 12)
	{
		texColor = texture(Material.Textures[12], f_TexCoord);
	}
	else if (f_TID > 11)
	{
		texColor = texture(Material.Textures[11], f_TexCoord);
	}
	else if (f_TID > 10)
	{
		texColor = texture(Material.Textures[10], f_TexCoord);
	}
	else if (f_TID > 9)
	{
		texColor = texture(Material.Textures[9], f_TexCoord);
	}
	else if (f_TID > 8)
	{
		texColor = texture(Material.Textures[8], f_TexCoord);
	}
	else if (f_TID > 7)
	{
		texColor = texture(Material.Textures[7], f_TexCoord);
	}
	else if (f_TID > 6)
	{
		texColor = texture(Material.Textures[6], f_TexCoord);
	}
	else if (f_TID > 5)
	{
		texColor = texture(Material.Textures[5], f_TexCoord);
	}
	else if (f_TID > 4)
	{
		texColor = texture(Material.Textures[4], f_TexCoord);
	}
	else if (f_TID > 3)
	{
		texColor = texture(Material.Textures[3], f_TexCoord);
	}
	else if (f_TID > 2)
	{
		texColor = texture(Material.Textures[2], f_TexCoord);
	}
	else if (f_TID > 1)
	{
		texColor = texture(Material.Textures[1], f_TexCoord);
	}
	else if (f_TID > 0)
	{
		texColor = texture(Material.Textures[0], f_TexCoord);
	}
	FinalColor = f_Color * Material.MeshColor * texColor;	
}
)"