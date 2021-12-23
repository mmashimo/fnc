/// @file
///
/// @brief Implementation of Exec class - expression execution.
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

#include <sys/stat.h>  // Used to check if file exists
#include <exception>

#include <cmath>

#include "exec.h"
#include "func.h"
#include "num.h"
#include "functions.h"

#include "interactive.h"

// static
bool Exec::s_showUndefinedVarMsg = true;
bool Exec::s_quitMsgFirstTime = true;

Exec::Exec()
	: m_historyIndex{0}
	, m_cursorIndex{0}
{
	// TODO: constants are added to variables list
}

bool Exec::execute(const CalString& equ)
{
	CalString tmp = equ;

	// Whether it is correctly parsed or not, keep in history
	addToHistory(equ);

	// NOTE: Internal use of m_parsedList is clear when "parse"d
	bool ok = parse(tmp, m_message);

	if (!ok)
	{
		std::cout << "! Parsing errored: " << m_message.asString() << std::endl;
	}

	run();

	return ok;
}


bool Exec::parse(CalString& equ, CalcList& message)
{
	Func fnc;
	bool bDone = false;

	m_parsedList.clear();
	m_functions.clear();

	while(!bDone && !equ.empty())
	{
		fnc.init();
		if (fnc.parse(equ, message, bDone))
		{
			m_functions.push_back(fnc);
			bDone = equ.empty();
		}
		else
		{
			// Function errored
			std::cout << "Function parsing failed: >>" << equ.c_str() << std::endl;
			return false;
		}
	}

	return bDone;
}


Num Exec::run()
{
	bool ok = run(m_stack);

	if (!ok)
	{
		std::cout << "!!! Exec::run() - ran with errors!" << std::endl;
	}

    Num::showVariables();

	if (m_stack.empty())
	{
		// std::cout << "!!! Exec::run() - No results!" << std::endl;
		return Num();
	}
#if 0
	else if (m_stack.size() == 1)
	{
		std::cout << m_stack.back().asString() << std::endl;
	}
	else
	{
		int i = 1;
		for(auto &it: m_stack)
		{
			std::cout << "[" << i << "] " << it.asString() << std::endl;
			i++;
		}
	}
#endif
	m_stack.list();

	return m_stack.back();
}

bool Exec::run(NumStack& params)
{
	int szFun = m_functions.size();
	bool running = true;
	auto itr = m_functions.begin();
	for (; (itr != m_functions.end()) && running; itr++)
	{
		if (!itr->run(params))
		{
			running = false;
		}
	}
#if 0
	for(auto &it: m_functions)
	{
		if (!it.run(params))
			return false;
	}
#endif
	return running;
}

bool Exec::inputParseAndRun(Num& inp, CalString& eq)
{
	bool ok = inputParseAndRun(inp, eq, m_stack);
	if (ok)
	{
		inp = m_stack.back();
	}

	return ok;
}

bool Exec::inputParseAndRun(Num& inp, CalString& eq, NumStack& stack)
{
	CalcList message;
	bool done = false;

	Func f;
	f.pushPrior(inp);
	if (!f.parse(eq, message, done))
		return false;

	m_functions.push_back(f);
	while(!eq.empty())
	{
		f.init();
		if (!f.parse(eq, message, done))
			return false;

		m_functions.push_back(f);
	}

	run(stack);

	if (stack.empty())
	{
		std::cout << "!!Exec::inputParseAndRun() - did not produce any results!" << std::endl;
		return false;
	}

	return true;
}

bool Exec::processInteractive(CalString& entry)
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


int Exec::runInteractive()
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
void Exec::printInteractiveHelp(const CalString& args)
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

void Exec::addToHistory(const CalString& string)
{
	if (entryHistory.size() == m_historyIndex)
	{
		m_historyIndex++;
	}

	entryHistory.push_back(string);
}


