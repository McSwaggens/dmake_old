#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include "colors.h"
#include "languages.h"
#include "pstring.h"

using namespace std;

/*---- DEFINES ----*/
#define CPP_COMPILER_GNU_LINUX "g++"
#define CS_COMPILER_GNU_LINUX "xbuild"
#define OUTPUT_DIRECTORY "./bin/"
#define OUTPUT_EXECUTABLE_NAME "program" // To be changed into static variable

/*---- LANGUAGE TALLY ----*/
static int tally_cpp;
static int tally_cs;

/*---- Structs ----*/
struct Project;
struct FSFile;
struct FSNode;

struct Project
{
	virtual void Compile () {}
	virtual void GenerateRequired () {}
	virtual void Clean () {}
};

struct ProjectCPP : public Project
{
	virtual void Compile ();
	virtual void GenerateRequired ();
	virtual void Clean ();
	vector<FSFile*> cppFiles;
	vector<FSFile*> headerFiles;
};

struct ProjectCSharp : public Project
{
	virtual void Compile ();
	virtual void GenerateRequired ();
	virtual void Clean ();
	FSFile* solutionFile;
};

struct FSFile
{
	string name;
	string extension;
	FSNode* parent;
};

struct FSNode
{
	string name;
	FSNode* childNodes;
	FSFile* childFiles;
	FSNode* parent;
	
	void CollectParentNameRecursive (string* current);
	void CollectParentNameRecursiveExcludeRoot (string* current);
};


/*---- Static Variables ----*/
static Project* project;

static vector<FSFile*> total_files;


/*---- Methods ----*/
FSFile* ToFSFile (dirent* dir);

void CheckRequiredDirectories ();

void ScanProjectType ();

void Scan (FSNode* node);

bool DirectoryExists(const char* pzPath);

vector<string> Split (string str, char splitter);

Language FindLanguage ();

Project* GenerateProject (Language* language);