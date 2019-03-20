#pragma once
#include "Components/Values/ShaderEnums.h"
#include "Shader.h"
#include "Graphics\Resources\Textures\Texture2D.h"

namespace Bolt
{

	class BLT_API UniformLinkContainer
	{
	protected:
		id_t m_ShaderId;
		int m_Location;

	public:
		UniformLinkContainer(id_t shaderId, int location);

		id_t ShaderId() const;
		int UniformLocation() const;
		virtual const void* GetValuePtr() const = 0;
		virtual void* GetValuePtr() = 0;

		virtual void UploadValue(const Shader& shader) const = 0;

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

		void UploadValue(const Shader& shader) const override
		{
			shader.SetUniform(m_Location, m_Value);
		}
	};

	template<>
	class BLT_API UniformLink<ResourcePtr<const Texture2D>> : public UniformLinkContainer
	{
	private:
		ResourcePtr<const Texture2D> m_Value;

	public:
		UniformLink() : UniformLinkContainer(0, -1)
		{

		}

		UniformLink(id_t shaderId, int location, const ResourcePtr<const Texture2D>& value) : UniformLinkContainer(shaderId, location),
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

		const ResourcePtr<const Texture2D>& Value() const
		{
			return m_Value;
		}

		ResourcePtr<const Texture2D>& Value()
		{
			return m_Value;
		}

		void UploadValue(const Shader& shader) const override
		{
			shader.SetUniform(m_Location, m_Value->Id());
		}

	};

	template<>
	class BLT_API UniformLink<ResourcePtr<Texture2D>> : public UniformLinkContainer
	{
	private:
		ResourcePtr<Texture2D> m_Value;

	public:
		UniformLink() : UniformLinkContainer(0, -1)
		{

		}

		UniformLink(id_t shaderId, int location, const ResourcePtr<Texture2D>& value) : UniformLinkContainer(shaderId, location),
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

		const ResourcePtr<Texture2D>& Value() const
		{
			return m_Value;
		}

		ResourcePtr<Texture2D>& Value()
		{
			return m_Value;
		}

		void UploadValue(const Shader& shader) const override
		{
			shader.SetUniform(m_Location, m_Value->Id());
		}

	};

}