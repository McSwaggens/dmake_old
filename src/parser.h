#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "pstring.h"
#include "colors.h"
#include <algorithm>
#include "string.h"

namespace Parser
{
	std::string GenerateGPPLibraryParameters (std::string file);
	std::string LoadFile (std::string fileLocation);
	std::vector<std::string> SeperateLines (std::string* code);
	std::vector<std::string> SeperateSpaces (std::string* line);
}