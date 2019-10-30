#pragma once
#include "Meshes/Materials/MaterialGraph/PBRMaterialGraph.h"
#include "Meshes/Materials/MaterialGraph/LitMaterialGraph.h"
#include "Meshes/Materials/MaterialGraph/BasicMaterialGraph.h"
#include "Meshes/Materials/Material.h"
#include "Textures/Fonts/Font.h"

#include "MaterialDefs/PBRMaterials.h"

namespace Bolt
{

	class ResourceManager;

	class BLT_API MaterialManager
	{
	private:
		ResourceManager* m_Manager;

		BasicMaterialGraph m_DefaultGraph;
		BasicMaterialGraph m_TextureGraph;
		MaterialBuilder m_FontBuilder;
		LitMaterialGraph m_DefaultLightingGraph;
		LitMaterialGraph m_TextureLightingGraph;
		PBRMaterialGraph m_PBRGraph;
		PBRMaterialGraph m_PBRTextureGraph;
		
	public:
		MaterialManager(ResourceManager* manager);

		std::unique_ptr<Material> Default(const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> Texture(const ResourcePtr<Texture2D>& texture) const;
		std::unique_ptr<Material> Font(const ResourcePtr<Bolt::Font>& font, const Color& baseColor = Color::White) const;
		std::unique_ptr<Material> DefaultLighting(const Color& color = Color::White) const;
		std::unique_ptr<Material> TextureLighting() const;
		std::unique_ptr<PBRMaterial> PBR() const;
		std::unique_ptr<PBRTextureMaterial> PBRTexture() const;

	private:
		void CreateDefaultMaterial(BasicMaterialGraph& builder) const;
		void CreateTextureMaterial(BasicMaterialGraph& builder) const;
		void CreateFontMaterial(MaterialBuilder& builder) const;
		
		void CreateDefaultLightingMaterial(LitMaterialGraph& graph) const;
		void CreateTextureLightingMaterial(LitMaterialGraph& graph) const;

		void CreatePBRMaterial(PBRMaterialGraph& graph) const;
		void CreatePBRTextureMaterial(PBRMaterialGraph& graph) const;

	};

}