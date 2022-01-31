/// @file
///
/// @brief class implementation for Func - function and parameter handling.
///
/// @copyright 2009-2022 - M.Mashimo and all licensors. All rights reserved.
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



#include "func.h"
#include <iostream>
#include <string>
#include <string.h>
#include <map>


Func::Func()
{
	m_state = STATE_INIT;
	init();
}

Func::Func(const Func& ref)
{
	copyHelper(ref);
}

Func& Func::operator =(const Func& ref)
{
	copyHelper(ref);
	return *this;
}

void Func::copyHelper(const Func& ref)
{
	m_state  = ref.m_state;
	m_function = ref.m_function;

	m_prior = ref.m_prior;
	m_params = ref.m_params;
	m_subFunctions = ref.m_subFunctions;
}

void Func::init()
{
	// Need to keep the last function state for parsing
	//	m_state = STATE_INIT;
	m_function = FunctionType();
	m_prior.clear();
	m_params.clear();
	m_subFunctions.clear();
}


bool Func::parse(CalString& eq, CalcList& message, bool& bDone)
{
	bDone = false;

	while (!bDone && !eq.empty())
	{
		eq.trimLeft();
		if ((m_state == STATE_CONVERT)
			|| (eq.isNumber() &&
			((m_state == STATE_INIT) || (m_state == STATE_BINARY))))
		{
			// For conversion, force number to be of certain unit
			if (!parseNumber(eq, message, bDone))
				return false;
			if (eq.empty())
				return true;
		}
		else if (!eq.empty())
		{
			if (!parseFunction(eq, message, bDone))
				return false;
		}
	}

	return true;
}

bool Func::parseNumber(CalString& eq, CalcList& message, bool& bDone)
{
	Num no;
	if (!no.parse(eq, message))
	{
		// Error occurred
		std::cout << "Func::parse(" << eq.c_str() << ") - Errored: " << message.asString() << std::endl;
		return false;
	}

	addNumber(no, message, bDone);

	return true;
}

bool Func::parseFunction(CalString& eq, CalcList& message, bool& bDone)
{
	Functions fnType;
	int pos = -1;
	if(FunctionType::findFunc(eq, pos, fnType))
	{
		// Make sure our current function is NOT set
	}
	else
	{
		// Function not found - possibly constant or variable
		if (eq.isNumber())
		{
			bool done = false; // dummy bool

			return parseNumber(eq,message,done);
		}

		// Function not found, check variables before quitting with message
		Num no;
		if (no.parseVar(eq, message))
		{
			addNumber(no, message, bDone);
			return true;
		}
        if (no.numString().empty() || no.varName().empty())
        {
			CalString msg;
            msg = "Func::parseFunction: >>";
            msg += eq.c_str();
            msg += " - Could not find function";
			msg.setErrorType();
			message.add(msg);
            return false;
        }
        // Variable was added with an assigned value - so keep going
        return true;
	}
    // So there is a function assigned, but is this the first or the function at unary
	if (m_function.isNop() || (m_state == STATE_UNARY))
	{
		CalString tmp(eq);
		tmp.left(pos);
		tmp.setFunctionType();
		message.add(tmp);
		eq.shiftLeft(pos);
		eq.trimLeft();
		if (!addFunction(fnType, eq, message, bDone))
			return false;
	}
	else
	{
		// Function already defined - push into subfunction
		Func fnx;
		while (!bDone && !eq.empty())
		{
			if (!fnx.parse(eq, message, bDone))
				return false;
			m_subFunctions.push_back(fnx);
		}
		m_state = STATE_FUNCTION;
		bDone = true;
	}
	return true;
}

void Func::addNumber(const Num& no, CalcList& message, bool& bDone)
{
	Functions func;
	if ((m_state == STATE_BINARY) || (m_state == STATE_CONVERT))
	{
		// If suspending on binary value, then function is now done
		m_state = STATE_PARSED;
		m_params.push_back(no);
		bDone = true;
	}
	else if ((m_state == STATE_NUMBER)
		&& isNop()
		&& !m_prior.empty()
		&& m_params.empty()
		&& FunctionType::getFunc(F_MUL, func))
	{
		// Mulitply the two numbers
		m_function = func;
		m_state = STATE_PARSED;
		m_params.push_back(no);
		bDone = true;
	}
	else
	{
		// Initial or functional is added after function
		m_state = STATE_NUMBER;
		m_prior.push_back(no);
	}
}

bool Func::addFunction(Functions& fnType, CalString& eq, CalcList& message, bool& bDone)
{
	if ((fnType.mode == MODE_BINARY) || (fnType.mode == MODE_ASSIGN))
	{
		m_function = fnType;
		// Binary function identified
		m_state = STATE_BINARY;
	}
	else if (fnType.mode == MODE_UNARY)
	{
		// make sure function is saved here
		m_function = fnType;
		int pos = -1;
		Functions ftype;
		// If parent follows, do function in it before doing the unary function
		bool found = !eq.empty() && FunctionType::findFunc(eq, pos, ftype);
		// If the function is followed by (paren), run that function
		if(found && (ftype.mode == MODE_PARAM))
		{
			// Set state - next function is '()' to push on function stack
			m_state = STATE_UNARY;
			// Make sure next function is processed
			bDone = false;
		}
		else
		{
			// Otherwise, we use the value in the stack
			m_state = STATE_FUNCTION;
			bDone = true;
		}
	}
	else if (fnType.mode == MODE_CONVERT)
	{
		m_function = fnType;
		m_state = STATE_CONVERT;
	}
	else if (fnType.mode == MODE_PARAM)
	{
		m_state = STATE_PAREN;
		Func fn;
		bDone = false;
		while (!bDone && !eq.empty())
		{
			fn.init();
			int i = fn.parse(eq, message, bDone);
			if (i < 0)
			{
				return false;
			}
			if (fn.m_state == STATE_CLOSE_PAREN)
			{
				bDone = true;
			}
			m_subFunctions.push_back(fn);
			if (!bDone && eq.empty())
			{
				// TODO: No ending paren
				fn.m_state = STATE_ERRORED;
				std::cout << "No closing parentheses!" << std::endl;
			}
		}
		bDone = true;
	}
	else if (fnType.mode == MODE_PARAM_END)
	{
		m_state = STATE_CLOSE_PAREN;
		bDone = true;
	}
	else
	{
		m_function = fnType;
		m_state = STATE_FUNCTION;
		bDone = true;
	}
	return true;
}

void Func::pushPrior(const Num& arg)
{
	m_state = STATE_NUMBER;
	m_prior.push_back(arg);
}


bool Func::run(NumStack& initValue, CalcList& message)
{
	bool ok = false;

	for (auto &itr0 : m_prior)
	{
		// TODO:: Check if the number needs variable input
		initValue.push_back(itr0);
	}

	// Run subtasks first
	for(auto &itr1 : m_subFunctions)
	{
		ok = itr1.run(initValue, message);
	}

	// Push results in backward order
	for (auto &itr2 : m_params)
	{
		// TODO:: Check if the number needs variable input
		initValue.push_back(itr2);
	}

	ok = m_function.run(initValue, message);

	return true;
}


