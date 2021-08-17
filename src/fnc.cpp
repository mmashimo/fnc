/// @file
///
/// @brief This file is the main program entry for fnc application.
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
#include <cstring>

// Used to install the INI parser (Note use of FNC_PROJECT to access Exec)
#define FNC_PROJECT
#include "iniParser.h"

#include "exec.h"

// Following used to set the fnc-Configuration (version, etc.)
#include "fncConfig.h"
static std::string s_iniPath = FNC_INI_LOCATION;
static std::string s_iniFileName = FNC_INI_FILENAME; // "fnc.ini";
static std::string s_iniFile;


/// @brief Prints version number - make sure CMake is run with configuration
void print_version(const char* arg)
{
	if ((strlen(arg) == 0) || (arg[0] == '.'))
	{
		std::cout << "fnc - Version " << FNC_VERSION_MAJOR << "." << FNC_VERSION_MINOR << std::endl;
	}
	else
	{
		std::cout << arg << " - Version " << FNC_VERSION_MAJOR << "." << FNC_VERSION_MINOR << std::endl;
	}
	std::cout << "Copyright (c) 2019-2021 - mmashimo" << std::endl << std::endl;
}

/// @brief Command line help
void print_help()
{
	std::cout << "Yet-another-Command line calculator. Expressions are run as arugments." << std::endl;
#ifdef __linux__
	std::cout << "NOTE: Using 'bash', it is better to add \"\" to avoid argument pre-processing of '*', '(' or pipe/redirection." << std::endl;
#endif
	std::cout << "\n'Interactive mode' runs when there are no arguments. Some added functionality in interactive mode:" << std::endl;
	std::cout << "- Expressions/variables can be retrieved or saved." << std::endl;
	std::cout << "- More than on expressions using results of previous calculations." << std::endl;
	std::cout << "- Expressions can be re-run by clearing results or expressed variables." << std::endl;
	std::cout << "- Detailed list of functions, constants, and unit-conversions can be listed." << std::endl;

	std::cout << std::endl;
}

/// @brief Some command line help details (uses Exec::printHelp, also).
void print_help_detail(const std::string& help)
{
	std::cout << "Help for '" << help << "' -- don't know how to help, yet" << std::endl;
}

/// @brief Simple test for 'fnc' (Exec) - libraries
int main(int argc, char** argv)
{
	// Use generic expression-processor
	Exec cmd;

#ifdef FNC_PROJECT
	IniParser iniConfig(s_iniPath, s_iniFileName, true);

	iniConfig.configure(cmd);

	std::string iniFile = iniConfig.getFileName();
#else
	// Setting INI file, if will reload later
	std::string iniFile;

	// NOTE: always re-construct if path is changed.
	// 'true' will save the path in case it does not exist
	IniParser::constructFileName(iniFile, s_iniPath, s_iniFileName, true);

	// Save to compare after argument processing
	s_iniFile = iniFile;

	// Setup processing from INI file
	cmd.getSettings(iniFile);
#endif

	if (argc == 1)
	{
		// Launches fnc in interactive mode
		print_version(argv[0]);
		std::cout << "Entering interactive mode. '?' for help; 'q<Enter>' to quit" << std::endl;

		// Do Interactive mode
		return cmd.runInteractive();
	}

	int ar = 1;
	CalString command;

	// first argument is a single option set having "--" as option list
	if ((strlen(argv[1]) > 2) && (argv[1][0] == '-') && (argv[1][1] == '-'))
	{
		// Get character after "--"
		char* option = argv[1] + 2;
		command = option;

		if (strcmp(option, "help") == 0)
		{
			print_version(argv[0]);
			if (argc == 2)
			{
				// "--help" only
				print_help();
			}
			else
			{
				// Get help for each item in argument. This wille exhaust argument list
				ar = 2;
				while(ar < argc)
				{
					print_help_detail(std::string(argv[ar]));
					ar++;
				}
			}
			return 0;
		}
		else if (strncmp(option, "vers", 4) == 0)
		{
			print_version(argv[0]);
			// Continue with parsing expressions
		}
		else
		{
			std::cout << "Don't know '" << argv[1] << "' option - try 'fnc --help'" << std::endl;
			// Continue
		}
		ar++;
	}

	command.clear();

	static bool possibleAsterisk = false;

	while (ar < argc)
	{
		CalString equ(argv[ar]);

#ifdef __linux__
		// More specifically #bash
		if (!equ.isNumber() && !equ.isFunction() && !equ.isVariable())
		{
			// An argument of "*" gives list of all files in the current directory
			if (!possibleAsterisk)
			{
				// command += " *";
				possibleAsterisk = true;
				std::cout << "!!! Possible use of '*', <file>, or unset variable in argument. May need to use \"<expression>\" (see '--help')" << std::endl;
			}
			std::cout << "Argument: '" << equ.c_str() << "' - ignored" << std::endl;
		}
		else
#endif

		{
			if (!command.empty())
			{
				// Don't add the first space
				command += " ";
			}
			command += argv[ar];
			if (possibleAsterisk)
			{
				std::cout << "Input: '" << argv[ar] << "' <= evaluate" << std::endl;
			}
		}
		
		ar++;
	}

	// If setting file changed, reload before running
	if (iniFile != s_iniFile)
	{
#ifdef FNC_PROJECT
	iniConfig.configure(cmd);
#else
		cmd.getSettings(iniFile);
#endif
	}

	std::cout << "Execute: '" << command << "'" << std::endl;

	cmd.execute(command);

	return 0;
}