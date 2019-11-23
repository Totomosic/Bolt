#include "ResourceSettings.h"

#include <iostream>
#include <fstream>

namespace Bolt
{

	void CleanString(std::string& str)
	{
		size_t index = str.find_first_not_of(' ');
		if (index != std::string::npos)
		{
			str = str.substr(index);
		}
		index = str.find_last_not_of(' ');
		if (index != std::string::npos)
		{
			str = str.substr(0, index + 1);
		}
	}

	ResourceProperty::ResourceProperty() : ResourceProperty("")
	{
	}

	ResourceProperty::ResourceProperty(const std::string& name, const std::string& defaultValue, std::vector<std::unique_ptr<PropertyConstraint>>&& constraints, bool setByUser)
		: m_Name(name), m_Value(defaultValue), m_Contraints(std::move(constraints)), m_SetByUser(setByUser)
	{

	}

	bool ResourceProperty::SetValue(const std::string& value)
	{
		for (const auto& constraint : m_Contraints)
		{
			if (!constraint->Evaluate(value))
			{
				return false;
			}
		}
		m_Value = value;
		return true;
	}

	bool ResourceProperty::HasValue() const
	{
		return !m_Value.empty();
	}

	bool ResourceProperty::IsSetByUser() const
	{
		return m_SetByUser;
	}

	const std::string& ResourceProperty::GetStringValue() const
	{
		return m_Value;
	}

	void ResourceProperty::AddConstraint(std::unique_ptr<PropertyConstraint>&& constraint)
	{
		m_Contraints.push_back(std::move(constraint));
	}

	ResourceSettings::ResourceSettings(ResourceType type)
		: m_Properties(), m_Type(type)
	{

	}

	ResourceType ResourceSettings::GetType() const
	{
		return m_Type;
	}

	ResourceProperty& ResourceSettings::AddProperty(const std::string& name, const std::string& defaultValue, std::vector<std::unique_ptr<PropertyConstraint>>&& constraints, bool setByUser)
	{
		m_Properties[name] = { name, defaultValue, std::move(constraints), setByUser };
		return m_Properties[name];
	}

	ResourceProperty& ResourceSettings::AddIntProperty(const std::string& name, int defaultValue, int min, int max, bool setByUser)
	{
		std::string value;
		if (defaultValue != INT_MAX)
		{
			value = std::to_string(defaultValue);
		}
		ResourceProperty& prop = AddProperty(name, value, {}, setByUser);
		prop.AddConstraint(std::make_unique<IntConstraint>(min, max));
		return prop;
	}

	ResourceProperty& ResourceSettings::AddFloatProperty(const std::string& name, float defaultValue, float min, float max, bool setByUser)
	{
		std::string value;
		if (defaultValue != INFINITY)
		{
			value = std::to_string(defaultValue);
		}
		ResourceProperty& prop = AddProperty(name, value, {}, setByUser);
		prop.AddConstraint(std::make_unique<FloatConstraint>(min, max));
		return prop;
	}

	ResourceProperty& ResourceSettings::AddStringProperty(const std::string& name, const std::string& defaultValue, int minLength, const std::vector<std::string>& acceptedValues, bool setByUser)
	{
		ResourceProperty& prop = AddProperty(name, defaultValue, {}, setByUser);
		prop.AddConstraint(std::make_unique<StringConstraint>(minLength, acceptedValues));
		return prop;
	}

	const ResourceProperty& ResourceSettings::GetProperty(const std::string& name) const
	{
		return m_Properties.at(name);
	}

	ResourceProperty& ResourceSettings::GetProperty(const std::string& name)
	{
		return m_Properties.at(name);
	}

	bool ResourceSettings::IsComplete() const
	{
		for (const auto& prop : m_Properties)
		{
			if (!prop.second.HasValue())
			{
				return false;
			}
		}
		return true;
	}

	bool ResourceSettings::HasProperty(const std::string& name) const
	{
		return m_Properties.find(name) != m_Properties.end();
	}

	bool ResourceSettings::PopulateFromFile(const std::string& filename)
	{
		std::ifstream stream(filename);
		std::string line;
		while (!stream.eof())
		{
			stream >> line;
			size_t index = line.find('=');
			if (index == std::string::npos)
			{
				continue;
			}
			std::string key = line.substr(0, index);
			std::string value = line.substr(index + 1);
			CleanString(key);
			CleanString(value);
			if (key == TYPE_KEY)
			{
				BLT_ASSERT(ConvertString<int>(value) == (int)GetType(), "Invalid Settings file type");
			}
			if (HasProperty(key))
			{
				auto& prop = GetProperty(key);
				prop.SetValue(value);
			}
		}
		return true;
	}

	std::string GetValueFromStdin(const std::string& prompt)
	{
		std::cout << prompt;
		char buffer[1024];
		std::cin.getline(buffer, sizeof(buffer) / sizeof(char));
		std::string result = buffer;
		return result;
	}

	bool ResourceSettings::PopulateFromStdin()
	{
		for (auto& prop : m_Properties)
		{
			if (prop.second.IsSetByUser())
			{
				while (true)
				{
					std::string value = GetValueFromStdin(prop.first + ": ");
					if (!prop.second.SetValue(value))
					{
						BLT_ERROR("Invalid value {0} for property {1}", value, prop.first);
					}
					else
					{
						break;
					}
				}
			}
		}
		return true;
	}

	void ResourceSettings::WriteToFile(const std::string& filename) const
	{
		std::ofstream stream(filename);
		stream << TYPE_KEY << ':' << (int)GetType() << std::endl;
		for (const auto& prop : m_Properties)
		{
			if (prop.second.HasValue())
			{
				stream << prop.first << "=" << prop.second.GetStringValue() << std::endl;
			}
			else
			{
				BLT_ERROR("Property {0} has no value.", prop.first);
			}
		}
	}

	IntConstraint::IntConstraint(int min, int max)
		: Min(min), Max(max)
	{
	}

	FloatConstraint::FloatConstraint(float min, float max)
		: Min(min), Max(max)
	{
	}

	StringConstraint::StringConstraint(int minLength, const std::vector<std::string>& acceptedValues)
		: AcceptedValues(acceptedValues), MinLength(minLength)
	{
	}

}