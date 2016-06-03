Regenamer
=========

A regex pattern file renamer for renaming all files within a directory.  Built with boost::filesystem.
Uses Boost libraries 1.60.0 natively in the code blocks project (should work with 1.55.0+ with small modification).
Make sure to change the .lib file locations if you use the project file.

Usage: Regenamer.exe "file pattern" "rename pattern"
	"file pattern" is a standard regex search pattern.
	"rename pattern" is a formatting string.  It can consist of:
		$n - where n is equal to the substring in the pattern
			(eg. $2 for the second substring)
		$0 - the string matching the whole regex pattern.
		$` - the substring preceding $0.
		$' - the substring succeeding $0.

Version 1.0.2 - Updated to use Boost 1.60.0.
Version 1.0.1 - Initial github version

By Joseph P. Miller
Please read the accompanied license
