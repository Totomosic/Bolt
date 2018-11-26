R"(
#version 430 core
#include "BoltMaterials.glh"

in vec3 f_TexCoord;
in vec4 f_Color;

uniform MaterialCube Material;

layout(location = 0) out vec4 FinalColor;

void main()
{
	FinalColor = f_Color * Material.MeshColor * texture(Material.Textures[0], f_TexCoord);	
}
)"