/*
    Copyright Joseph Miller (C) 2014-2018.
*/
#ifndef FILERENAMER_H_INCLUDED
#define FILERENAMER_H_INCLUDED


class FileRenamer
{
public:
    FileRenamer();
	FileRenamer(const std::wstring & filePattern, const std::wstring & renamePattern,
                std::wstring & errorString, const bool includeSubDirectories = false);
	unsigned int Rename(const std::wstring & filePattern, const std::wstring & renamePattern,
                        std::wstring & errorString, const bool includeSubDirectories = false) const;
private:
    void GetFileMap(std::map<std::wstring, std::wstring>& mapOutput, const bool includeSubDirectories) const;
    void DuplicateFilenameCheck(const std::map<std::wstring, std::wstring>& mapOutput, std::wstring& errorString) const;
};

#endif // FILERENAMER_H_INCLUDED
