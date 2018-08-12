R"(
#version 430 core

in vec2 f_TexCoord;
in vec4 f_Color;

uniform vec4 u_Color;

layout(location = 0) out vec4 FinalColor;

void main()
{
	FinalColor = f_Color * u_Color;	
}
)"