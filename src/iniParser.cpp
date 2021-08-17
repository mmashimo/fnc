/// @file
///
/// @brief Class implementation file for INI-Parsing Utility.
///
/// This class is used for generic INI parsing, but here, we added (few)
/// functions specific to FNC project just so we can keep this somewhere safe.
/// (NOTE: use of FNC_PROJECT - for 'fnc' project)
///
/// Much of the original code (written in C) has been modified for C++
/// but modified for MS-VC from gcc and returned back to gcc. There are known
/// issues and dissimilarities that are not fully vetted.
///
/// @copyright 2019-2021 - M.Mashimo and all licensors. All rights reserved.
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

#include "iniParser.h"

#include "exec.h"
#include "functions.h"

// static
bool IniParser::m_debugErrors = false;

// static
bool IniParser::s_forceUseIni = true;
// static
std::string IniParser::m_defaultPath;
// static
std::string IniParser::m_defaultFile;

IniParser::IniParser(std::string& iniFile)
	: m_fileWasRead{false}
	, m_opened{false}
	, m_changed{false}
	, m_iniFile(iniFile)
{
	openIni();
}

IniParser::IniParser(const std::string& path, const std::string& fname, const bool savePath)
	: m_fileWasRead{false}
	, m_opened{false}
	, m_changed{false}
{
	constructFileName(m_iniFile, path, fname, savePath);

	openIni();
}



IniParser::~IniParser()
{
	if (m_changed)
	{
		// Write INI file
		write_ini(m_iniFile, m_pt);
	}
}

// #ifdef FNC_PROJECT
void IniParser::configure(Exec& exec)
{
	// By initializing ini file, INI file will be created
	if (exists())
	{
		FunctionType::s_defaultAngle = getString("Number.Angle", "deg");
		FunctionType::s_defaultExpression = getString("Number.Expression", "Dec");
		FunctionType::s_defaultFPP = getString("Number.FPP", "bin-64");

		Exec::s_showUndefinedVarMsg = 0 != getBool("Exec.UndefinedMsg", 1);
		Exec::s_quitMsgFirstTime = 0 != getBool("Exec.quitMsg", 1);
	}
	else if (IniParser::s_forceUseIni)
	{
		putString("Number.Angle", FunctionType::s_defaultAngle);
		putBool("Exec.UndefinedMsg", Exec::s_showUndefinedVarMsg);
		putBool("Exec.quitMsg", Exec::s_quitMsgFirstTime);
	}

}
// #endif

bool IniParser::exists() const
{
	return exists(m_iniFile);
}

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

// static
void IniParser::constructFileName(	std::string& iniFile,
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

		iniFile = homeDir();
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
bool IniParser::exists(const std::string& fileName)
{
	struct stat status;

	return stat(fileName.c_str(), &status) == 0;
}

// static
bool IniParser::pathExists(const std::string& path, const bool constructPath)
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

int IniParser::getInt(const std::string& key_name, const int defValue)
{
	int value = defValue;
	if (m_fileWasRead)
	{
		try
		{
			value = m_pt.get<int>(key_name);
		}
		catch(std::exception& e)
		{
			if (m_debugErrors)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	return value;
}

std::string IniParser::getString(const std::string& key_name, const char* defValue)
{
	std::string value = defValue;
	if (m_fileWasRead)
	{
		try
		{
			value = m_pt.get<std::string>(key_name);
		}
		catch(std::exception& e)
		{
			if (m_debugErrors)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	return value;
}


double IniParser::getFloat(const std::string& key_name, const double defValue)
{
	double value = defValue;
	if (m_fileWasRead)
	{
		try
		{
			value = m_pt.get<double>(key_name);
		}
		catch(std::exception& e)
		{
			if (m_debugErrors)
			{
				std::cout << e.what() << std::endl;
			}
			value = defValue;
		}
	}

	return value;
}

bool IniParser::getBool(const std::string& key_name, const bool defValue)
{
	bool value = defValue;
	if (m_fileWasRead)
	{
		std::string strFlag;
		try
		{
			strFlag = m_pt.get<std::string>(key_name);
			value = strFlag == "true";
		}
		catch(std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return value;
}


void IniParser::putInt(const std::string& keyName, const int& value)
{
	if (m_opened)
	{
		m_pt.put(keyName, value);
		m_changed = true;
	}
}

void IniParser::putFloat(const std::string& keyName, const double& value)
{
	if (m_opened)
	{
		m_pt.put(keyName, value);
		m_changed = true;
	}
}

void IniParser::putString(const std::string& keyName, const std::string& strValue)
{
	if (m_opened)
	{
		m_pt.put(keyName, strValue);
		m_changed = true;
	}
}

void IniParser::putBool(const std::string& keyName, const bool flag)
{
	if (m_opened)
	{
		std::string strFlag = flag?"true":"false";
		m_pt.put(keyName, strFlag);
		m_changed = true;
	}
}

bool IniParser::openIni()
{
	// OK if file exists, but not necessarily read
	bool ok = false;

	if (m_iniFile.empty())
	{
		std::cout << "! INI parser cannot open empty file name" << std::endl;
		// Cannot open empty file
		s_forceUseIni = false;
		m_opened = false;
		return false;
	}

	if(exists())
	{
		try
		{
			boost::property_tree::ini_parser::read_ini(m_iniFile, m_pt);
			m_fileWasRead = true;
			// Get all the configuration in INI file
#ifdef DEFINE_APP
			ok = apply();
#else
			ok = true;
#endif
		}
		catch(std::exception& e)
		{
			std::cout << e.what() << std::endl;
			std::cout << "Using file: '" << m_iniFile << "' -- exception caught at contructor" << std::endl;
			m_fileWasRead = false;
		}
	}
	else
	{
		std::cout << "INI file '" << m_iniFile << "' - not found." << std::endl;
		m_fileWasRead = false;

		if (s_forceUseIni)
		{
			if (pathExists(m_defaultPath, true))
			{
#ifdef DEFINE_APP
				std::cout << " Writing default.";
				ok = write();
#else
				std::cout << " Writing empty .";
#endif
				ok = true;
			}
			else
			{
				std::cout << "! Cannot create path '" << m_defaultPath << "' !" << std::endl;
			}
			std::cout << std::endl;
		}
	}

	// make sure we have opened flag set
	m_opened = ok;

	return ok;
}

bool IniParser::createPath(const std::string& path)
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
			tmp0 += homeDir();
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


