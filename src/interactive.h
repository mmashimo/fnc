/// @file
///
/// @brief Header file for Interactive Parser.
///
/// Interactive parser was extracted from one-line editor where some
/// of the editing features are used here to capture one line of command.
/// This is now incorporated into this project (see cpp header for details)
///
/// @copyright 2002-2022 - M.Mashimo and all licensors. All rights reserved.
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

//***************************************************************
// Features:
// - Uses ncurses like input one char at a time.
// - Commands are wrappers around strings (specific to the project) that
//   are returned after the user enters <CR>.
// - In-line editing, backspace, and simple history retrieval are
//   implemented. Any other project related functions not implemented
//
// TODO:
// - Save/Restore history
// - Sequential command entry
// - Undo/Redo
//****************************************************************
#pragma once

#include <cstdint>

#include <string>
#include <vector>

#include "calstring.h"


class InteractiveParser
{
public:
	InteractiveParser();

	/// @brief Get input command from interactive command parser.
	/// @param[in] cmd - command input state and outputs string to parse
	bool getCommandLine(CalString& cmd);

	/// @brief Adds command line to history, good or bad
	void addCommandToHistory(const CalString& cmd);

	/// @brief Command line prompt
	static std::string m_prompt;

private:
	void update(const char c);

	void processEscape();

	void processBackSpace();

	void previousCommand();

	void nextCommand();

	void clearCommandLine();

	void cursorRight();

	void cursorLeft();

	void cursorToStart();

	void cursorToEnd();

	/// @brief Clears current line by erasing the string.
	/// NOTE: Only clears line of the current line - use 'updateCommandLine' to get new string
	void clearCurrentLine();

	/// @brief updates current line with new string and cursor
	/// @param[in] cmd - new command line
	/// @param[in] cursorIndex - new cursor index
	void updateCommandLine(const CalString& cmd, const int cursorIndex);

	//--- Data member ---

	/// @brief Current command line string
	CalString m_command;

	/// @brief Command line before editing history. This does not get saved
	CalString m_previousCommand;

	/// @brief Current cursor index to command string
	int  m_cursorIndex;

	/// @brief Keeps record of key entry
	std::vector<CalString> m_entryHistory;

	/// @brief Current index to history
	int  m_historyIndex;

	/// @brief Insert mode flag.
	/// NOTE: If true, adds and shifts at cursor (default). If false, inserts and replaces at cursor
	bool m_insertMode;

};

