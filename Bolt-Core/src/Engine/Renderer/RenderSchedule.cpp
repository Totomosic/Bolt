#include "bltpch.h"
#include "RenderSchedule.h"

namespace Bolt
{

	RenderSchedule::RenderSchedule()
		: m_Processes()
	{
	
	}

	const std::vector<RenderProcess>& RenderSchedule::Processes() const
	{
		return m_Processes;
	}

	id_t RenderSchedule::AddRenderProcess(const RenderProcess& process)
	{
		id_t index = m_Processes.size();
#ifdef BLT_DEBUG
		if (process.Cameras.size() > 0)
		{
			BLT_CORE_WARN("For override cameras referring to &Camera::ViewMatrix() and &Camera::ProjectionMatrix(), ensure to set Camera::transform().UpdateOnInvalidate(true) for proper functionality");
		}
#endif
		m_Processes.push_back(process);
		return index;
	}

}