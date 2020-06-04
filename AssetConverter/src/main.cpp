#include "Module.h"
#include "ConverterModule.h"
#include "RecoveryModule.h"
#include <unordered_map>
#include "argparse.h"

static std::unordered_map<std::string, std::shared_ptr<Bolt::Assets::Module>> Modules = {
	{ "convert", std::make_shared<Bolt::Assets::ConverterModule>() },
	{ "recover", std::make_shared<Bolt::Assets::RecoveryModule>() }
};

int main(int argc, const char** argv)
{
	Bolt::Log::Initialize();
	if (argc > 1)
	{
		std::string module = argv[1];
		if (Modules.find(module) != Modules.end())
		{
			return Modules.at(module)->Run(argc - 1, argv + 1);
		}
		else
		{
			BLT_CORE_FATAL("Unrecognised module {}", module);
		}
	}
	std::cout << "No module specified" << std::endl;
	return 1;
}