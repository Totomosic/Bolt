#pragma once
#include "UniformLink.h"
#include "Shader.h"

namespace Bolt
{

	class BLT_API UniformValueContainer
	{
	public:
		UniformValueContainer() = default;
		virtual ~UniformValueContainer() = default;

		virtual std::unique_ptr<UniformLinkContainer> CreateLinkContainer(id_t shaderId, int location) const = 0;

	};

	template<typename T>
	class BLT_API UniformValue : public UniformValueContainer
	{
	private:
		T m_Value;

	public:
		UniformValue(const T& value) : UniformValueContainer(),
			m_Value(value)
		{
		
		}

		const T& GetValue() const
		{
			return m_Value;
		}

		T& GetValue()
		{
			return m_Value;
		}

		std::unique_ptr<UniformLinkContainer> CreateLinkContainer(id_t shaderId, int location) const override
		{
			return std::make_unique<UniformLink<T>>(shaderId, location, m_Value);
		}

	};

}