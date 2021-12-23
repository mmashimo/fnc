/// @file
///
/// @brief Header file for Exec - function execution class (library)
///
/// This class is the entry point for both string and interactive function
/// execution library. It has both interactive (CLI) and direct argument
/// string processing.
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

#include <vector>

#include "func.h"

#include "num.h"

#include "calstring.h"

/// @brief Base Bridged object used to run functional/numeric constructs

class Exec
{
public:
	Exec();

	bool execute(const CalString& equ);

	bool parse(CalString& equ, CalcList& message);

	Num run();

	bool run(NumStack& params);

	bool inputParseAndRun(Num& inp, CalString& eq);

	bool inputParseAndRun(Num& inp, CalString& eq, NumStack& stack);

	int runInteractive();

	const std::string errMessage() const { return m_message.asString(); }

	/// @brief Process interactive commands.
	/// @param[in,out] entry - command string - may alter to pass into exec
	/// @return true if interactive command is processed.
	bool processInteractive(CalString& entry);

	static void printInteractiveHelp(const CalString& args);

	static bool s_showUndefinedVarMsg;
	static bool s_quitMsgFirstTime;

private:

	// Keep records of all messages
	CalcList m_message;

	// Parsed list of numbers/functions
	CalcList m_parsedList;

	// Keeps a list of functions
	std::vector<Func> m_functions;

	// Numeric stack
	NumStack m_stack;

	// Keeps record of key entry
	std::vector<CalString> entryHistory;

	// Current index to history
	int  m_historyIndex;

	// Current index to cursor
	int  m_cursorIndex;

	/// @brief Adds string to history buffer
	void addToHistory(const CalString& string);
};