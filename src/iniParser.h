


#pragma once

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class IniParser
{
public:
	/// @brief Constructor
	IniParser(std::string& iniFile);

	/// @brief Destructor
	~IniParser();

	/// @brief Checks if INI exists
	bool exists() const;

#ifdef DEFINE_APP
	/// @brief Applies INI file to settings.
	/// NOTE: This was a base class where "apply" is a derived class. For
	/// this application, this is where everything is set
	bool apply() = 0;

	/// @brief Writes current settings to INI file
	/// NOTE: This was a base class where "write" is defined in derived classes.
	bool write() = 0;
#endif

	/// @brief Retrieve integer
	int getInt(const std::string& key_name, const int defValue);

	/// @brief Retrieves string
	std::string getString(const std::string& key_name, const char* defValue);

	/// @brief Retrieves double
	double getFloat(const std::string& key_name, const double defValue);

	/// @brief Retrieves bool
	bool getBool(const std::string& key_name, const bool defValue);

	void putInt(const std::string& keyName, const int& value);
	void putFloat(const std::string& keyName, const double& value);
	void putString(const std::string& keyName, const std::string& strValue);
	void putBool(const std::string& keyName, const bool flag);

	/// @brief Opens INI file.
	/// @return false if there was a failure
	bool openIni();

	/// @brief Constructs file name given path and filename
	static void constructFileName(	std::string& iniFile,
									const std::string& path,
									const std::string& fname,
									const bool savePath = false);

	/// @brief Check if path exists - construct if path does not exist
	static bool pathExists(const std::string& path, const bool constructPath = false);

	/// @brief Checks file exists - fstat.h in come cases
	static bool exists(const std::string& fileName);

	/// @brief Forces writing default INI file (if true) if none found
	static bool s_forceUseIni;

private:
	/// @brief Creates paths one at a time
	static bool createPath(const std::string& path);

	/// @brief Property Tree
	boost::property_tree::ptree m_pt;

	/// @brief File was created or opened
	bool m_opened;

	/// @brief INI file was read or forced to be written
	bool m_fileWasRead;

	/// @brief Contents changed flag
	bool m_changed;

	/// @brief Default File name used to change
	std::string m_iniFile;

	static std::string m_defaultPath;
	static std::string m_defaultFile;

};