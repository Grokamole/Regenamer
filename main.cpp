/*
    Copyright Joseph Miller (C) 2014-2016.
*/
#include <iostream>
#include <regex>
#include <string>
#include <string.h>
#include <boost/filesystem.hpp>
#include <cstdlib>

#include "FileRenamer.h"

const std::wstring program_description = L"Renames files in a directory based on a regex pattern";
const std::wstring argument_error = L"Error: Wrong number of arguments.  This program needs an input pattern and an output pattern.";
const std::wstring usage = L"Usage: Regenamer.exe \"file pattern\" \"rename pattern\"";
const std::wstring fpattern = L"\t \"file pattern\" is a standard regex search pattern.\n"
							   "\t \"rename pattern\" is a formatting string.  It can consist of: \n"
                               "\t\t $n - where n is equal to the substring in the pattern\n"
							   "\t\t\t(eg. $2 for the second substring)\n"
                               "\t\t $0 - the string matching the whole regex pattern.\n"
							   "\t\t $` - the substring preceding $0.\n"
							   "\t\t $' - the substring succeeding $0.";
const std::wstring version = L"Version 1.0.1";
const std::wstring creator = L"Joseph P. Miller";
const std::wstring programName = L"Regenamer";

int main(int argc, char *argv[])
{
	//Currently, we expect exactly 2 arguments: the input regex pattern and the renamed output regex pattern
	//TODO: Fix code for recursive subdirectory pattern renaming which means adding an additional option to
	//the command line and bringing the possible number of arguments above 3
	//TODO: Also, a version and help command would probably be helpful
	if(argc != 3)
	{
		if(argc > 1)
		{
			std::wcout << argument_error << std::endl;
		}

		//this works as a help screen for now
		std::wcout << programName << " " << version << " by " << creator << std::endl;
		std::wcout << usage << std::endl;
		std::wcout << fpattern << std::endl;
		return EXIT_SUCCESS;
	}

	try
	{
		//convert the patterns to strings
		FileRenamer fr;
		std::wstring filePattern = std::wstring(argv[1], argv[1] + strlen(argv[1]));
		std::wstring renamePattern = std::wstring(argv[2], argv[2] + strlen(argv[2]));
		//attempt to rename some files
		std::wcout << fr.Rename(filePattern, renamePattern, false) << L" files renamed." << std::endl;
	}
	catch (std::wstring error)
	{
		std::wcout << L"Error in " << programName << L": " << std::endl << error << std::endl;
	}
	catch (boost::filesystem::filesystem_error fse)
	{
		std::wcout << L"Filesystem error: " << fse.what() << std::endl;
	}
    return EXIT_SUCCESS;
}
