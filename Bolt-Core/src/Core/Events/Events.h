#pragma once
#include "bltpch.h"

namespace Bolt
{

	struct BLT_API Events
	{
	public:
		struct BLT_API InputEvents
		{
		public:
			static constexpr uint32_t KeyDown = 1;
			static constexpr uint32_t KeyUp = 2;
			static constexpr uint32_t MouseDown = 3;
			static constexpr uint32_t MouseUp = 4;
			static constexpr uint32_t MouseScrolled = 5;
			static constexpr uint32_t MouseMoved = 6;
			static constexpr uint32_t MouseClicked = 7;
			static constexpr uint32_t CharPressed = 8;
		};

		struct BLT_API UIEvents
		{
		public:
			static constexpr uint32_t OnHoverEntry = 101;
			static constexpr uint32_t OnHover = 102;
			static constexpr uint32_t OnHoverExit = 103;
			static constexpr uint32_t OnClick = 104;
			static constexpr uint32_t OnFocus = 105;
			static constexpr uint32_t OnFocusLost = 106;
			static constexpr uint32_t OnMouseDown = 107;
			static constexpr uint32_t OnMouseUp = 108;
			static constexpr uint32_t OnKeyDown = 109;
			static constexpr uint32_t OnKeyUp = 110;
			static constexpr uint32_t OnCharPressed = 111;
		};

		struct BLT_API WindowEvents
		{
		public:
			static constexpr uint32_t WindowResized = 201;
			static constexpr uint32_t WindowMoved = 202;
			static constexpr uint32_t WindowFocused = 203;
			static constexpr uint32_t WindowLostFocus = 204;
			static constexpr uint32_t WindowClosed = 205;
		};

		struct BLT_API SceneEvents
		{
		public:
			static constexpr uint32_t SceneLoaded = 301;
			static constexpr uint32_t SceneUnloaded = 302;
			static constexpr uint32_t EntityCreated = 303;
			static constexpr uint32_t EntityDestroyed = 304;
		};

		struct BLT_API InternalEvents
		{
		public:
			static constexpr uint32_t AsyncTaskCompleted = 1001;
		};

	public:
		static constexpr uint32_t NONE = 0;
		static constexpr uint32_t MIN_USER_ID = 10000;
		
		static constexpr Events::InputEvents Input = Events::InputEvents();
		static constexpr Events::UIEvents UI = Events::UIEvents();
		static constexpr Events::WindowEvents Window = Events::WindowEvents();
		static constexpr Events::SceneEvents Scene = Events::SceneEvents();
		static constexpr Events::InternalEvents Internal = Events::InternalEvents();
	};

}