#include "dmake.h"







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
	for (int i = 0; i < total_files.size(); i++)
	{
		FSFile* file = total_files[i];
		if (file->extension == "cpp")
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
	
	pstring objects;
	
	for (int i = 0; i < this->cppFiles.size(); i++)
	{
		FSFile* cppFile = this->cppFiles[i];
		
		// COMPILE CPP FILE
		cout << title << " Compiling " << cppFile->name << ".cpp -> "
			<< cppFile->extension << ".o" << endl;
		
		string path;
		cppFile->parent->CollectParentNameRecursive(&path);
		
		string path_no_root;
		cppFile->parent->CollectParentNameRecursiveExcludeRoot(&path_no_root);
		
		pstring command;
		command << CPP_COMPILER_GNU_LINUX << " -w -c " << path << cppFile->name << ".cpp -o ./obj/"
				 << cppFile->name << ".o";
		
		system (command.c_str());
		
		objects << "./obj/" << cppFile->name << ".o ";
	}
	
	pstring link_command;
	link_command << CPP_COMPILER_GNU_LINUX << " -w " << objects << "-o "
					<< OUTPUT_DIRECTORY << OUTPUT_EXECUTABLE_NAME;
	
	cout << title << " Linking object files -> " << OUTPUT_DIRECTORY
			<< OUTPUT_EXECUTABLE_NAME << endl;
	
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


void CheckRequiredDirectories ()
{
	cout << BOLD << WHITE << "Initializing..." << endl;
	
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

