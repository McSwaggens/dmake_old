#include <iostream>
#include <vector>
#include <string>
#include "colors.h"

enum Option
{
	COMPILE,
	INSTALL,
	CLEAN
};

static std::vector<Option> options;

void Evaluate (int argc, char* argv[]);