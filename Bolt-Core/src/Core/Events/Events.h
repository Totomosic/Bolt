#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API InputEvents
	{
	public:
		uint32_t KeyPressed = 1;
		uint32_t KeyReleased = 2;
		uint32_t MousePressed = 3;
		uint32_t MouseReleased = 4;
		uint32_t MouseScrolled = 5;
		uint32_t MouseMoved = 6;
	};

	struct BLT_API UIEvents
	{
	public:
		uint32_t OnHoverEntry = 101;
		uint32_t OnHover = 102;
		uint32_t OnHoverExit = 103;
		uint32_t OnClicked = 104;
	};

	struct BLT_API WindowEvents
	{
	public:
		uint32_t WindowResized = 201;
		uint32_t WindowMoved = 202;
		uint32_t WindowFocused = 203;
		uint32_t WindowLostFocus = 204;
		uint32_t WindowClosed = 205;
	};

	struct BLT_API SceneEvents
	{
	public:
		uint32_t SceneLoaded = 301;
		uint32_t SceneUnloaded = 302;
		uint32_t GameObjectInstantiated = 303;
		uint32_t GameObjectDestroyed = 304;
	};

	struct BLT_API InternalEvents
	{
	public:
		uint32_t AsyncTaskCompleted = 1001;
	};

	struct BLT_API Events
	{
	public:
		static constexpr uint32_t NONE = 0;
		static constexpr uint32_t MIN_USER_ID = 10000;
		
		static constexpr InputEvents Input = InputEvents();
		static constexpr UIEvents UI = UIEvents();
		static constexpr WindowEvents Window = WindowEvents();
		static constexpr SceneEvents Scene = SceneEvents();
		static constexpr InternalEvents Internal = InternalEvents();
	};

}