#pragma once
#include "Core/Functions.h"
#include "Core/Time/Time.h"
#include "Core/Memory/Allocator.h"
#include "Core/Tasks/TaskManager.h"
#include "Core/Utils/Filesystem/Filesystem.h"
#include "Core/Math/Random/NoiseFunctions/SimplexNoise.h"
#include "Core/Profiling/Profiling.h"

#include "Graphics/Window.h"
#include "Graphics/Resources/ResourceManager.h"
#include "Graphics/Resources/ResourceExtractor.h"
#include "Graphics/Resources/Meshes/Materials/MaterialGraph/Graphs.h"
#include "Graphics/Resources/Textures/RenderTexture2D.h"
#include "Graphics/Resources/Meshes/Materials/Shaders/ShaderFileReader.h"
#include "Graphics/Resources/Meshes/Materials/MaterialFileReader.h"

#include "Graphics/Resources/Meshes/Model.h"
#include "Graphics/Resources/Meshes/Factories/RectangleFactory.h"
#include "Graphics/Resources/Meshes/Factories/CuboidFactory.h"
#include "Graphics/Resources/Meshes/Mesh.h"

#include "Engine/Application.h"
#include "Engine/Engine.h"
#include "Engine/CurrentContext.h"
#include "Engine/Scripting/Lua/LuaScriptingEnv.h"

#include "Engine/Networking/SocketUtil.h"
#include "Engine/Networking/UDPsocket.h"
#include "Engine/Networking/TCPsocket.h"
#include "Engine/Networking/IO/TCPclient.h"
#include "Engine/Networking/IO/TCPserver.h"

#include "Engine/Renderer/2D/Renderer2D.h"

#include "Engine/Scene/EntityManager.h"

#include "Engine/Scene/Systems/UI/Shapes/UIRectangle.h"
#include "Engine/Scene/Systems/UI/UIText.h"

#include "Engine/Scene/Components/Transform.h"
#include "Engine/Scene/Components/Camera.h"