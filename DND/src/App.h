#pragma once
#include "bltpch.h"

namespace DND
{

	class DndClient : public Application
	{
	public:
		int PORT = 10000;
		int TARGET_PORT = 12345;
		blt::string ADDRESS = "192.168.1.77";
		blt::string TARGET_ADDRESS = "localhost";

	public:
		void Init() override;
		void Tick() override;
		void Update() override;
		void Render() override;
		void Exit() override;

	};

}