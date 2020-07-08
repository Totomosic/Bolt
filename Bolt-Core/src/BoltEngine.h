#pragma once
#include "BoltLib/Functions.h"
#include "Core/Time/Time.h"
#include "Core/Memory/Allocator.h"
#include "Core/Tasks/TaskManager.h"
#include "BoltLib/IO/Filesystem.h"
#include "BoltLib/Math/Random/NoiseFunctions/SimplexNoise.h"
#include "BoltLib/Profiling/Profiling.h"
#include "BoltLib/DebugTimer.h"

#include "Graphics/Window.h"
#include "Graphics/Assets/AssetManager.h"
#include "Graphics/Assets/Meshes/Materials/MaterialGraph/Graphs.h"
#include "Graphics/Assets/Textures/RenderTexture2D.h"
#include "Graphics/Assets/Meshes/Materials/Shaders/ShaderFileReader.h"
#include "Graphics/Assets/Meshes/Materials/MaterialFileReader.h"

#include "Graphics/Assets/Meshes/Model.h"
#include "Graphics/Assets/Meshes/Factories/RectangleFactory.h"
#include "Graphics/Assets/Meshes/Factories/CuboidFactory.h"
#include "Graphics/Assets/Meshes/Mesh.h"

#include "Engine/Application.h"
#include "Engine/Engine.h"
#include "Engine/Scripting/Lua/LuaScriptingEnv.h"

#include "Engine/Networking/SocketUtil.h"
#include "Engine/Networking/UDPsocket.h"
#include "Engine/Networking/TCPsocket.h"

#include "Engine/Renderer/2D/Renderer2D.h"

#include "Engine/Scene/EntityManager.h"
#include "Engine/Scene/ScopedEntity.h"

#include "Engine/Scene/Systems/UI/Shapes/UIRectangle.h"
#include "Engine/Scene/Systems/UI/UIText.h"
#include "Engine/Scene/Systems/UI/UITextInput.h"

#include "Engine/Scene/Components/Transform.h"
#include "Engine/Scene/Components/Camera.h"
#include "Engine/Scene/Components/UpdateFunction.h"

#include "Engine/Scripting/JavaScript/JSContext.h"