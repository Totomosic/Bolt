#pragma once
#include "Bolt-Core.h"
#include "..\..\Shaders\__Shaders__.h"

#include "LightSettings.h"
#include "RenderSettings.h"
#include "TextureGroup.h"
#include "..\..\ResourcePtr.h"

namespace Bolt
{

	struct BLT_API Material
	{
	public:
		ResourcePtr<const Shader> Shader = Shader::DefaultColor();
		Color BaseColor = Color::White;
		LightSettings LightingOptions = LightSettings();
		RenderSettings RenderOptions = RenderSettings();
		TextureGroup Textures = TextureGroup();
		UniformManager Uniforms = UniformManager();

	public:
		bool operator==(const Material& other) const;
		bool operator!=(const Material& other) const;

		bool HasTransparency() const;

	};

}

namespace std
{

	template<>
	struct BLT_API hash<Bolt::Material>
	{
	public:
		inline size_t operator()(const Bolt::Material& material) const { return 42; };
	};

}