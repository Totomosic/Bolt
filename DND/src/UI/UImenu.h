#pragma once

namespace DND
{

	class UImenu
	{
	public:
		virtual void CreateMenu() = 0;
		virtual void Update() = 0;
		virtual void DestroyMenu() = 0;

	};

}