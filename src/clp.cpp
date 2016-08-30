#include "clp.h"

std::vector<Option> Evaluate (int argc, char* argv[])
{
	std::vector<Option> options;
	
	if (argc == 1)
	{
		options.push_back (COMPILE);
	}
	else
	for (int i = 1; i < argc; i++)
	{
		std::string str (argv[i]);
		
		if (str == "install")
		{
			options.push_back (INSTALL);
		}
		else
		if (str == "clean")
		{
			options.push_back (CLEAN);
		}
	}
	
	return options;
}