#pragma once
#include "..\File.h"
#include "XMLnode.h"

namespace Bolt
{

	struct BLT_API XMLfile : public File
	{
	public:
		XMLnode Root;

	public:
		XMLfile(const Filepath& filename);

	};

}