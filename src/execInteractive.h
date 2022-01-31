/// @file
///
/// @brief Header file for Interactive Exec - Interactive function execution class
///
/// This class is derived from Exec where its internal function can be exposed for debugging.
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


#pragma once

#include "exec.h"

class ExecInteractive : public Exec
{
public:
	/// @brief Constructor
	ExecInteractive();

	/// @brief Destructor
	~ExecInteractive() = default;

	/// @brief

	/// @brief Process interactive commands.
	/// @param[in,out] entry - command string - may alter to pass into exec
	/// @return true if interactive command is processed.
	bool processInteractive(CalString& entry);

	int runInteractive();

	static void printInteractiveHelp(const CalString& args);

};