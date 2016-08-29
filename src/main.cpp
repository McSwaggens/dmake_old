#include <string>
#include <iostream>
#include "colors.h"
#include "dmake.h"
#include "languages.h"

int main (int argc, char* argv[])
{
	FSNode* rootDirectory = new FSNode;
	rootDirectory->name = "./src";
	
	Scan (rootDirectory);
	
	Language language = FindLanguage ();
	
	Project* project;
	project = GenerateProject (&language);
	
	CheckRequiredDirectories ();
	
	project->GenerateRequired ();
	
	project->Compile ();
	
	project->Clean ();
	
	return 0;
}