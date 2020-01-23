#pragma once
#include "Values/ShaderVariable.h"
#include "Values/ShaderLiteral.h"
#include "Operations/ShaderOp.h"

namespace Bolt
{

#define BLT_DEFAULT_SHADERSCOPE_CLONE(ScopeType) inline virtual std::unique_ptr<ShaderScope> Clone() const override { return std::make_unique<ScopeType>(*this); }

	class BLT_API ShaderScope
	{
	protected:
		int m_ScopeIndex;
		std::vector<const ShaderVariable*> m_DeclaredVariables;
		std::vector<std::unique_ptr<ShaderOp>> m_Operations;

	public:
		ShaderScope();
		ShaderScope(int index);
		ShaderScope(const ShaderScope& other);
		ShaderScope& operator=(const ShaderScope& other);
		ShaderScope(ShaderScope&& other) = default;
		ShaderScope& operator=(ShaderScope&& other) = default;
		virtual ~ShaderScope() = default;

		int GetScopeIndex() const;
		bool IsDefinedInThisScope(const ShaderVariable* var) const;

		ShaderVariablePtr DefineVar(const ShaderValuePtr& value, const std::string& meta = "");
		ShaderVariablePtr DeclareVar(ValueType type, const std::string& meta = "");
		ShaderPassVariablePtr DeclarePassIn(const ShaderPassVariablePtr& outVar, const std::string& meta = "");
		ShaderPassVariablePtr DeclarePassOut(ValueType type, PassType passType,  const std::string& meta = "");
		ShaderVariablePtr DeclareArray(ValueType type, const ShaderLiteralPtr& length, const std::string& meta = "");
		void AddOperation(std::unique_ptr<ShaderOp>&& op);
		void InsertOperation(int index, std::unique_ptr<ShaderOp>&& op);
		ShaderScope& AddScope(std::unique_ptr<ShaderScope>&& scope);

		template<typename T>
		ShaderVariablePtr DeclareVar()
		{
			return DeclareVar(GetValueType<T>());
		}

		template<typename T>
		ShaderVariablePtr DeclareArray(uint32_t length)
		{
			return DeclareArray(GetValueType<T>(), length);
		}

		void SetVariable(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void AddAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void SubAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void MulAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		void DivAssign(const ShaderLValuePtr& var, const ShaderValuePtr& value);
		// Discards current fragment (only valid in fragment shader)
		void Discard();

		virtual void Build(ShaderBuilder& builder) const = 0;
		virtual std::unique_ptr<ShaderScope> Clone() const = 0;
		virtual void Reset();

		template<typename T, typename... Args>
		T& AddScope(Args&& ... args)
		{
			return (T&)AddScope(std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<typename T, typename... Args>
		void AddOperation(Args&& ... args)
		{
			AddOperation(std::make_unique<T>(std::forward<Args>(args)...));
		}

		friend class ShaderProgram;

	protected:
		void BuildOperations(ShaderBuilder& builder) const;
		void AddDeclaredVar(const ShaderVariable* var);
		int GetNextOpIndex() const;
		ShaderOp& GetOpAtIndex(int index) const;
	};

}