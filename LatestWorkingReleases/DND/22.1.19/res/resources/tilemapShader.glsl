#shader vertex
#version 430 core

struct ClipPlane
{
    vec3 Normal;
    float Distance;
};

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;
layout(location = 3) in vec4 in_Color;
layout(location = 4) in vec2 in_TileMapSize;
layout(location = 5) in vec2 in_TotalMapSize;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_TexTransforms[1];

uniform ClipPlane u_ClippingPlane;

out vec2 f_TexCoord;
out vec4 f_Color;
out vec3 f_ModelPosition;
out vec2 f_TileMapSize;
out vec2 f_TotalMapSize;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0);
	gl_ClipDistance[0] = dot(u_ModelMatrix * vec4(in_Position, 1.0), vec4(u_ClippingPlane.Normal, u_ClippingPlane.Distance));

	f_TexCoord = (u_TexTransforms[0] * vec3(in_TexCoord, 1.0)).xy;
	f_Color = in_Color;
	f_ModelPosition = in_Position;
	f_TileMapSize = in_TileMapSize;
	f_TotalMapSize = in_TotalMapSize;
}

#shader fragment
#version 430 core
#include "BoltMaterials.glh"

in vec2 f_TexCoord;
in vec4 f_Color;
in vec3 f_ModelPosition;
in vec2 f_TileMapSize;
in vec2 f_TotalMapSize;

uniform Material2D Material;

layout(location = 0) out vec4 FinalColor;

vec2 GetTexCoord()
{
	float tileWidth = f_TotalMapSize.x / f_TileMapSize.x;
	float tileHeight = f_TotalMapSize.y / f_TileMapSize.y;
	float tileX = floor((f_ModelPosition.x) / tileWidth) * tileWidth;
	float tileY = floor((f_ModelPosition.y) / tileHeight) * tileHeight;
	float x = (f_ModelPosition.x - tileX) / tileWidth;
	float y = (f_ModelPosition.y - tileY) / tileHeight;
	return vec2(x, y);
}

void main()
{
	FinalColor = f_Color * Material.MeshColor;
	vec4 tilemapColor = texture(Material.Textures[0], f_TexCoord);
	vec2 coord = GetTexCoord();
	vec4 grassColor = texture(Material.Textures[1], coord);
	vec4 sandColor = texture(Material.Textures[2], coord);
	vec4 waterColor = texture(Material.Textures[3], coord);
	float grassAmount = tilemapColor.g;
	float sandAmount = tilemapColor.r;
	float waterAmount = tilemapColor.b;

	vec4 final = grassColor * grassAmount + sandColor * sandAmount + waterColor * waterAmount;

	FinalColor *= vec4(final.xyz, 1.0);
}