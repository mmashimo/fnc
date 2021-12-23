/// @file
///
/// @brief Header for Parsing File Utility Base Class
///
/// This class is used for generic file parsing util, but here, we added (few)
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

#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class BaseParseFile
{
public:
	/// @brief Constructor
	BaseParseFile(std::string& fileName);

	/// @brief Construct using path and file name contents
	BaseParseFile(const std::string& path,
			const std::string& fname,
			const bool savePath = false);

	/// @brief Destructor
	virtual ~BaseParseFile();

	/// @brief Accessor to file name used for this iniParser
	const std::string& getFileName() const { return m_fileName; }

	/// @brief Checks if INI exists
	bool exists() const;

	/// @brief Is file opened?
	/// @return true if file is opened
	bool isFileOpened() const { return m_opened; }

	/// @brief Checks if file hase changed
	/// @return true if there is file change
	bool isFileChanged() const { return m_changed; }

	/// @brief Sets File changed flag
	/// @param[in] changed
	void setFileChanged(bool changed = true) { m_changed = changed; }

	/// @brief Was File Read?
	/// @return true if fhile was read
	bool wasFileRead() const { return m_fileWasRead; }

	/// @brief Constructs file name given path and filename
	static void constructFileName(	std::string& fileName,
									const std::string& path,
									const std::string& fname,
									const bool savePath = false);

	/// @brief Check if path exists - construct if path does not exist
	static bool pathExists(const std::string& path, const bool constructPath = false);

	/// @brief Checks file exists - fstat.h in come cases
	static bool exists(const std::string& fileName);

	/// @brief Returns default path name
	static const std::string& getDefaultPathName() { return m_defaultPath; }


protected:
	/// @brief Creates paths one at a time
	static bool createPath(const std::string& path);

	/// @brief Can we debug errors?
	/// return true if we can debug Errors
	bool canDebugErrors() const { return m_debugErrors; }

	/// @brief File was created or opened
	bool m_opened;

	/// @brief INI file was read or forced to be written
	bool m_fileWasRead;

	/// @brief Contents changed flag
	bool m_changed;

	/// @brief Default File name used to change
	std::string m_fileName;


private:
	/// @brief Property Tree
	boost::property_tree::ptree m_pt;

	/// @brief Debug input/output if errors occur
	static bool m_debugErrors;

	static std::string m_defaultPath;
	static std::string m_defaultFile;

};