#shader vertex
#version 430 core

struct ClipPlane
{
    vec3 Normal;
    float Distance;
};

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 color;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

uniform ClipPlane u_ClippingPlane;

out vec2 f_TexCoord;
out vec3 f_WorldPosition;
out vec3 f_WorldNormal;
out vec3 f_CameraPosition;
out vec4 f_Color;
out vec4 f_ClipPosition;

const float scaling = 50;

void main()
{
	vec4 worldPosition = u_ModelMatrix * position;
	vec4 viewPosition = u_ViewMatrix * worldPosition;
	vec4 screenPosition = u_ProjectionMatrix * viewPosition;

	gl_ClipDistance[0] = dot(worldPosition, vec4(u_ClippingPlane.Normal, u_ClippingPlane.Distance));

	gl_Position = screenPosition;

	f_TexCoord = texCoord * scaling;
	f_WorldPosition = worldPosition.xyz;
	f_WorldNormal = (u_ModelMatrix * vec4(normal, 0)).xyz;
	f_CameraPosition = (inverse(u_ViewMatrix)[3]).xyz;
	f_Color = color;
	f_ClipPosition = screenPosition;
}

#shader fragment
#version 430 core

const int MAX_LIGHTS = 5;

struct Light
{
	vec3 Position;
	vec4 Color;
	float Ambient;
};

struct Material
{
	vec4 SpecularHighlight;
	float Shininess;
	float ShineDamper;
};

vec4 CalculateDiffuseColor(Light light, Material material, vec3 worldPosition, vec3 worldNormal)
{
	vec3 toLightVector = light.Position - worldPosition;
	vec3 unitToLightVector = normalize(toLightVector);
	vec3 unitNormal = normalize(worldNormal);
	float nDotV = dot(unitNormal, unitToLightVector);
	vec4 color = light.Color * max(nDotV, light.Ambient);
	color.a = light.Color.a;
	return color;
}

vec4 CalculateSpecularColor(Light light, Material material, vec3 worldPosition, vec3 worldNormal, vec3 cameraPosition)
{
	vec3 toLightVector = light.Position - worldPosition;
	vec3 unitToLightVector = normalize(toLightVector);
	vec3 unitNormal = normalize(worldNormal);
	vec3 toCameraVector = cameraPosition - worldPosition;
	vec3 unitToCameraVector = normalize(toCameraVector);
	vec3 lightDirection = -unitToLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specularFactor = max(dot(reflectedLightDirection, unitToCameraVector), 0.0);
	float dampedSpecularFactor = pow(specularFactor, material.ShineDamper);
	return light.Color * dampedSpecularFactor * material.Shininess * material.SpecularHighlight;
}

in vec2 f_TexCoord;
in vec3 f_WorldPosition;
in vec3 f_WorldNormal;
in vec3 f_CameraPosition;
in vec4 f_Color;
in vec4 f_ClipPosition;

uniform sampler2D Reflection;
uniform sampler2D DUDV;
uniform sampler2D Normal;

uniform vec4 u_Color;
uniform float u_MoveFactor;
uniform Light u_Lights[MAX_LIGHTS];
uniform int UsedLights = 1;
uniform Material material;

layout(location = 0) out vec4 finalColor;
const float waveStrength = 0.03;

void main()
{
	vec4 lightColor = vec4(0.0);
	vec3 toCameraVector = f_CameraPosition - f_WorldPosition;
	vec3 unitToCameraVector = normalize(toCameraVector);

	vec2 ndc = f_ClipPosition.xy / f_ClipPosition.w;
    vec2 texCoords = ndc / 2.0 + 0.5;
    vec2 reflectCoords = vec2(texCoords.x, 1.0 - texCoords.y);
    vec2 refractCoords = vec2(texCoords.x, texCoords.y);

    vec2 distortedTexCoords = texture(DUDV, vec2(f_TexCoord.x + u_MoveFactor, f_TexCoord.y)).rg * 0.1;
    distortedTexCoords = f_TexCoord + vec2(distortedTexCoords.x, distortedTexCoords.y + u_MoveFactor);
    vec2 totalDistortion = (texture(DUDV, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    vec4 normalMapColor = texture(Normal, distortedTexCoords);
    vec3 worldNormal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    vec3 unitNormal = normalize(worldNormal);

    float refractiveFactor = dot(unitToCameraVector, f_WorldNormal);
    refractiveFactor = pow(refractiveFactor, material.Shininess / 3.0);

    refractCoords += totalDistortion;
    refractCoords = clamp(refractCoords, 0.00001, 0.99999);
    reflectCoords += totalDistortion;
    //reflectCoords = clamp(reflectCoords, 0.00001, 0.99999);

	//vec4 refractColor = texture(Refraction, refractCoords);
	vec4 reflectColor = texture(Reflection, reflectCoords);
	vec4 mixColor = reflectColor;// mix(reflectColor, vec4(0.0, 0.0, 0.0, 1.0), refractiveFactor / 2);

	for (int i = 0; i < UsedLights; i++)
	{
		lightColor += CalculateDiffuseColor(u_Lights[i], material, f_WorldPosition, unitNormal);
		lightColor += CalculateSpecularColor(u_Lights[i], material, f_WorldPosition, unitNormal, f_CameraPosition);
		lightColor.a = 1.0;
	}

	finalColor = mixColor * u_Color * f_Color * lightColor;
}