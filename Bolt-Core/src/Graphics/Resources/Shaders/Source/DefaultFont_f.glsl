R"(
#version 430 core

in vec2 f_TexCoord;
in vec4 f_Color;

uniform vec4 u_Color;
uniform sampler2D u_Textures[1];

layout(location = 0) out vec4 FinalColor;

void main()
{
	FinalColor = f_Color * u_Color;	
	FinalColor.a = texture(u_Textures[0], f_TexCoord).r;
}
)"