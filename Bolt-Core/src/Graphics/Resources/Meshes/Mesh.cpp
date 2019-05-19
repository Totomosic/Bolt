#include "bltpch.h"
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

	Mesh::Mesh(Mesh&& other)
		: Models(std::move(other.Models)), Materials()
	{
		for (int i = 0; i < MAX_MATERIALS; i++)
		{
			Materials[i] = std::move(other.Materials[i]);
		}
	}

	Mesh& Mesh::operator=(Mesh&& other)
	{
		Models = std::move(other.Models);
		for (int i = 0; i < MAX_MATERIALS; i++)
		{
			Materials[i] = std::move(other.Materials[i]);
		}
		return *this;
	}

	Mesh::~Mesh()
	{
		
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
		return result;
	}

}