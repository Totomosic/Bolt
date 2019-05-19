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
		Materials.reserve(other.Materials.size());
		for (const auto& material : other.Materials)
		{
			Materials.push_back(material->Clone());
		}
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		Models = other.Models;
		Materials.clear();
		Materials.resize(other.Materials.size());
		for (const auto& material : other.Materials)
		{
			Materials.push_back(material->Clone());
		}
		return *this;
	}

	Mesh::Mesh(Mesh&& other)
		: Models(std::move(other.Models)), Materials(std::move(other.Materials))
	{

	}

	Mesh& Mesh::operator=(Mesh&& other)
	{
		Models = std::move(other.Models);
		Materials = std::move(other.Materials);
		return *this;
	}

}