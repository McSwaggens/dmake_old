#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include "colors.h"
#include "languages.h"
#include "pstring.h"
#include "parser.h"

using namespace std;

/*---- DEFINES ----*/
#define CPP_COMPILER_GNU_LINUX "g++"
#define CS_COMPILER_GNU_LINUX "xbuild"
#define OUTPUT_DIRECTORY "./bin/"
#define INSTALL_DIRECTORY "/usr/bin/" // Unix install directory

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
	string* libs;
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

static string projectName;


/*---- Methods ----*/

string GetProjectName ();

string GetWorkingDirectory ();

bool Install ();

void Clean ();

void Run ();

FSFile* ToFSFile (dirent* dir);

bool CheckRequiredDirectories ();

void ScanProjectType ();

void Scan (FSNode* node);

bool DirectoryExists(const char* pzPath);

bool FileExists (string directory, string file);

vector<string> Split (string str, char splitter);

Language FindLanguage ();

Project* GenerateProject (Language* language);

