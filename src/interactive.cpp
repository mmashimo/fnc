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

 // Used for getting Linux friendly getch()
 #include <termios.h>
 #include <cstdio>

#define KEY_UP    91
#define KEY_DOWN  80
#define KEY_LEFT  75
#define KEY_RIGHT 77

#define ESCAPE    27
#define BKSPACE   127


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
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

InteractiveCommand::InteractiveCommand()
	: m_state{0}
{

}


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
		c = getch();

		// Special keys
		if ((c == '\n') || (c == '\r'))
		{
			// Complete the command line
			std::cout << std::endl;
			cmd = m_command;
			addCommandToHistory(m_command);

			done = true;
		}
		else if (c == ESCAPE)
		{
			processEscape();
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
	int c = getch();

	// Look for cursor key
	if (c == '[')
	{
		c = getch();
		switch(c)
		{
			case 'A':		// Up Arrow
				previousCommand();
				break;
			case 'B':		// Down Arrow
				nextCommand();
				break;
			case 'C':		// Right Arrow
				cursorRight();
				break;
			case 'D':		// Left Arrow
				cursorLeft();
				break;
			case '2':		// INS
				// Clear the tilde at the end
				c = getch();
				if (c == 126)
				{
					// Process INS
					m_insertMode = !m_insertMode;
					// TODO: Change cursor (if possible on terminal)
				}
				break;
			case '3':		// DEL
				// Clear the tilde at the end
				c = getch();
				if (c == 126)
				{
					clearCommandLine();
				}
				break;
			case 'H':		// HOME
				cursorToStart();
				break;
			case 'F':		// END
				cursorToEnd();
				break;
			case '5':		// PageUp
				c = getch();
				if (c == 126)
				{
					// Process PageUp
				}
				break;
			case '6':		// PageDn
				c = getch();
				if (c == 126)
				{
					// Process PageDn
				}
				break;
			default:
				{
					std::string t0;
					t0.push_back(c);
					std::cout << "\r" << m_prompt << " Cursor key: <ESC>[" << t0 << " (" << c << ")" << std::endl;
					std::cout << m_prompt << m_command;
					m_cursorIndex = m_command.size();
				}
				break;
		}
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
	if (!(m_entryHistory.empty()) && (m_historyIndex > 0) && (m_historyIndex <= m_entryHistory.size()))
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
		if (m_historyIndex >= m_entryHistory.size())
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
	if (m_cursorIndex < m_command.size())
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
	if (cursorIndex < cmd.size())
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
