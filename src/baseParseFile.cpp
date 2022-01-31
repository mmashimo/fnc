/// @file
///
/// @brief Class implementation file for Parsing File Utility Base Class.
///
/// This class is used for generic file parsing, but here, we added (few)
/// functions specific to FNC project just so we can keep this somewhere safe.
/// (NOTE: use of FNC_PROJECT - for 'fnc' project)
///
/// Much of the original code (written in C) has been modified for C++
/// but modified for MS-VC from gcc and returned back to gcc. There are known
/// issues and dissimilarities that are not fully vetted.
///
/// @copyright 2009-2022 - M.Mashimo and all licensors. All rights reserved.
///
///  This program is free software: you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation, either version 3 of the License, or
///  any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///  You should have received a copy of the GNU General Public License
///  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iostream>
#include <exception>
#include <sys/stat.h>

#ifdef BOOST_FILE
 #include <boost/filesystem.hpp>
#endif
#ifdef EXPERIMENTAL_FILE
 #include <experimental/filesystem>
#endif

#define USE_CURRENT_DIR 
#ifdef USE_CURRENT_DIR
 #include <cstdio>
 #include <cstdlib>
 #include <deque>
#endif

#ifdef _MSC_VER
#include <direct.h>
#endif

#include "baseParseFile.h"

#include "exec.h"
#include "functions.h"

// static
bool BaseParseFile::m_debugErrors = false;

// static
std::string BaseParseFile::m_defaultPath;

// static
std::string BaseParseFile::m_defaultFile;

BaseParseFile::BaseParseFile(std::string& iniFile)
	: m_fileWasRead{false}
	, m_opened{false}
	, m_changed{false}
	, m_fileName(iniFile)
{
}

BaseParseFile::BaseParseFile(const std::string& path, const std::string& fname, const bool savePath)
	: m_fileWasRead{false}
	, m_opened{false}
	, m_changed{false}
{
	constructFileName(m_fileName, path, fname, savePath);
}



BaseParseFile::~BaseParseFile()
{
	if (m_changed)
	{
		// Write INI file
		write_ini(m_fileName, m_pt);
	}
}


bool BaseParseFile::exists() const
{
	return exists(m_fileName);
}

// static
void BaseParseFile::constructFileName(	std::string& iniFile,
									const std::string& path,
									const std::string& fname,
									const bool savePath)
{
	std::string tmpPath = path;
	if (path.empty())
	{
		iniFile = fname;
	}
	else if (path[0] == '~')
	{
		std::string tmp = path.substr(1);

		homeDir(iniFile);
		iniFile += tmp;
		if (iniFile.back() != '/')
			iniFile += '/';
		tmpPath = iniFile;
		iniFile += fname;		
	}
	else
	{
		// Path exists
		iniFile = path;
		if (path.back() != '/')
			iniFile += '/';
		tmpPath = iniFile;
		iniFile += fname;
	}
	if (savePath)
	{
		m_defaultPath = tmpPath;
		m_defaultFile = fname;
	}
}

// static
bool BaseParseFile::exists(const std::string& fileName)
{
	struct stat status;

	return stat(fileName.c_str(), &status) == 0;
}

// static
bool BaseParseFile::pathExists(const std::string& path, const bool constructPath)
{
	if (path.empty())
		return true;

	struct stat status;

	bool itsThere = stat(path.c_str(), &status) == 0;

	if (!itsThere && constructPath)
	{
#if 0
		// Remove last delimiter
#if 0
		std::string dir = path;
		size_t sz = dir.size();
		if ((dir.back() == '/') || (dir.back() == '\\'))
		{
			dir.resize(sz-1);
		}
#endif

#ifdef EXPERIMENTAL_FILE
		std::stringstream dir;
		dir << path;
		if (!std::experimental::filesystem::exists(dir.str()))
		{
			std::experimental::filesystem::create_directories(dir.str());
			itsThere = true;
		}
#endif

#ifdef USE_CURRENT_DIR
		std::deque<std::string> paths;
		std::string tmp = path;
		std::string tmp0;
		while(!tmp.empty())
		{
			if (tmp[0] == '/')
			{
				tmp0 += '/';
				tmp = tmp.substr(1);
			}
			else if (tmp[0] == '~')
			{
				// replace with "HOME"
				tmp0 += getenv("HOME");
				paths.push_back(tmp0);
				tmp = tmp.substr(1);
				tmp0.clear();
			}
			else
			{
				size_t pos = tmp.find_first_of('/');
				if (pos == std::string::npos)
				{
					// No more delimiter found
					paths.push_back(tmp);
				}
				else
				{
					tmp0 += tmp.substr(0,pos);
					paths.push_back(tmp0);
					tmp0.clear();			
					tmp = tmp.substr(pos);
				}
			}
		}
		tmp.clear();
		do {
			tmp += paths.front();
			paths.pop_front();
		 	if (stat(tmp.c_str(), &status) == 0)
			 {
				 // Skip
				 std::cout << "'" << tmp << "' - exists, moving on" << std::endl;
			 }
			 else
			 {
				 int check = mkdir(tmp.c_str(), 0777);
				 if (check < 0)
				 {
					 std::cout << "!Error creating '" << tmp << "' - returned " << check << std::endl;
					 itsThere = false;
					 break;
				 }
				 else
				 {
					 std::cout << "Created '" << tmp << "' directory" << std::endl;
					 itsThere = true;
				 }
			 }
		} while (!paths.empty());

#endif

#if 0
		// Create path - NOTE: This does not make recursive directories, so beware
		// int check = mkdir(path.c_str(), 0777);
		int check = mkdir("../../.fnc", 0777);
		if ( check == 0)
		{
			// Path created - reopen INI file
			itsThere = true;
		}
#endif

#endif
		itsThere = createPath(path);
	}

	return itsThere;
}



bool BaseParseFile::createPath(const std::string& path)
{
	bool itsThere = false;

#if 0
	// Remove last delimiter
	std::string dir = path;
	size_t sz = dir.size();
	if ((dir.back() == '/') || (dir.back() == '\\'))
	{
		dir.resize(sz-1);
	}
#endif

#ifdef EXPERIMENTAL_FILE
	std::stringstream dir;
	dir << path;
	if (!std::experimental::filesystem::exists(dir.str()))
	{
		std::experimental::filesystem::create_directories(dir.str());
		itsThere = true;
	}
#endif

#ifdef USE_CURRENT_DIR
	struct stat status;
	std::deque<std::string> paths;
	std::string tmp = path;
	std::string tmp0;
	while(!tmp.empty())
	{
		if (tmp[0] == '/')
		{
			tmp0 += '/';
			tmp = tmp.substr(1);
		}
		else if (tmp[0] == '~')
		{
			// replace with "HOME"
			std::string dirStr;
			homeDir(dirStr);
			tmp0 += dirStr;
			paths.push_back(tmp0);
			tmp = tmp.substr(1);
			tmp0.clear();
		}
		else
		{
			size_t pos = tmp.find_first_of('/');
			if (pos == std::string::npos)
			{
				// No more delimiter found
				paths.push_back(tmp);
			}
			else
			{
				tmp0 += tmp.substr(0,pos);
				paths.push_back(tmp0);
				tmp0.clear();			
				tmp = tmp.substr(pos);
			}
		}
	}
	tmp.clear();
	do {
		tmp += paths.front();
		paths.pop_front();
		if (stat(tmp.c_str(), &status) == 0)
			{
				// Skip
				std::cout << "'" << tmp << "' - exists, moving on" << std::endl;
			}
			else
			{
#ifdef _MSC_VER
				int check = _mkdir(tmp.c_str());
#else
				int check = mkdir(tmp.c_str(), 0777);
#endif
				if (check < 0)
				{
					std::cout << "!Error creating '" << tmp << "' - returned " << check << std::endl;
					itsThere = false;
					break;
				}
				else
				{
					std::cout << "Created '" << tmp << "' directory" << std::endl;
					itsThere = true;
				}
			}
	} while (!paths.empty());

#endif

#if 0
		// Create path - NOTE: This does not make recursive directories, so beware
		// int check = mkdir(path.c_str(), 0777);
		int check = mkdir("../../.fnc", 0777);
		if ( check == 0)
		{
			// Path created - reopen INI file
			itsThere = true;
		}
#endif
	return itsThere;
}


bool BaseParseFile::homeDir(std::string& dir)
{
#ifdef _MSC_VER
	size_t retSize = 1024;
	char buf[1024];
	errno_t retCode = getenv_s(&retSize, buf, 1024, "HOME");
	return std::string(buf);
#else
	dir = getenv("HOME");
	return true;
#endif
}

#if 0
static std::string homeDir()
{
#ifdef _MSC_VER
	size_t retSize = 1024;
	char buf[1024];
	errno_t retCode = getenv_s(&retSize, buf, 1024, "HOME");
	return std::string(buf);
#else
	return std::string(getenv("HOME"));
#endif
}
#endif