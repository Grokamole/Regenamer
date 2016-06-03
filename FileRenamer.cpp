/*
    Copyright Joseph Miller (C) 2014-2016.
*/
#include <map>
#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "FileRenamer.h"

FileRenamer::FileRenamer()
{

}

FileRenamer::FileRenamer( const std::wstring & filePattern, const std::wstring & renamePattern,
            const bool & includeSubDirectories )
{
    Rename(filePattern, renamePattern, includeSubDirectories);
}


/*
	Rename iterates through the file list, checks for regex matches and replaces those matches with
	regex formatted file names.  This returns the number of files that were renamed.
*/

unsigned int FileRenamer::Rename( const std::wstring & filePattern, const std::wstring & renamePattern,
                                  const bool & includeSubDirectories )
{
	//mapOutput is used to hold original filenames and renamed filenames
	std::map<std::wstring, std::wstring> mapOutput;

	//TODO: this still needs fixed.  Current behavior is that the pattern renamer is going to
	//take the whole pathname into consideration instead of JUST the filename

	if(includeSubDirectories==true)
	{
		boost::filesystem::recursive_directory_iterator rdi(L".");
		while(rdi != boost::filesystem::recursive_directory_iterator())
		{
			if(boost::filesystem::is_regular_file(*rdi))
			{
				//add the original filename to mapOutput
				std::wstring out = (*rdi).path().filename().wstring();
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
				std::wstring out = (*di).path().filename().wstring();
				mapOutput.insert(std::pair<std::wstring, std::wstring>(out, out));
			}
			++di;
		}
	}

	//if there aren't any regular files to rename in the directories, go away
	if(mapOutput.empty() == true)
	{
		return 0;
	}

	unsigned int filesRenamed=0; //return value

	auto mit = mapOutput.begin();

	//rename the files, if we can
	for(;mit != mapOutput.end();++mit)
	{
		mit->second = regex_replace(mit->first, boost::wregex(filePattern), renamePattern);
		//if the filename was changed with the pattern...
		if(mit->first != mit->second)
		{
			++filesRenamed;
		}
	}
	//detect if output filename will match another output filename or if the change isn't a valid filename
	//if true, issue a warning
	std::wstring warn;
	for(mit = mapOutput.begin();mit != mapOutput.end();++mit)
	{
		for(auto mit2 = next(mit);mit2 != mapOutput.end();++mit2)
		{
			if(mit->second == mit2->second)
			{
				warn += std::wstring(L"filename: ") + mit->second + std::wstring(L" has multiple matches.\n");
			}
			else
			{
				std::string checkValidity = std::string(mit2->second.begin(), mit2->second.end());
				if(!boost::filesystem::portable_name(checkValidity))
				{
					warn += std::wstring(L"filename: ") + mit2->second + std::wstring(L" will not be a valid filename.\n");
				}
			}
		}
	}

	//if there is a warning, throw it, otherwise,
	//start renaming the files and throw an error if we've received one
	if(warn.empty())
	{
		for(mit = mapOutput.begin();mit != mapOutput.end();++mit)
		{
			try
			{
				boost::filesystem::rename(mit->first, mit->second);
			}
			catch(...)
			{
				throw;
				//error
			}
		}
		return filesRenamed;
	}
	else
	{
		/*
		  note: yes, I fully realize that throwing a string is considered bad
		  practice, but the information gleamed from this can be helpful in
		  understanding what might be wrong with your regex pattern design and
		  so the tradeoff vs. memory issues is imho, acceptable
		*/
		throw warn;
	}
}
