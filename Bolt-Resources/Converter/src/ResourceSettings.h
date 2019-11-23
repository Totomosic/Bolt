#pragma once
#include "Logging.h"
#include <string>
#include "ResourceTypes.h"

namespace Bolt
{

	template<typename T>
	inline T ConvertString(const std::string& value)
	{
		BLT_ASSERT(false, "invalid type");
		return T();
	}

	template<>
	inline int ConvertString<int>(const std::string& value)
	{
		return std::stoi(value);
	}

	template<>
	inline float ConvertString<float>(const std::string& value)
	{
		return std::stof(value);
	}

	template<>
	inline std::string ConvertString<std::string>(const std::string& value)
	{
		return value;
	}

	class BLT_API PropertyConstraint
	{
	public:
		virtual bool Evaluate(const std::string& value) const = 0;
	};

	class BLT_API IntConstraint : public PropertyConstraint
	{
	public:
		int Min = INT_MIN;
		int Max = INT_MAX;

	public:
		IntConstraint(int min = INT_MIN, int max = INT_MAX);

		inline virtual bool Evaluate(const std::string& value) const override
		{
			for (int i = 0; i < value.length(); i++)
			{
				char c = value[i];
				if (!(std::isdigit(c) || (i == 0 && c == '-')))
				{
					return false;
				}
			}
			int i = std::stoi(value);
			return i >= Min && i <= Max;
		}
	};

	class BLT_API FloatConstraint : public PropertyConstraint
	{
	public:
		float Min = -INFINITY;
		float Max = +INFINITY;

	public:
		FloatConstraint(float min = -INFINITY, float max = INFINITY);

		inline virtual bool Evaluate(const std::string& value) const override
		{
			bool foundDot = false;
			for (int i = 0; i < value.length(); i++)
			{
				char c = value[i];
				if (!(std::isdigit(c) || (i == 0 && c == '-') || (!foundDot && c == '.')))
				{
					return false;
				}
				if (c == '.')
				{
					foundDot = true;
				}
			}
			float f = std::stof(value);
			return f >= Min && f <= Max;
		}
	};

	class BLT_API StringConstraint : public PropertyConstraint
	{
	public:
		std::vector<std::string> AcceptedValues = {};
		int MinLength = 1;

	public:
		StringConstraint(int minLength = 1, const std::vector<std::string>& acceptedValues = {});

		inline virtual bool Evaluate(const std::string& value) const override
		{
			if (value.length() < MinLength)
			{
				return false;
			}
			if (AcceptedValues.empty())
			{
				return true;
			}
			return std::find(AcceptedValues.begin(), AcceptedValues.end(), value) != AcceptedValues.end();
		}
	};

	class BLT_API ResourceProperty
	{
	private:
		std::string m_Name;
		std::vector<std::unique_ptr<PropertyConstraint>> m_Contraints;
		bool m_SetByUser;

		std::string m_Value;

	public:
		ResourceProperty();
		ResourceProperty(const std::string& name, const std::string& defaultValue = "", std::vector<std::unique_ptr<PropertyConstraint>>&& constraints = {}, bool setByUser = true);

		bool SetValue(const std::string& value);
		bool HasValue() const;
		bool IsSetByUser() const;
		const std::string& GetStringValue() const;

		void AddConstraint(std::unique_ptr<PropertyConstraint>&& constraint);

		template<typename T>
		bool SetValue(const T& value)
		{
			return SetValue(std::to_string(value));
		}

		template<size_t N>
		bool SetValue(const char (&value)[N])
		{
			return SetValue(std::string(value));
		}

		template<typename T>
		T GetValue()
		{
			BLT_ASSERT(GetPropertyType<T>() == m_Type, "Invalid property type");
			BLT_ASSERT(!m_Value.empty(), "Value has not been set");
			return ConvertString<T>(m_Value);
		}
	};

	class BLT_API ResourceSettings
	{
	public:
		static constexpr const char* TYPE_KEY = "Type";

	private:
		std::unordered_map<std::string, ResourceProperty> m_Properties;
		ResourceType m_Type;

	public:
		ResourceSettings(ResourceType type);

		ResourceType GetType() const;

		ResourceProperty& AddProperty(const std::string& name, const std::string& defaultValue = "", std::vector<std::unique_ptr<PropertyConstraint>>&& constraints = {}, bool setByUser = true);
		ResourceProperty& AddIntProperty(const std::string& name, int defaultValue = INT_MAX, int min = INT_MIN, int max = INT_MAX, bool setByUser = true);
		ResourceProperty& AddFloatProperty(const std::string& name, float defaultValue = INFINITY, float min = -INFINITY, float max = INFINITY, bool setByUser = true);
		ResourceProperty& AddStringProperty(const std::string& name, const std::string& defaultValue = "", int minLength = 1, const std::vector<std::string>& acceptedValues = {}, bool setByUser = true);

		const ResourceProperty& GetProperty(const std::string& name) const;
		ResourceProperty& GetProperty(const std::string& name);

		bool IsComplete() const;
		bool HasProperty(const std::string& name) const;

		bool PopulateFromFile(const std::string& filename);
		bool PopulateFromStdin();
		void WriteToFile(const std::string& filename) const;

	};

}