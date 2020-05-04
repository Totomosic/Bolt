#pragma once
#include "../../MaterialNode.h"
#include "../../../Shaders/UniformValue.h"

namespace Bolt
{

	class BLT_API PropertyNode : public MaterialNode
	{
	private:
		std::string m_PropertyName;
		ValueType m_Type;
		std::shared_ptr<UniformValueContainer> m_DefaultValue;

	public:
		PropertyNode(const std::string& propertyName, ValueType type, const std::shared_ptr<UniformValueContainer>& defaultValue);

		const std::string& GetName() const;
		NodeConnection GetValue() const;

		virtual void Build(BuiltMaterialNode& node, const LinkedInputs& inputs, const MaterialGraphContext& context, MaterialGraphBuilder& builder) const override;
		virtual void ConnectDefaults(MaterialGraph& graph, const MaterialGraphContext& context) override;

	public:
		template<typename T>
		static std::unique_ptr<PropertyNode> Create(const std::string& propertyName, const T& defaultValue = T())
		{
			return std::make_unique<PropertyNode>(propertyName, GetValueType<T>(), std::make_shared<UniformValue<T>>(defaultValue));
		}

	};

}