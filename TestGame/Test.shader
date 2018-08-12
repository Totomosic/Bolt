#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 color;

out vec2 f_TexCoord;
out vec4 f_Color;

void main(void)
{
	gl_Position = vec4(position, 1.0);
	f_TexCoord = texCoord;
	f_Color = color;
}

#shader fragment
#version 430 core

in vec2 f_TexCoord;
in vec4 f_Color;

layout(location = 0) out vec4 color;

uniform sampler2D Tex;
uniform vec4 u_Color;

void main(void)
{
	color = f_Color * u_Color;// *texture(Tex, f_TexCoord);
}