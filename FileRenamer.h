#ifndef FILERENAMER_H_INCLUDED
#define FILERENAMER_H_INCLUDED

#include <string>
#include <list>
#include <boost/regex.hpp>

class FileRenamer
{
public:
	FileRenamer(){}
	FileRenamer(std::wstring filePattern, std::wstring renamePattern, bool includeSubDirectories=false)
	{
		Rename(filePattern, renamePattern, includeSubDirectories);
	}
	unsigned int Rename(std::wstring filePattern, std::wstring renamePattern, bool includeSubDirectories=false);
};

#endif // FILERENAMER_H_INCLUDED
