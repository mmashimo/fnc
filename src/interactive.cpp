/// @file
///
/// @brief Class Implementation file for Interactive Parser.
///
/// Interactive parser was extracted from one-line editor where some
/// of the editing features are used here to capture one line of command.
/// This is now incorporated into this project.
///
/// NOTE: This class uses a derived class of std::string that has extra
/// functionality such as 'left' and 'shiftLeft', etc. (derived from a
/// similar constructs of MFC CString class).
///
/// Simplified operation: When Interactive is instantiated, show a prompt and
/// expects the user to "type" a command. Certain features are available:
/// - Command is passed to calling function when [Enter] (or Ctrl-M) is keyed.
/// - [Backspace] - erases last character typed or to the left of the cursor
/// - [Left],[Right] - cursor movement; add or delete at cursor
/// - [Home], [End] - cursor moves to the start or end of a line
/// - [Up],[Down] - command history recall. Replaces current edit
/// - [DEL] - Clears the entire command line
/// - [INS] - Toggles insert/delete mode. NOTE: Reverts to Insert mode after <Enter>
///
/// Few bugs had been fixed and currently runs on gcc or mingw version
/// only. Others have been ripped out. VC (cl) version has yet to be fully
/// fixed.
///
/// @copyright 2002-2021 - M.Mashimo and all licensors. All rights reserved.
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
#include <cstdio>

#include "interactive.h"

#ifdef WIN32
 #include <conio.h>
 #include <ctype.h>
#else
 // Used for getting Linux friendly getch()
 #include <termios.h>
#endif

 #include <cstdio>

#ifdef WIN32
// WIN32 SCAN Codes for Escape sequence
#define ESCAPE_KEY  224
// F# Keys
#define FN_KEY 0

#define KEY_UP    72
#define KEY_DOWN  80
#define KEY_RIGHT 77
#define KEY_LEFT  75

#define KEY_HOME  71
#define KEY_END   79
#define KEY_INS   82
#define KEY_DEL   83
#define KEY_PGUP  73
#define KEY_PGDN  81

#define BKSPACE   8

#else

// POSIX terminal Escape key
#define ESCAPE_KEY 27
#define SECOND_ESCAPE_SEQ '['

// Key up  91
#define KEY_UP    'A'
// Key down 80
#define KEY_DOWN  'B'
// Key Right 77
#define KEY_RIGHT 'C'
// Key Left 75
#define KEY_LEFT  'D'

#define KEY_INS   '2'
#define KEY_DEL   '3'
#define KEY_HOME  'H'
#define KEY_END   'F'
#define KEY_PGUP  '5'
#define KEY_PGDN  '6'

#define BKSPACE   127
#define TRAILING_TILDE 126
#endif


#ifndef WIN32
static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char _getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char _getche(void) 
{
  return getch_(1);
}
#endif


/*static*/
std::string InteractiveParser::m_prompt{"fnc>"};

// constructor
InteractiveParser::InteractiveParser()
	: m_cursorIndex{0}
	, m_historyIndex{0}
	, m_insertMode{true}
{
	// left intentionally blank	
}

bool InteractiveParser::getCommandLine(CalString& cmd)
{
	bool ok{true};

	bool done{false};
	int c;

	std::cout << m_prompt << cmd;
	m_command = cmd;
	m_previousCommand = cmd;
	m_insertMode = true;

	m_cursorIndex = cmd.size();

	do {
		c = _getch();

		// Special keys
		if ((c == '\n') || (c == '\r'))
		{
			// Complete the command line
			std::cout << std::endl;
			cmd = m_command;
			addCommandToHistory(m_command);

			done = true;
		}
		else if (c == ESCAPE_KEY)
		{
#ifdef WIN32
			processEscape();
#else
			// POSIX terminal uses '<ESC>[' for escape sequence. Handle second char
			c = _getch();
			if (c == SECOND_ESCAPE_SEQ)
			{
				processEscape();
			}
#endif
		}
		else if (c == BKSPACE)
		{
			processBackSpace();
		}
		else
		{
			update(c);
		}

	} while (!done);

	return ok;
}

void InteractiveParser::addCommandToHistory(const CalString& cmd)
{
	// If we have are at the end of the history buffer, then we will advance index
	if (m_entryHistory.size() == m_historyIndex)
	{
		m_historyIndex++;
	}

	// TODO: Check if we already have the same item in the list
	// and adjust the index to it
	m_entryHistory.push_back(m_command);
}

void InteractiveParser::update(const char c)
{
	if (m_cursorIndex == m_command.size())
	{
		// Tack the char to the end - insert or delete modes
		printf("%c", c);
		m_command += c;
		m_cursorIndex++;
	}
	else
	{
		CalString cmd(m_command);
		if (m_insertMode)
		{
			// Cursor is somewhere in the middle, push right
			CalString tmp(m_command);
			cmd.left(m_cursorIndex);
			cmd += c;
			tmp.shiftLeft(m_cursorIndex);
			cmd += tmp;
		}
		else
		{
			// Delete mode - overwrite and move cursor
			cmd[m_cursorIndex] = c;
		}
		updateCommandLine(cmd, m_cursorIndex+1);
	}
}

void InteractiveParser::processEscape()
{
	int c = _getch();

	switch(c)
	{
		case KEY_UP:		// Up Arrow
			previousCommand();
			break;
		case KEY_DOWN:		// Down Arrow
			nextCommand();
			break;
		case KEY_RIGHT:		// Right Arrow
			cursorRight();
			break;
		case KEY_LEFT:		// Left Arrow
			cursorLeft();
			break;
		case KEY_INS:		// INS
#ifdef WIN32
			m_insertMode = !m_insertMode;
#else
			// Clear the tilde at the end
			c = _getch();

			if (c == TRAILING_TILDE)
			{
				// Process INS
				m_insertMode = !m_insertMode;
				// TODO: Change cursor (if possible on terminal)
			}
#endif
			break;
		case KEY_DEL:		// DEL
#ifdef WIN32
			clearCommandLine();
#else
			// Clear the tilde at the end
			c = _getch();

			if (c == TRAILING_TILDE)
			{
				clearCommandLine();
			}
#endif
			break;
		case KEY_HOME:		// HOME
			cursorToStart();
			break;
		case KEY_END:		// END
			cursorToEnd();
			break;
		case KEY_PGUP:		// PageUp
#ifdef WIN32
				// Process PageUp
#else
			c = _getch();

			if (c == TRAILING_TILDE)
			{
				// Process PageUp
			}
#endif
			break;
		case KEY_PGDN:		// PageDn
#ifdef WIN32
				// Process PageDn
#else
			c = _getch();

			if (c == TRAILING_TILDE)
			{
				// Process PageDn
			}
#endif
			break;
		default:
			{
				std::string t0;
				t0.push_back(c);
#ifdef WIN32
				std::cout << "\r" << m_prompt << " Cursor key: <224>" << t0 << " (" << c << ") - errored" << std::endl;
#else
				std::cout << "\r" << m_prompt << " Cursor key: <ESC>[" << t0 << " (" << c << ") - errored" << std::endl;
#endif
				std::cout << m_prompt << m_command;
				m_cursorIndex = m_command.size();
			}
			break;
	}
}

void InteractiveParser::processBackSpace()
{
	// backspace
	// TODO: need ctrl-Z to undo
	int eqLen = m_command.size();
	if (eqLen > 0)
	{
		CalString cmd{m_command};

		if (m_cursorIndex == eqLen)
		{
			// Cursor at end
			cmd.left(eqLen-1);
			m_cursorIndex = cmd.size();
			updateCommandLine(cmd, m_cursorIndex);
		}
		else if (m_cursorIndex > 0)
		{
			CalString tmp{cmd};
			// Cursor is not at the end
			cmd.left(m_cursorIndex-1);
			cmd.append(tmp.shiftLeft(m_cursorIndex));
			updateCommandLine(cmd, m_cursorIndex-1);
		}
	}
}

void InteractiveParser::previousCommand()
{
	if (!(m_entryHistory.empty()) && (m_historyIndex > 0) && ((size_t)m_historyIndex <= m_entryHistory.size()))
	{
		if (m_historyIndex == m_entryHistory.size())
		{
			// Save current
			m_previousCommand = m_command;
		}
		--m_historyIndex;
		CalString cmd(m_entryHistory[m_historyIndex]);
		updateCommandLine(cmd, cmd.size());
	}
}

void InteractiveParser::nextCommand()
{
	if (!m_entryHistory.empty())
	{
		bool process{true};
		CalString cmd;
		m_historyIndex++;
		if ((size_t)m_historyIndex >= m_entryHistory.size())
		{
			if (m_previousCommand.empty())
			{
				m_previousCommand = m_command;
				process = false;
			}
			cmd = m_previousCommand;
			m_historyIndex = m_entryHistory.size();
		}
		else
		{
			// Saves whatever is in the editor before changing history
			// NOTE: if nothing in command, the first history item is preserved
			cmd = m_entryHistory[m_historyIndex];
		}
		if (process)
		{
			updateCommandLine(cmd, cmd.size());
		}
	}
}

void InteractiveParser::clearCommandLine()
{
	clearCurrentLine();
	m_command.clear();
	m_cursorIndex = 0;
}

void InteractiveParser::cursorRight()
{
	if ((size_t)m_cursorIndex < m_command.size())
	{
		m_cursorIndex++;
		updateCommandLine(m_command, m_cursorIndex);
	}
}

void InteractiveParser::cursorLeft()
{
	if (m_cursorIndex > 0)
	{
		m_cursorIndex--;
		updateCommandLine(m_command, m_cursorIndex);
	}
}

void InteractiveParser::cursorToStart()
{
	m_cursorIndex = 0;
	std::cout << "\r" << m_prompt;
}

void InteractiveParser::cursorToEnd()
{
	m_cursorIndex = m_command.size();
	std::cout << "\r" << m_prompt << m_command;
}

void InteractiveParser::clearCurrentLine()
{
	CalString tmp;
	tmp.fill(" ", m_command.size() + m_prompt.size() + 1);
	std::cout << "\r" << tmp << "\r" << m_prompt;
}

void InteractiveParser::updateCommandLine(const CalString& cmd, const int cursorIndex)
{
	if (m_command.size() > cmd.size())
	{
		clearCurrentLine();
	}

	std::cout << "\r" << m_prompt << cmd;
	if ((size_t)cursorIndex < cmd.size())
	{
		// Cursor is somewhere in the middle
		CalString tmp{cmd};
		tmp.left(cursorIndex);
		std::cout << "\r" << m_prompt << tmp;
	}

	if (!(m_command == cmd))
	{
		m_command = cmd;
	}

	m_cursorIndex = cursorIndex;
}

