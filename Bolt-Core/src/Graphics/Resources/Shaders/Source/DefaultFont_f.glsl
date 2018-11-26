R"(
#version 430 core
#include "BoltMaterials.glh"

in vec2 f_TexCoord;
in vec4 f_Color;

uniform Material2D Material;

layout(location = 0) out vec4 FinalColor;

void main()
{
	FinalColor = f_Color * Material.MeshColor;	
	FinalColor.a = texture(Material.Textures[0], f_TexCoord).r;
}
)"