/// @file
///
/// @brief Header for Function operations.
///
/// FunctionType is a generic function "type" class. Actual functions are
/// handled in direct calls to "C" operations, not bound by class definitions.
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

#include <stddef.h>
#include <string>
#include "num.h"

enum FunctionMode : uint32_t
{
	MODE_NORMAL  = 0, // Generic function
	MODE_BINARY  = 0x10,
	MODE_UNARY   = 0x100,
	MODE_CONVERT = 0x200, // Unary function where the following "Num" is only the format
	MODE_PARAM   = 0x400,
	MODE_PARAM_END,
    MODE_ASSIGN  = 0x800, // Assignment - either assign or save
};

enum FunctionValue : uint32_t
{
	F_NOP     = MODE_NORMAL,   // Includes Error function
	F_SET,

// Binary
	F_ADD     = MODE_BINARY,
	F_SUB,
	F_MUL,
	F_DIV,
	F_POW,
	F_ROOT,   // \x/ y or y^-x (same as power, except '- not needed)
	F_MOD,
	F_MAX,
	F_MIN,

// Unary
	F_RESULT   = MODE_UNARY,
	F_SQRT,
	F_ABS,
	F_NEG,    // Negate number
	F_INV,    // 1/x
	F_EXP,    // e^x
	F_E10X,   // 10^x
	F_EXP2,   // 2^x
	F_LN,     // Natural log
	F_LOG,    // log-base 10
	F_LOG2,   // log-base 2
	F_SIN,
	F_COS,
	F_TAN,
	F_ASIN,
	F_ACOS,
	F_ATAN,
	F_SINH,
	F_COSH,
	F_TANH,
	F_ASINH,
	F_ACOSH,
	F_ATANH,
	F_CEIL,
	F_FLOOR,
	F_FRAC,		// Fractional part

// Binary conversion
	F_CONV,

// Parameter
	F_OPEN_PAREN = MODE_PARAM,
	F_OPEN_KEY,
	F_SEP,       // comma or semi separator
	F_VECTOR,
	F_MATRIX,
	F_BOUND_ABS,
	F_SAVE_MEM_NAME,

	F_CLOSE_PAREN = MODE_PARAM_END,
	F_CLOSE_KEY,
	F_CLOSE_SAVE,
};

using Functions = struct _funcs
{
	std::string   str;
	FunctionValue type;
	FunctionMode  mode;
	bool          (*f)(NumStack&);
};


class FunctionType
{
public:
	Functions   m_function;

	// Default constructor
	FunctionType();

	FunctionType(const FunctionValue& type);

	FunctionType(const FunctionType& ref) : m_function(ref.m_function) {}

	FunctionType(const Functions& ref) : m_function(ref) {}

	~FunctionType(){}

	FunctionType& operator =(const FunctionType& ref)
	{
		m_function  = ref.m_function;
		return *this;
	}

	FunctionType& operator =(const Functions& ref)
	{
		m_function  = ref;
		return *this;
	}

	bool isNop() const { return m_function.type == F_NOP; }

	/// @brief Runs function
	bool run(NumStack& params);

	/// @brief Converts units of Binary function as required by Mode
	static void convertUnits(Num& result, Num& inp0, Num& inp1);

	/// @brief Finds function in the list of functions
	/// @param[in] str - find function in string
	/// @param[out] pos - string on left that best describes function
	/// @param[out] func - Function found (populated if it returns true)
	/// @return true if Function is found
	static int findFunc(const CalString& str, int& pos, Functions& func);

	static bool getFunc(const FunctionValue type, Functions& func);

	/// @brief Used to verify that non-angle computation reverts to radians
	static bool isDefaultRad();

	/// @brief Default angle used for transcendental
	static void setDefaultAngleRad(bool rad = true) { s_defaultAngle = rad?"rad":"deg";}

	/// @brief Transcendental computation default angle.
	/// Used to check default numeric computation (as comiled, should be degrees)
	static std::string s_defaultAngle;

};

