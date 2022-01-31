/// @file
///
/// @brief Header file for Func - handling functions and parameters.
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


#pragma once

#include <vector>

#include "num.h"

#include "calstring.h"
#include "calclist.h"
#include "functions.h"

#include <string>
#include <cmath>

// Function states
enum FunctionState : int8_t
{
	STATE_ERRORED = -1,
	STATE_INIT = 0,
	STATE_PARSED,
	STATE_NUMBER,   // last entry was a number
	STATE_FUNCTION, // Generic function - usually UNARY or BINARY where the second param is defined
	STATE_BINARY,	// expecting the second parameter
	STATE_UNARY,    // Unary function having paren associated
	STATE_CONVERT,  // Awaiting Number format
	STATE_PAREN,    // Function enclosed in a parenthesis
	STATE_CLOSE_PAREN,
	STATE_KEYED     // Function enclosed in a keyed parameter function
};

// How Func (function) works:
// A function is an equation extracted from string(s). It contains the entire
// functional block, number(s) and functions to perform. A formula is a function
// requiring input(s) - and starts with "=". A formula requiring 'x' and 'y' variables
// in the formula would start with: '=x; =y; (formula with x and y in the equation'
// meaning that interpreter will wait with: 'x=' and 'y='

class Func
{
public:
	/// @brief Default constructor
	Func();

	/// @brief Copy constructor
	Func(const Func& ref);

	/// @brief Assignment operator
	Func& operator =(const Func& ref);

	Func& operator +=(const std::string& addCode);

	~Func() { }

	void init();

	// Returns position of the string where function stopped. -1 if entire string is used
	bool parse(CalString& code, CalcList& message, bool& bDone);

	bool run(NumStack& initValue, CalcList& message);

	bool isNop() const { return m_function.isNop(); }

	void pushPrior(const Num& arg);

private:
	void copyHelper(const Func& ref);

	bool parseNumber(CalString& eq, CalcList& message, bool& bDone);

	bool parseFunction(CalString& eq, CalcList& message, bool& bDone);

	void addNumber(const Num& no, CalcList& message, bool& bDone);

	bool addFunction(Functions& fnType, CalString& eq, CalcList& message, bool& bDone);

	bool addSubFunctions(Functions& fnType, CalString& eq, CalcList& message, bool& bDone);

	// Data member
	FunctionState  m_state;

	FunctionType m_function;

	// Numbers added before the function
	NumStack m_prior;
	// Numbers added after the function
	NumStack m_params;

	std::vector<Func> m_subFunctions;
};
