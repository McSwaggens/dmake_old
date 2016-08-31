#include <iostream>
#include <vector>
#include <string>
#include "colors.h"

enum Option
{
	COMPILE,
	INSTALL,
	CLEAN,
	RUN
};

std::vector<Option> Evaluate (int argc, char* argv[]);