R"(
#version 430 core

in vec2 f_TexCoord;
in vec4 f_Color;
in float f_TID;
in vec3 f_WorldPosition;

uniform vec4 u_Color;
uniform sampler2D u_Textures[16];

layout(location = 0) out vec4 FinalColor;

void main()
{
	vec4 texColor = vec4(1.0);
	if (f_TID > 15)
	{
		texColor = texture(u_Textures[15], f_TexCoord);
	}
	else if (f_TID > 14)
	{
		texColor = texture(u_Textures[14], f_TexCoord);
	}
	else if (f_TID > 13)
	{
		texColor = texture(u_Textures[13], f_TexCoord);
	}
	else if (f_TID > 12)
	{
		texColor = texture(u_Textures[12], f_TexCoord);
	}
	else if (f_TID > 11)
	{
		texColor = texture(u_Textures[11], f_TexCoord);
	}
	else if (f_TID > 10)
	{
		texColor = texture(u_Textures[10], f_TexCoord);
	}
	else if (f_TID > 9)
	{
		texColor = texture(u_Textures[9], f_TexCoord);
	}
	else if (f_TID > 8)
	{
		texColor = texture(u_Textures[8], f_TexCoord);
	}
	else if (f_TID > 7)
	{
		texColor = texture(u_Textures[7], f_TexCoord);
	}
	else if (f_TID > 6)
	{
		texColor = texture(u_Textures[6], f_TexCoord);
	}
	else if (f_TID > 5)
	{
		texColor = texture(u_Textures[5], f_TexCoord);
	}
	else if (f_TID > 4)
	{
		texColor = texture(u_Textures[4], f_TexCoord);
	}
	else if (f_TID > 3)
	{
		texColor = texture(u_Textures[3], f_TexCoord);
	}
	else if (f_TID > 2)
	{
		texColor = texture(u_Textures[2], f_TexCoord);
	}
	else if (f_TID > 1)
	{
		texColor = texture(u_Textures[1], f_TexCoord);
	}
	else if (f_TID > 0)
	{
		texColor = texture(u_Textures[0], f_TexCoord);
	}
	FinalColor = f_Color * u_Color * texColor;	
}
)"