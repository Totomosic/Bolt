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

		blt::string EC2_ADDRESS = "ec2-13-58-43-219.us-east-2.compute.amazonaws.com";
		int EC2_PORT = 12345;

	public:
		void Init() override;
		void Tick() override;
		void Update() override;
		void Render() override;
		void Exit() override;

	};

}