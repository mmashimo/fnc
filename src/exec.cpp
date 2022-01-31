/// @file
///
/// @brief Implementation of Exec class - expression execution.
///
/// @copyright 2009-2021 - M.Mashimo and all licensors. All rights reserved.
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

#include <sys/stat.h> // Used to check if file exists
#include <exception>

#include <cmath>

#include "exec.h"
#include "func.h"
#include "num.h"
#include "functions.h"

#include "calstring.h"

// static
bool Exec::s_showUndefinedVarMsg = true;
bool Exec::s_quitMsgFirstTime = true;

uint32_t Exec::s_verbose{ 0 };


Exec::Exec()
	: m_historyIndex{0}, m_cursorIndex{0}
{
	// TODO: constants are added to variables list
}

bool Exec::execute(const CalString &equ)
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

	if (s_verbose)
	{
		for (auto &it : m_message)
		{
			std::cout << "[" << it << "]" << std::endl;
		}
	}

	run();

	return ok;
}

bool Exec::parse(CalString &equ, CalcList &message)
{
	Func fnc;
	bool bDone = false;

	m_parsedList.clear();
	m_functions.clear();

	while (!bDone && !equ.empty())
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

	m_stack.list();

	return m_stack.back();
}

bool Exec::run(NumStack &params)
{
	int szFun = m_functions.size();
	bool running = true;
	auto itr = m_functions.begin();
	for (; (itr != m_functions.end()) && running; itr++)
	{
		if (!itr->run(params, m_message))
		{
			running = false;
		}
	}

	return running;
}

// This can run without specifying "Exec"
bool Exec::inputParseAndRun(Num &inp, CalString &eq)
{
	Exec cmd;

	bool ok = cmd.inputParseAndRun(inp, eq, cmd.m_stack);
	if (ok)
	{
		inp = cmd.m_stack.back();
	}

	return ok;
}

bool Exec::inputParseAndRun(Num &inp, CalString &eq, NumStack &stack)
{
	CalcList message;
	bool done = false;

	Func f;
	f.pushPrior(inp);
	if (!f.parse(eq, message, done))
		return false;

	m_functions.push_back(f);
	while (!eq.empty())
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


void Exec::addToHistory(const CalString &string)
{
	if (entryHistory.size() == m_historyIndex)
	{
		m_historyIndex++;
	}

	entryHistory.push_back(string);
}
