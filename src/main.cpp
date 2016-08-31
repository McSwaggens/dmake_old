#include <string>
#include <iostream>
#include "colors.h"
#include "dmake.h"
#include "languages.h"
#include "clp.h"

void Compile 	();


int main (int argc, char* argv[])
{
	// Evaluate the parameters given to dmake
	vector<Option> options = Evaluate (argc, argv);
	
	for (int f = 0; f < options.size(); f++)
	{
		Option flag = options[f];
		
		if (flag == COMPILE)
		{
			Compile ();
		}
		else
		if (flag == INSTALL)
		{
			bool success = Install ();
			if (!success)
			{
				return 0;
			}
		}
		else
		if (flag == RUN)
		{
			Run ();
		}
		else
		if (flag == CLEAN)
		{
			Clean ();
		}
	}
	
	return 0;
}

void Compile ()
{
	FSNode* rootDirectory = new FSNode;
	rootDirectory->name = "./src";
	
	bool directoryCheckSuccess = CheckRequiredDirectories ();
	
	if (!directoryCheckSuccess)
	{
		return;
	}
	
	Scan (rootDirectory);
	
	Language language = FindLanguage ();
	
	Project* project;
	project = GenerateProject (&language);
	
	project->GenerateRequired ();
	
	project->Compile ();
	
	project->Clean ();
}


void Clean ()
{
	std::cout << BOLD << WHITE << "Cleaning..." << RESET << std::endl;
}

