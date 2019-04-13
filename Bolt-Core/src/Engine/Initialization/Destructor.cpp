#include "Types.h"
#include "Destructor.h"

#include "Engine/Scene/SceneManager.h"
#include "Engine/User/Input.h"
#include "Engine/Renderer/Graphics.h"

namespace Bolt
{

	void Destructor::Run(std::unique_ptr<Window>&& window)
	{
		SceneManager::Terminate();
		BLT_CORE_INFO("SceneManager Terminated");
		ObjectFactory::Terminate();
		BLT_CORE_INFO("ObjectFactory Terminated");
		Graphics::Terminate();
		BLT_CORE_INFO("Graphics Terminated");
		ResourceManager::Get().Terminate();
		BLT_CORE_INFO("ResourceManager Terminated");
		Input::Get().Terminate();
		BLT_CORE_INFO("Input Terminated");
		{
			std::unique_ptr<Window> windowPtr = std::move(window);
		}
		BLT_CORE_INFO("Window Terminated");
		WSACleanup();
		BLT_CORE_INFO("Socket Terminated");
	}

}