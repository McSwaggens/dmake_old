#include "dmake.h"


string GetProjectName ()
{
	string workingDirectory = GetWorkingDirectory ();
	
	vector<string> _split = Split (workingDirectory, '/');
	
	string projectName = _split[_split.size()-1];
	
	return projectName;
}

string GetWorkingDirectory ()
{
	char cCurrentPath[FILENAME_MAX];
	getcwd(cCurrentPath, sizeof(cCurrentPath));
	return string(cCurrentPath);
}

bool IsRoot ()
{
	return getuid() == 0;
}

bool Install ()
{
	pstring title;
	title << BOLD << CYAN << "[" << MAGENTA << "INST" << CYAN << "]" << RESET;
	
	cout << title << " Installing..." << RESET << endl;
	
	if (!IsRoot())
	{
		cout 	<< title
				<< BOLD << CYAN << " [" << RED << "ERR!" << CYAN << "]" << RESET
				<< " You must be root user to install using dmake." << endl;
		
		return false;
	}
	
	// Get the binary to copy
	DIR* dir;
	dirent* pdir;
	
	dir = opendir("./bin/");
	
	vector<string> binaryFiles;
	
	while (pdir = readdir(dir))
	{
		if (pdir->d_type == DT_REG)
		{
			binaryFiles.push_back (pdir->d_name);
		}
	}
	
	/*
		Only works for binary files as of this moment!
		This code will soon have to check whether or not it is a binary file or a library
														(eg. .so for linux and .dylib for OSX)
	*/
	for (int i = 0; i < binaryFiles.size (); i++)
	{
		string file = binaryFiles[i];
		
		pstring command;
		command << "cp ./bin/" << file << " " << INSTALL_DIRECTORY;
		
		//cout << command << endl;
		
		cout << title << " " << file << " -> " << INSTALL_DIRECTORY << endl;
		
		system (command.c_str());
	}
	
	/* 	TODO:
		
		In the future we want to be able to install libraries
					(eg. .so for linux and .dylib for OSX)
		
		we also want to be able to install libraries along side of executables
		
		permission changing would also be nice
		
	*/
	
	cout << title << " Success!" << endl;
	
	return true;
}


void Run ()
{
	string projectName = GetProjectName();
	pstring binaryPath;
	binaryPath << OUTPUT_DIRECTORY << projectName;
	
	system (binaryPath.c_str());
}


void ProjectCSharp::GenerateRequired ()
{
}

void ProjectCSharp::Compile ()
{
	pstring title;
	title << BOLD << CYAN << "[" << MAGENTA << "C#" << CYAN << "]" << RESET;
}


void ProjectCSharp::Clean ()
{
}


void ProjectCPP::GenerateRequired ()
{
	this->libs = new string("");
	
	if (FileExists ("./", "dlibs"))
	{
		this->libs = new string(Parser::GenerateGPPLibraryParameters ("./dlibs"));
	}
	
	for (int i = 0; i < total_files.size(); i++)
	{
		FSFile* file = total_files[i];
		if (file->extension == "cpp")
		{
			this->cppFiles.push_back (file);
		}
		if (file->extension == "c") // Treat C files as CPP files
		{
			this->cppFiles.push_back (file);
		}
		else
		if (file->extension == "h")
		{
			this->headerFiles.push_back (file);
		}
	}
}

void ProjectCPP::Compile ()
{
	pstring title;
	title << BOLD << CYAN << "[" << MAGENTA << "CPP" << CYAN << "]" << RESET;
	
	projectName = GetProjectName ();
	
	pstring objects;
	
	for (int i = 0; i < this->cppFiles.size(); i++)
	{
		FSFile* cppFile = this->cppFiles[i];
		
		// COMPILE CPP FILE
		cout << title << " Compiling " << cppFile->name << "." << cppFile->extension << " -> "
			<< cppFile->name << ".o" << endl;
		
		string path;
		cppFile->parent->CollectParentNameRecursive(&path);
		
		string path_no_root;
		cppFile->parent->CollectParentNameRecursiveExcludeRoot(&path_no_root);
		
		pstring command;
		command << CPP_COMPILER_GNU_LINUX << " -w -c " << path << cppFile->name << "." << cppFile->extension << " " << *this->libs << "-o ./obj/"
				 << cppFile->name << ".o";
		
		//Uncomment to see the compile commands sent to the shell
		//cout << title << BOLD << YELLOW << " COMPILE->" << command << RESET << endl;
		
		system (command.c_str());
		
		objects << "./obj/" << cppFile->name << ".o ";
	}
	
	pstring link_command;
	link_command << CPP_COMPILER_GNU_LINUX << " -w " << objects << *this->libs << "-o "
					<< OUTPUT_DIRECTORY << projectName;
	
	cout << title << " Linking object files -> " << OUTPUT_DIRECTORY
			<< projectName << endl;
	
	//Uncomment to see the compile commands sent to the shell
	//cout << title << BOLD << YELLOW << " linker->" << link_command << RESET << endl;
	
	system (link_command.c_str());
}


void ProjectCPP::Clean ()
{
	pstring title;
	title << BOLD << CYAN << "[" << MAGENTA << "CPP" << CYAN << "]" << RESET;
	
	cout << title << " Cleaning up..." << endl;
	
	system ("rm -rf ./obj/");
}


void FSNode::CollectParentNameRecursive (string* current)
{
	current->insert(0, "/");
	current->insert(0, this->name);
	if (parent != NULL)
	{
		parent->CollectParentNameRecursive(current);
	}
}

void FSNode::CollectParentNameRecursiveExcludeRoot (string* current)
{
	if (parent != NULL)
	{
		current->insert(0, "/");
		current->insert(0, this->name);
		parent->CollectParentNameRecursiveExcludeRoot(current);
	}
}

FSFile* ToFSFile (dirent* dir)
{
	FSFile* file = new FSFile;
	
	vector<string> split = Split (string(dir->d_name), '.');
	
	file->name = split[0];
	
	if (split.size() > 1)
	{
		file->extension = split[split.size()-1];
		
		if (file->extension == "cpp" || file->extension == "h")
		{	// File is of the C++ family
			tally_cpp++;
		}
		else
		if (file->extension == "cs")
		{	// File is of the C# family
			tally_cs++;
		}
	}
	
	return file;
}


bool CheckRequiredDirectories ()
{
	cout << BOLD << WHITE << "Initializing..." << endl;
	
	if (!DirectoryExists("./src/"))
	{
		cout 	<< BOLD << CYAN << "[" << RED << "ERR!" << CYAN << "]" << RESET
				<< " You aren't inside of a valid dmake project directory!"
				<< endl << "...Missing ./src/ directory..." << endl;
		
		return false;
	}
	
	/* Binary Directory */
	if (DirectoryExists("./bin/"))
	{
		system ((pstring("rm -rf ") << OUTPUT_DIRECTORY).c_str());
	}
	system ((pstring("mkdir ") << OUTPUT_DIRECTORY).c_str());
	
	/* Object Directory */
	if (DirectoryExists("./obj/"))
	{
		system ("rm -rf ./obj/");
	}
	system ("mkdir ./obj/");
	
	return true;
}


bool DirectoryExists( const char* pzPath )
{
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;    
        (void) closedir (pDir);
    }

    return bExists;
}

bool FileExists (string directory, string file)
{
    DIR* dir;
	dirent* pdir;
	
	dir = opendir(directory.c_str ());
	
	while (pdir = readdir(dir))
	{
		if (pdir->d_type == DT_REG && pdir->d_name == file)
		{
			return true;
		}
	}
	
	return false;
}


vector<string> Split (string str, char splitter)
{
	vector<string> result;
	
	string current = "";
	
	for (int i = 0; i < str.size(); i++)
	{
		char c = str[i];
		if (c == splitter)
		{
			result.push_back (current);
			current = "";
		}
		else
		{
			current += c;
		}
	}
	
	if (current != "")
	{
		result.push_back (current);
	}
	
	return result;
}


void Scan (FSNode* node)
{
	string path;
	node->CollectParentNameRecursive(&path);
	//cout << BOLD << GREEN << "SCANNING " << path << RESET << endl;
	
	DIR* dir;
	dirent* pdir;
	
	dir = opendir(path.c_str());
	
	vector<FSNode*> directories;
	vector<FSFile*> files;
	
	while (pdir = readdir(dir))
	{
		if (string(pdir->d_name) == "." || string(pdir->d_name) == "..") // ignore current directory and parent directory
		{
			continue; // continue to the next read
		}
		
		if (pdir->d_type == DT_DIR) // directory
		{
			//cout << BOLD << BLUE << pdir->d_name << RESET << endl;
			FSNode* new_node = new FSNode;
			new_node->name = pdir->d_name;
			new_node->parent = node;
			directories.push_back (new_node);
		}
		else // regular file
		{
			//cout << BOLD << RED << pdir->d_name << RESET << endl;
			FSFile* file = ToFSFile (pdir);
			file->parent = node;
			files.push_back (file);
			total_files.push_back(file);
		}
	}
	
	closedir (dir);
	
	for (int i = 0; i < directories.size(); i++)
	{
		FSNode* node = directories[i];
		Scan (node);
	}
}

Language FindLanguage ()
{
	Language language = UNKNOWN;
	
	if (tally_cpp > 0)
	{
		language = CPP;
	}
	
	if (tally_cs > tally_cpp)
	{
		language = CSHARP;
	}
	
	return language;
}


Project* GenerateProject (Language* language)
{
	Project* project;
	
	cout << BOLD << WHITE << "Project type is ";
	
	if (*language == CPP)
	{
		cout << "C++";
		project = new ProjectCPP;
	}
	else if (*language == CSHARP)
	{
		cout << "C#";
		project = new ProjectCSharp;
	}
	else
	{
		cout << BOLD << RED << "[ERROR] UNKNOWN LANGUAGE EXCEPTION!" << RESET << endl;
		throw;
	}
	
	cout << RESET << endl;
	
	return project;
}

