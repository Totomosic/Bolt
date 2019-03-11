#pragma once
#include "Values/ShaderEnums.h"
#include "Values/ShaderValue.h"

namespace Bolt
{

	class BLT_API BuiltInFunc
	{
	public:
		struct BLT_API Overload
		{
		public:
			std::vector<ValueType> Inputs;
			blt::string SourceTemplate;
			ValueType Output;
		};

		struct BLT_API OverloadInstance
		{
		public:
			ValueType Output;
			blt::string Source;
		};

	private:
		std::vector<Overload> m_Overloads;

	public:
		BuiltInFunc();

		const std::vector<Overload>& Overloads() const;

		bool HasOverload(const std::vector<ValueType>& inputs, ValueType output) const;
		void AddOverload(const std::vector<ValueType>& inputs, ValueType output, const blt::string& sourceTemplate);
		OverloadInstance GetSource(const std::vector<ShaderValuePtr>& inputs) const;

	private:
		const Overload& GetOverload(const std::vector<ValueType>& inputs) const;

	};

}