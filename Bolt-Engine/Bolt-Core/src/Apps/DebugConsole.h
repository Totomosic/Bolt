#pragma once
#include "Engine/Application.h"

namespace Bolt
{

	class BLT_API DebugConsole : public Application
	{
	private:
		bool m_NeedRedraw = true;

	private:
		void Init() override;
		void Update() override;
		void Render() override;
		void Exit() override;

	public:
		void PostMsg(const blt::string& msg);

	};

}