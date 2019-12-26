#pragma once
#include "RenderCamera.h"
#include "RenderOptions.h"

namespace Bolt
{

	struct BLT_API RenderProcess
	{
	public:
		id_t LayerMask = (id_t)-1;
		RenderOptions Options;
		std::unordered_map<id_t, RenderCameraRef> Cameras;
	};

	class BLT_API RenderSchedule
	{
	private:
		std::vector<RenderProcess> m_Processes;

	public:
		RenderSchedule();

		const std::vector<RenderProcess>& Processes() const;
		id_t AddRenderProcess(const RenderProcess& process);

	};

}