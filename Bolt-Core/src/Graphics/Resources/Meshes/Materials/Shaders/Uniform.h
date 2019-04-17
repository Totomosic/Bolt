#pragma once
#include "Bolt-Core.h"
#include "Shader.h"

namespace Bolt
{

	struct BLT_API Uniform
	{
	public:
		blt::string UniformName;

	public:
		virtual void Upload(const Shader* shader) const = 0;
		virtual std::unique_ptr<Uniform> Clone() const = 0;

	};

	template<typename T>
	struct BLT_API UniformInstance : public Uniform
	{
	public:
		T Value;

	public:
		void Upload(const Shader* shader) const override
		{
			shader->SetUniform<T>(UniformName, Value);
		}

		std::unique_ptr<Uniform> Clone() const override
		{
			std::unique_ptr<UniformInstance<T>> result = std::make_unique<UniformInstance<T>>();
			result->UniformName = UniformName;
			result->Value = Value;
			return result;
		}

	};

}