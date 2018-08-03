/*
    Copyright Joseph Miller (C) 2014-2018.
*/
#include <map>
#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "FileRenamer.h"


/** Base constructor does nothing.
*/
FileRenamer::FileRenamer()
{
}


/** Overloaded constructor renames on instantiation.

@param[in] filePattern is the regular expression to use for determining which files to rename.
@param[in] renamePattern is the regular expression pattern to rename files with.
@param[out] errorString is the error string for issues.
@param[in] includeSubDirectories is used for recursive renames.
    @note default is false.
*/
FileRenamer::FileRenamer( const std::wstring & filePattern, const std::wstring & renamePattern,
                          std::wstring& errorString, const bool includeSubDirectories )
{
    Rename(filePattern, renamePattern, errorString, includeSubDirectories);
}


/** Rename iterates through the file list, checks for regex matches and replaces those matches with regex formatted file names.

@param[in] filePattern is the regular expression to use for determining which files to rename.
@param[in] renamePattern is the regular expression pattern to rename files with.
@param[out] errorString is the error string for issues.
@param[in] includeSubDirectories is used for recursive renames.
    @note default is false.
@return the number of files that were renamed.
*/
unsigned int FileRenamer::Rename( const std::wstring & filePattern, const std::wstring & renamePattern,
                                  std::wstring & errorString, const bool includeSubDirectories ) const
{
	//mapOutput is used to hold original filenames and renamed filenames
	std::map<std::wstring, std::wstring> mapOutput;

	GetFileMap(mapOutput, includeSubDirectories);
	//if there aren't any regular files to rename in the directories, go away
	if(mapOutput.empty())
	{
	    errorString = L"No files to rename.\n";
		return 0;
	}

	unsigned int filesRenamed=0; //return value

	for (auto& mit : mapOutput)
    {
        mit.second = regex_replace(mit.first, boost::wregex(filePattern), renamePattern);
		//if the filename was changed with the pattern...
		if(mit.first != mit.second)
		{
			++filesRenamed;
		}
	}

	DuplicateFilenameCheck(mapOutput, errorString);

	//if there is a warning, throw it, otherwise,
	//start renaming the files and throw an error if we've received one
	if(errorString.empty())
	{
	    for(auto& mit : mapOutput)
        {
			try
			{
				boost::filesystem::rename(mit.first, mit.second);
			}
			catch(...)
			{
				throw;
			}
        }
		return filesRenamed;
	}

	return 0;
}


/** This checks for duplicate filenames between original and files to rename to.

@param[in] mapOutput the file map to check input and output for.
@param[out] errorString the error string if there are duplicate files.
*/
void FileRenamer::DuplicateFilenameCheck(const std::map<std::wstring, std::wstring>& mapOutput, std::wstring& errorString) const
{
    //detect if output filename will match another output filename or if the change isn't a valid filename
	//if true, issue a warning
	for(auto mit = mapOutput.begin();mit != mapOutput.end();++mit)
	{
		for(auto mit2 = next(mit);mit2 != mapOutput.end();++mit2)
		{
			if(mit->second == mit2->second)
			{
				errorString += std::wstring(L"filename: ") + mit->second + std::wstring(L" has multiple matches.\n");
				continue;
			}

            const std::string checkValidity = std::string(mit2->second.begin(), mit2->second.end());
            if(!boost::filesystem::portable_name(checkValidity))
            {
                errorString += std::wstring(L"filename: ") + mit2->second + std::wstring(L" will not be a valid filename.\n");
            }
		}
	}
}


/** This pulls in the file map.

@param[out] mapOutput the file map containing all files in the directory or directories.
@param[in] includeSubDirectories should be true if you wish to recursively include files in subdirectories.
*/
void FileRenamer::GetFileMap(std::map<std::wstring, std::wstring>& mapOutput, const bool includeSubDirectories) const
{
    //TODO: this may need fixed.  Current behavior is that the pattern renamer is going to
	//take the whole pathname into consideration instead of JUST the filename

	if(includeSubDirectories)
	{
		boost::filesystem::recursive_directory_iterator rdi(L".");
		while(rdi != boost::filesystem::recursive_directory_iterator())
		{
			if(boost::filesystem::is_regular_file(*rdi))
			{
				//add the original filename to mapOutput
				const std::wstring out = (*rdi).path().filename().wstring();
				mapOutput.insert(std::pair<std::wstring, std::wstring>(out, out));
			}
			++rdi;
		}
	}
	else
	{
		boost::filesystem::directory_iterator di(L".");
		while(di != boost::filesystem::directory_iterator())
		{
			if(boost::filesystem::is_regular_file(*di))
			{
				//add the original filename to mapOutput
				const std::wstring out = (*di).path().filename().wstring();
				mapOutput.insert(std::pair<std::wstring, std::wstring>(out, out));
			}
			++di;
		}
	}
}
