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

#include "iniParser.h"

// static
bool Exec::s_showUndefinedVarMsg = true;
bool Exec::s_quitMsgFirstTime = true;

Exec::Exec()
{
	// TODO: constants are added to variables list
}

bool Exec::execute(const CalString& equ)
{
	CalString tmp = equ;
	bool ok = parse(tmp, m_message);

	if (!ok)
	{
		std::cout << "! Parsing errored: " << m_message << std::endl;
	}

	run();

	return ok;
}


bool Exec::parse(CalString& equ, std::string& message)
{
	Func fnc;
	bool bDone = false;

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
	else if (m_stack.size() == 1)
	{
		std::cout << m_stack.back().asString() << std::endl;
	}
	else
	{
		int i = 1;
		for(auto it: m_stack)
		{
			std::cout << "[" << i << "] " << it.asString() << std::endl;
			i++;
		}
	}

	return m_stack.back();
}

bool Exec::run(NumStack& params)
{
	for(auto it: m_functions)
	{
		if (!it.run(params))
			return false;
	}
	return true;
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
	std::string message;
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

int Exec::runInteractive()
{
	CalString eq;
	int i = -1;
	int c;
	bool done = false;
	while (!done)
	{
		// Capture buffered line
		bool parsed = false;

		if (!eq.empty())
		{
			// TODO: Need to save equations and variables list before clearing expressions
			eq.clear();
			m_functions.clear();
		}

		std::cout << ">";
		do {
			// NOTE: each character is buffered until \n (is now like getchar())
			// TODO: Use getch() (using conio.h) to check for escape keys
			c = getchar();
			if (c == '\n')
			{
				parsed = true;
			}
			else
			{
				eq += c;
			}
		} while (!parsed);

		if (eq.empty())
		{
			if (s_quitMsgFirstTime)
			{
				std::cout << "Did you want to quit? - type 'q<CR>' to quit." << std::endl;
				s_quitMsgFirstTime = false;
			}
			Num::showVariables();
		}
		else if ((eq[0] == 'q') && (eq.size() == 1))
		{
			// Exiting - TODO: check if things need to be saved?
			done = true;
		}
		else if (eq[0] == '?')
		{
			// TODO: Need help stuff
			eq.left(1);
			printHelp(eq);
		}
		else
		{
			// TODO: Need a method NOT to clear function list
			execute(eq);
		}		
	}

	return 0;
}

void Exec::getSettings(std::string& iniPath)
{
	// By initializing ini file, INI file will be created
	IniParser ini(iniPath);
	if (ini.exists())
	{
		FunctionType::s_defaultAngle = ini.getString("Number.Angle", "deg");
		Exec::s_showUndefinedVarMsg = 0 != ini.getBool("Exec.UndefinedMsg", 1);
		Exec::s_quitMsgFirstTime = 0 != ini.getBool("Exec.quitMsg", 1);
	}
	else if (IniParser::s_forceUseIni)
	{
		ini.putString("Number.Angle", FunctionType::s_defaultAngle);
		ini.putBool("Exec.UndefinedMsg", Exec::s_showUndefinedVarMsg);
		ini.putBool("Exec.quitMsg", Exec::s_quitMsgFirstTime);
	}
}


// static
void Exec::printHelp(const CalString& args)
{
	if (args.empty())
	{
		// Lists help topics
		std::cout << "Help: Type expression as in command argument." << std::endl;
		std::cout << "- to exit, type 'q' and [Enter]." << std::endl;
		std::cout << "Other help topics not implemented, yet" << std::endl;
	}
}

