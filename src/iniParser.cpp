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

#include "iniParser.h"

#include "exec.h"
#include "functions.h"

// static
bool IniParser::s_forceUseIni = true;

IniParser::IniParser(std::string& iniFile)
	: BaseParseFile(iniFile)
	, m_iniFile(iniFile)
{
	openIni();
}

IniParser::IniParser(const std::string& path, const std::string& fname, const bool savePath)
	: BaseParseFile(path, fname, savePath)
{
	constructFileName(m_iniFile, path, fname, savePath);

	openIni();
}



IniParser::~IniParser()
{
	if (isFileChanged())
	{
		// Write INI file
		write_ini(m_iniFile, m_pt);
	}
}

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


int IniParser::getInt(const std::string& key_name, const int defValue)
{
	int value = defValue;
	if (wasFileRead())
	{
		try
		{
			value = m_pt.get<int>(key_name);
		}
		catch(std::exception& e)
		{
			if (canDebugErrors())
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
	if (wasFileRead())
	{
		try
		{
			value = m_pt.get<std::string>(key_name);
		}
		catch(std::exception& e)
		{
			if (canDebugErrors())
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
	if (wasFileRead())
	{
		try
		{
			value = m_pt.get<double>(key_name);
		}
		catch(std::exception& e)
		{
			if (canDebugErrors())
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
	if (wasFileRead())
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
	if (isFileOpened())
	{
		m_pt.put(keyName, value);
		setFileChanged(true);
	}
}

void IniParser::putFloat(const std::string& keyName, const double& value)
{
	if (isFileOpened())
	{
		m_pt.put(keyName, value);
		setFileChanged(true);
	}
}

void IniParser::putString(const std::string& keyName, const std::string& strValue)
{
	if (isFileOpened())
	{
		m_pt.put(keyName, strValue);
		setFileChanged(true);
	}
}

void IniParser::putBool(const std::string& keyName, const bool flag)
{
	if (isFileOpened())
	{
		std::string strFlag = flag?"true":"false";
		m_pt.put(keyName, strFlag);
		setFileChanged(true);
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
			if (pathExists(getDefaultPathName(), true))
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
				std::cout << "! Cannot create path '" << getDefaultPathName() << "' !" << std::endl;
			}
			std::cout << std::endl;
		}
	}

	// make sure we have opened flag set
	m_opened = ok;

	return ok;
}

