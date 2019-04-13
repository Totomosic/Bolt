#include "Types.h"
#include "Destructor.h"

#include "Engine/Scene/SceneManager.h"
#include "Engine/User/Input.h"
#include "Engine/Renderer/Graphics.h"

namespace Bolt
{

	void Destructor::Run()
	{
		WSACleanup();
		BLT_CORE_INFO("Socket Terminated");
	}

}