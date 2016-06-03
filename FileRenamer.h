/*
    Copyright Joseph Miller (C) 2014-2016.
*/
#ifndef FILERENAMER_H_INCLUDED
#define FILERENAMER_H_INCLUDED

#include <string>

class FileRenamer
{
public:
    FileRenamer();
	FileRenamer(const std::wstring & filePattern, const std::wstring & renamePattern,
                const bool & includeSubDirectories = false);
	unsigned int Rename(const std::wstring & filePattern, const std::wstring & renamePattern,
                        const bool & includeSubDirectories = false);
};

#endif // FILERENAMER_H_INCLUDED
