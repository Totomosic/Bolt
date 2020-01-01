#pragma once
#include "Material.h"
#include "Shaders/ShaderFileReader.h"

namespace Bolt
{

	class BLT_API MaterialFileReader
	{
	private:
		ShaderFileReader m_Reader;
		bool m_IsTransparent;

	public:
		MaterialFileReader();
		MaterialFileReader(const FilePath& shaderFile);
		MaterialFileReader(const FilePath& vertexFile, const FilePath& fragmentFile);
		MaterialFileReader(const FilePath& vertexFile, const FilePath& geometryFile, const FilePath& fragmentFile);

		const ShaderFileReader& GetShaderReader() const;
		bool IsTransparent() const;

		void SetIsTransparent(bool isTransparent);

		template<typename MaterialT = Material>
		std::unique_ptr<MaterialT> BuildMaterial() const
		{
			std::unique_ptr<MaterialT> material = std::make_unique<MaterialT>(m_Reader.GetLinkContext(), IsTransparent());
			return material;
		}
	};

}