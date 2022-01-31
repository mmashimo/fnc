/// @file
///
/// @brief Implementation of ExecInteractive class - interactive expression execution.
///
/// @copyright 2022 - M.Mashimo and all licensors. All rights reserved.
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

#include <sys/stat.h>  // Used to check if file exists
#include <exception>

#include "execInteractive.h"

#include "interactive.h"


ExecInteractive::ExecInteractive()
{
	// Intentionally left blank
}

bool ExecInteractive::processInteractive(CalString& entry)
{
	bool processed = false;
	size_t len = entry.size();

	if (entry == "help")
	{
		printInteractiveHelp("");
		processed = true;
	}
	else if (entry.compare("list", 4))
	{
		std::cout << "Listing variables:" << std::endl;
		Num::showVariables(true);
		std::cout << "Listing Stack:" << std::endl;
		m_stack.list(true);
		processed = true;
	}
	else if (entry.compare("listVars", 5))
	{
		std::cout << "Listing variables:" << std::endl;
		Num::showVariables(true);
		processed = true;
	}
	else if (entry.compare("listStack", 5) || entry.compare("showStack", 5))
	{
		std::cout << "Listing Stack:" << std::endl;
		m_stack.list(true);
		processed = true;
	}
	else if (entry.compare("clearStack", 6))
	{
		// TODO: Clear stack
		std::cout << "Clearing stack" << std::endl;
		m_stack.clear();
		processed = true;
	}
	else if (entry.compare("functions", 4))
	{
		// List all functions
		std::cout << "Functions" << std::endl;
		FunctionType::listFunctions(0);
		processed = true;
	}
	else
	{
		processed = false;
	}

	if (processed)
	{
		addToHistory(entry);
	}

	return processed;
}


int ExecInteractive::runInteractive()
{
	CalString cmd;
	InteractiveParser parser;

	bool done{false};

	while (!done)
	{
		parser.getCommandLine(cmd);

		if (cmd.empty())
		{
			if (s_quitMsgFirstTime)
			{
				std::cout << "Did you want to quit? - type 'q<CR>' to quit." << std::endl;
				s_quitMsgFirstTime = false;
			}
		}
		else if ((cmd[0] == 'q') && (cmd.size() == 1))
		{
			// Exiting - TODO: check if things need to be saved?
			done = true;
		}
		else if (cmd[0] == '?')
		{
			// TODO: Need help stuff
			cmd.shiftLeft(1);
			printInteractiveHelp(cmd);
		}
		else
		{
			if (processInteractive(cmd))
			{
				// Process next command
				cmd.clear();
			}
			else if (execute(cmd))
			{
				// TODO: Need a method NOT to clear function list
				cmd.clear();
				// Possibly add to "good history?"
			}
			else
			{
				// Error condition, so keep in command line
			}
		}
	}

	return 0;
}


// static
void ExecInteractive::printInteractiveHelp(const CalString& args)
{
	if (args.empty())
	{
		// Lists help topics
		std::cout << "Help: Type expression as in command argument." << std::endl;
		std::cout << "- to exit, type 'q' and [Enter]." << std::endl;
		
		std::cout << "listVars          - lists stored variables and stack" << std::endl;
		std::cout << "showStack         - lists stored variables and stack" << std::endl;
		std::cout << "clearStack        - clears stack" << std::endl;
		std::cout << "functions         - lists all functions" << std::endl;
	}
}