#pragma once
#include "Components/Values/ShaderEnums.h"
#include "Graphics/Resources/Textures/Texture2D.h"
#include "Graphics/Resources/Textures/Fonts/Font.h"
#include "Shader.h"

namespace Bolt
{

	template<typename T>
	static void UploadShaderValue(const Shader& shader, int location, const T& value, int index)
	{
		shader.SetUniform(location, value);
	}

	template<>
	static void UploadShaderValue(const Shader& shader, int location, const ResourcePtr<Texture2D>& value, int index)
	{
		if (value != nullptr)
		{
			shader.SetUniform(location, index);
			value->Bind(index);
		}
	}

	template<>
	static void UploadShaderValue(const Shader& shader, int location, const ResourcePtr<Font>& value, int index)
	{
		if (value != nullptr)
		{
			shader.SetUniform(location, index);
			value->Bind(index);
		}
	}

	template<typename T>
	static void UploadShaderValue(const Shader& shader, int location, const std::function<T()>& value, int index)
	{
		if (value)
		{
			UploadShaderValue(shader, location, value(), index);
		}
	}

	class BLT_API UniformLinkContainer
	{
	protected:
		id_t m_ShaderId;
		int m_Location;

	public:
		UniformLinkContainer(id_t shaderId, int location);
		virtual ~UniformLinkContainer();

		template<typename T>
		UniformLinkContainer& operator=(const T& value)
		{
			SetValue(value);
			return *this;
		}

		id_t ShaderId() const;
		int UniformLocation() const;
		virtual const void* GetValuePtr() const = 0;
		virtual void* GetValuePtr() = 0;

		virtual void UploadValue(const Shader& shader, int index) const = 0;
		virtual std::unique_ptr<UniformLinkContainer> Clone(id_t shaderId, int location) const = 0;

		template<typename T>
		void SetValue(const T& value)
		{
			*(T*)GetValuePtr() = value;
		}

	};

	template<typename T>
	class BLT_API UniformLink : public UniformLinkContainer
	{
	private:
		T m_Value;

	public:
		UniformLink() : UniformLinkContainer(0, -1)
		{

		}

		UniformLink(id_t shaderId, int location, const T& value) : UniformLinkContainer(shaderId, location),
			m_Value(value)
		{

		}

		const void* GetValuePtr() const override
		{
			return (const void*)&m_Value;
		}

		void* GetValuePtr() override
		{
			return (void*)&m_Value;
		}

		const T& Value() const
		{
			return m_Value;
		}

		T& Value()
		{
			return m_Value;
		}

		void UploadValue(const Shader& shader, int index) const override
		{
			UploadShaderValue(shader, m_Location, m_Value, index);
		}

		std::unique_ptr<UniformLinkContainer> Clone(id_t shaderId, int location) const override
		{
			return std::make_unique<UniformLink<T>>(shaderId, location, m_Value);
		}

	};

}