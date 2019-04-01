#include "Types.h"
#include "Mesh.h"

namespace Bolt
{

	Mesh::Mesh()
		: Models(), Materials()
	{
	
	}

	Mesh::Mesh(const Mesh& other)
		: Models(other.Models), Materials()
	{
		for (int i = 0; i < MAX_MATERIALS; i++)
		{
			const std::unique_ptr<Material>& material = other.Materials[i];
			if (material)
			{
				Materials[i] = material->Clone();
			}
		}
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		Models = other.Models;
		for (int i = 0; i < MAX_MATERIALS; i++)
		{
			const std::unique_ptr<Material>& material = other.Materials[i];
			if (material)
			{
				Materials[i] = material->Clone();
			}
		}
		return *this;
	}

	Mesh Mesh::Clone() const
	{
		Mesh result;
		result.Models = Models;
		for (int i = 0; i < MAX_MATERIALS; i++)
		{
			const std::unique_ptr<Material>& material = Materials[i];
			if (material)
			{
				result.Materials[i] = material->Clone();
			}
		}
		return std::move(result);
	}

}