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

	bool parse(CalString& equ, std::string& message);

	Num run();

	bool run(NumStack& params);

	bool inputParseAndRun(Num& inp, CalString& eq);

	bool inputParseAndRun(Num& inp, CalString& eq, NumStack& stack);

	int runInteractive();

	const std::string& errMessage() const { return m_message; }

	void getSettings(std::string& iniPath);

	static void printHelp(const CalString& args);

	static bool s_showUndefinedVarMsg;
	static bool s_quitMsgFirstTime;

private:

	std::string m_message;

	// Keeps a list of functions
	std::vector<Func> m_functions;

	NumStack m_stack;
};