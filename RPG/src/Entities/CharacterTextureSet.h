#pragma once
#include "BoltEngine.h"

using namespace Bolt;

namespace RPG
{

	BLT_API enum class CharacterOrientation
	{
		Left,
		Right,
		Up,
		Down
	};

	struct CharacterTextureSet
	{
	public:
		const Texture2D* Left = nullptr;
		const Texture2D* Right = nullptr;
		const Texture2D* Up = nullptr;
		const Texture2D* Down = nullptr;
	};

}