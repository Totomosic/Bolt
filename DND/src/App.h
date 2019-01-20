#pragma once
#include "bltpch.h"

namespace DND
{

	class DndClient : public Application
	{
	public:
		int PORT = 12345;
		int TARGET_PORT = 12345;
		blt::string ADDRESS = "localhost";
		blt::string TARGET_ADDRESS = "localhost";

	public:
		void Init() override;
		void Tick() override;
		void Update() override;
		void Render() override;
		void Exit() override;

	};

}