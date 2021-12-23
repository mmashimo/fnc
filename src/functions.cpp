/// @file
///
/// @brief Implements function operations.
///
/// For simplicity of coding, C-style function calls are used to extract data
/// from stack-like number-lists. Both unary and binary functions are performed
/// as if running in a stack-oriented (RPN-like) operation.
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


#include <iostream>
#include <string.h>

#include "func.h"
#include "num.h"
#include "exec.h"

// static
std::string FunctionType::s_defaultAngle{"deg"};
std::string FunctionType::s_defaultExpression{"Dec"};
std::string FunctionType::s_defaultFPP{"bin-64"};

bool nop(NumStack& params)
{
	return true;
}

// Binary Functions
bool add(NumStack& params)
{
	Num result;
	// Second param
	Num inp1 = params.back();
	params.pop_back();
	// First param
	Num inp0 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = inp0.m_dValue + inp1.m_dValue;
	}
	else
	{
		// Assume integer math
		result.m_lValue = inp0.m_lValue + inp1.m_lValue;
	}

	if (inp0.m_unit == inp1.m_unit)
	{
		result.m_unit = inp0.m_unit;
	}

	params.push_back(result);
	return true;
}

bool sub(NumStack& params)
{
	Num result;
	Num inp1 = params.back();
	params.pop_back();
	Num inp0 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = inp0.m_dValue - inp1.m_dValue;
	}
	else
	{
		// Assume integer math
		result.m_lValue = inp0.m_lValue - inp1.m_lValue;
	}

	if (inp0.m_unit == inp1.m_unit)
	{
		result.m_unit = inp0.m_unit;
	}

	params.push_back(result);
	return true;
}

bool mul(NumStack& params)
{
	Num result;
	Num inp1 = params.back();
	params.pop_back();
	Num inp0 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = inp0.m_dValue * inp1.m_dValue;
	}
	else
	{
		// Assume integer math
		result.m_lValue = inp0.m_lValue * inp1.m_lValue;
	}

	if (inp0.m_unit.isDefault())
	{
		result.m_unit = inp1.m_unit;
	}
	else if (inp1.m_unit.isDefault())
	{
		result.m_unit = inp0.m_unit;
	}

	params.push_back(result);
	return true;
}

bool div(NumStack& params)
{
	bool ok{true};
	Num result;
	Num inp1 = params.back();
	params.pop_back();
	Num inp0 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// NOTE: MISRA recommends not to use equate on doubles, but what the heck
		if (inp1.m_dValue != 0.)
		{
			// return results in double:
			result.m_dValue = inp0.m_dValue / inp1.m_dValue;
		}
		else
		{
			ok = false;
		}
	}
	else
	{
		// Assume integer math
		int64_t rem = inp0.m_lValue % inp1.m_lValue;
		if (rem == 0)
		{
			if (inp1.m_lValue != 0)
			{
				result.m_lValue = inp0.m_lValue / inp1.m_lValue;
			}
			else
			{
				ok = false;
			}
		}
		else
		{
			// Convert to doubles;
			result.convertTo(NUM_DOUBLE);
			if (inp1.m_lValue != 0.)
			{
				result.m_dValue = static_cast<double>(inp0.m_lValue) / static_cast<double>(inp1.m_lValue);
			}
			else
			{
				ok = false;
			}
		}
	}

	if (ok)
	{
		if (inp1.m_unit.isDefault())
		{
			result.m_unit = inp0.m_unit;
		}

		params.push_back(result);
	}
	else
	{
		std::cout << "Attempting to divide by zero! Operation suspended." << std::endl;
		params.push_back(inp0);
		params.push_back(inp1);
	}
	return true;
}

bool pow(NumStack& params)
{
	Num result;
	Num inp1 = params.back();
	params.pop_back();
	Num inp0 = params.back();
	params.pop_back();

#ifdef _MSC_VER
    inp0.convertTo(NUM_DOUBLE);
    inp1.convertTo(NUM_DOUBLE);
    result.convertTo(NUM_DOUBLE);
    result.m_dValue = pow(inp0.m_dValue, inp1.m_dValue);
#else
	FunctionType::convertUnits(result, inp0, inp1);
	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = pow(inp0.m_dValue, inp1.m_dValue);
	}
	else
	{
		// Assume integer math
		result.m_lValue = pow(inp0.m_lValue, inp1.m_lValue);
	}
#endif


    params.push_back(result);
	return true;
}

bool max(NumStack& params)
{
	Num result;
	Num inp0 = params.back();
	params.pop_back();
	Num inp1 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = fmax(inp0.m_dValue, inp1.m_dValue);
	}
	else
	{
		// Assume integer math
		result.m_lValue = inp0.m_lValue > inp1.m_lValue ? inp0.m_lValue : inp1.m_lValue;
	}
	params.push_back(result);
	return true;
}

bool min(NumStack& params)
{
	Num result;
	Num inp0 = params.back();
	params.pop_back();
	Num inp1 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = fmin(inp0.m_dValue, inp1.m_dValue);
	}
	else
	{
		// Assume integer math
		result.m_lValue = inp0.m_lValue < inp1.m_lValue ? inp0.m_lValue : inp1.m_lValue;
	}
	params.push_back(result);
	return true;
}


// UNARY functions
bool root(NumStack& params)
{
	Num result;
	Num inp1 = params.back();
	params.pop_back();
	Num inp0 = params.back();
	params.pop_back();

	inp0.convertTo(NUM_DOUBLE);
	inp1.convertTo(NUM_DOUBLE);

	// return results in double:
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = pow(inp0.m_dValue, (1./inp1.m_dValue));

	params.push_back(result);
	return true;
}

bool mod(NumStack& params)
{
	Num result;
	Num inp1 = params.back();
	params.pop_back();
	Num inp0 = params.back();
	params.pop_back();

	FunctionType::convertUnits(result, inp0, inp1);

	if (result.isDouble())
	{
		// return results in double:
		result.m_dValue = fmod(inp0.m_dValue, inp1.m_dValue);
	}
	else
	{
		// Assume integer math
		result.m_lValue = inp0.m_lValue % inp1.m_lValue;
	}
	params.push_back(result);
	return true;
}

bool sqrt(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = sqrt(result.m_dValue);
	params.push_back(result);
	return true;
}

bool abs(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	if (result.isInteger())
	{
		result.m_lValue = labs(static_cast<long>(result.m_lValue));
	}
	else if (result.isDouble())
	{
		result.m_dValue = fabs(result.m_dValue);
	}
	params.push_back(result);
	return true;
}

bool neg(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	if (result.isInteger())
	{
		result.m_lValue = -result.m_lValue;
	}
	else if (result.isDouble())
	{
		result.m_dValue = -result.m_dValue;
	}
	params.push_back(result);
	return true;
}

bool inv(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = 1 / result.m_dValue;
	params.push_back(result);
	return true;
}

bool exp(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = exp(result.m_dValue);
	params.push_back(result);
	return true;
}

bool exp10(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = pow(10., result.m_dValue);
	params.push_back(result);
	return true;
}

bool exp2(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	if (result.isInteger())
	{
		result.m_lValue = 2 ^ result.m_lValue;
	}
	else if (result.isDouble())
	{
		result.m_dValue = pow(2., result.m_dValue);
	}
	params.push_back(result);
	return true;
}

bool ln(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = log(result.m_dValue);
	params.push_back(result);
	return true;
}

bool log10(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = log10(result.m_dValue);
	params.push_back(result);
	return true;
}

bool log2(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = log2(result.m_dValue);
	params.push_back(result);
	return true;
}

bool sin(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertToRads();

	result.m_dValue = sin(result.m_dValue);
	params.push_back(result);
	return true;
}

bool cos(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertToRads();

	result.m_dValue = cos(result.m_dValue);
	params.push_back(result);
	return true;
}

bool tan(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertToRads();

	result.m_dValue = tan(result.m_dValue);
	params.push_back(result);
	return true;
}

bool asin(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = asin(result.m_dValue);
	result.convertFromRads();
	params.push_back(result);
	return true;
}

bool acos(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = acos(result.m_dValue);
	result.convertFromRads();
	params.push_back(result);
	return true;
}

bool atan(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = atan(result.m_dValue);
	result.convertFromRads();
	params.push_back(result);
	return true;
}

bool sinh(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = sinh(result.m_dValue);
	params.push_back(result);
	return true;
}

bool cosh(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = cosh(result.m_dValue);
	params.push_back(result);
	return true;
}

bool tanh(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = tanh(result.m_dValue);
	params.push_back(result);
	return true;
}

bool asinh(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = asinh(result.m_dValue);
	params.push_back(result);
	return true;
}

bool acosh(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = acosh(result.m_dValue);
	params.push_back(result);
	return true;
}

bool atanh(NumStack& params)
{
	Num result = params.back();
	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = atanh(result.m_dValue);
	params.push_back(result);
	return true;
}

bool ceil(NumStack& params)
{
	Num result = params.back();
	if (result.isInteger())
		return true;

	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = ceil(result.m_dValue);
	params.push_back(result);
	return true;
}

bool floor(NumStack& params)
{
	Num result = params.back();
	if (result.isInteger())
		return true;

	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	result.m_dValue = floor(result.m_dValue);
	params.push_back(result);
	return true;
}

bool frac(NumStack& params)
{
	Num result = params.back();
	if (result.isInteger())
		return true;

	params.pop_back();
	result.convertTo(NUM_DOUBLE);
	double intPart;
	result.m_dValue = modf(result.m_dValue, &intPart);
	params.push_back(result);
	return true;

}

bool convert(NumStack& params)
{
	// Second param - convert to number unit/format
	Num inp1 = params.back();
	params.pop_back();
	// First param
	Num inp0 = params.back();

	if (!inp0.convertUnitTo(inp1))
	{
		std::cout << "Conversion from " << inp0.asString()
			<< " to " << inp1.asString() << " <= not implemented yet" << std::endl;
		return false;
	}

	// Replace last value
	params.pop_back();

	params.push_back(inp0);

	return true;
}

bool assign(NumStack& params)
{
    // Function is run-time assignment - NOTE: parsed assignment, var ahead
    Num inp1 = params.back();
    // If Variable name is not set, keep in stack
    if (!inp1.isVar() || (inp1.varName().isNumber()))
    {
        std::cout << "! Assignment (=) into a non-variable cannot be executed!" << std::endl;
        return false;
    }

    // Remove the variable off the stack
    params.pop_back();

    // Takes the last in computation and saves the value in the variable set
    inp1.setNumber(params.back());
    inp1.addOrUpdateVariable();

    return true;
}

bool swapStack(NumStack& params)
{
    Num inp1 = params.back();
    params.pop_back();
	Num inp2 = params.back();
	params.pop_back();
	params.push_back(inp1);
	params.push_back(inp2);

	return true;
}

bool clearStack(NumStack& params)
{
    // Remove the variable off the stack
    params.pop_back();
	return true;
}

// static
std::vector<Functions> s_functions
{
	{"+",    "Add",      F_ADD,  MODE_BINARY, add},
	{"-",    "Subtract", F_SUB,  MODE_BINARY, sub},
	{"*",    "Multiply", F_MUL,  MODE_BINARY, mul},
	{"/",    "Divide",   F_DIV,  MODE_BINARY, div},
	{"^",    "Power",    F_POW,  MODE_BINARY, pow},
	{"\\",   "Root",     F_ROOT, MODE_BINARY, root}, // \x/ y or y^-x (same as power, except '- not needed)
	{"%",    "Modulus",  F_MOD,  MODE_BINARY, mod},
	{"max",  "Maximum",  F_MAX,  MODE_BINARY, max},
	{"min",  "Minimum",  F_MIN,  MODE_BINARY, min},
	{"::",   "Convert",  F_CONV, MODE_CONVERT, convert},  // Unary, but second param looks for #-format

	{";",    "Clear Stack", F_RESULT, MODE_UNARY, clearStack},  // Clear stack (one num)
	{",",    "Push Stack",  F_RESULT, MODE_UNARY, nullptr},  // Push onto stack - not yet defined
	{"[-]",  "Swap Stack",  F_RESULT, MODE_UNARY, swapStack},  // Swaps stack (only if there are two)
	{"swap", "Swap Stack",  F_RESULT, MODE_UNARY, swapStack},  // Swaps stack (only if there are two)

// Unary
	{"sqrt", "Square Root", F_SQRT,   MODE_UNARY, sqrt},  // Test 4-char functions before 3-chars
	{"sqr",  "Square Root", F_SQRT,   MODE_UNARY, sqrt},
	{"abs",  "Absolute",    F_ABS,    MODE_UNARY, abs},
	{"neg",  "Negate",      F_NEG,    MODE_UNARY, neg},    // Negate number
	{"~",    "Negate",      F_NEG,    MODE_UNARY, neg},    // Negate number or bit-wise 1's complement if hex
	{"inv",  "Invert (1/x)",F_INV,    MODE_UNARY, inv},    // 1/x
	{"exp10","10^x",        F_E10X,  MODE_UNARY, exp10},  // 10^x
	{"exp2", "2^x",         F_EXP2,   MODE_UNARY, exp2},   // 2^x
	{"exp",  "e^x",         F_EXP,    MODE_UNARY, exp},    // e^x
	{"e10x", "10^x",        F_E10X,   MODE_UNARY, exp10},  // 10^x
	{"e2x",  "2^x",         F_EXP2,   MODE_UNARY, exp2},   // e^x
	{"log10","Log-base10",  F_LOG,   MODE_UNARY, log10},  // log-base 10
	{"log2", "Log-base2",   F_LOG2,   MODE_UNARY, log2},   // log-base 2
	{"logn", "Natural Log", F_LN,     MODE_UNARY, ln},     // Natural log - test larger char words first
	{"log",  "Log-base10",  F_LOG,    MODE_UNARY, log10},  // log-base 10
	{"ln",   "Natural log", F_LN,     MODE_UNARY, ln},     // Natural log
	{"sin",  "Sine",        F_SIN,    MODE_UNARY, sin},
	{"cos",  "Cosine",      F_COS,    MODE_UNARY, cos},
	{"tan",  "Tangent",     F_TAN,    MODE_UNARY, tan},
	{"asin", "Arc-Sine",    F_ASIN,   MODE_UNARY, asin},   // Returns to deg/rad as defined
	{"acos", "Arc-Cosine",  F_ACOS,   MODE_UNARY, acos},   // Returns to deg/rad as defined
	{"atan", "Arc-Tangent", F_ATAN,   MODE_UNARY, atan},   // Returns to deg/rad as defined
	{"sinh", "Hyperbolic Sine",   F_SINH,   MODE_UNARY, sinh},
	{"cosh", "Hyperbolic Cosine", F_COSH,   MODE_UNARY, cosh},
	{"tanh", "Hyperbolic Tangent",F_TANH,   MODE_UNARY, tanh},
	{"asinh","Arc Hyperb-Sine",   F_ASINH,  MODE_UNARY, asinh},
	{"acosh","Arc-Hyperb-Cosine", F_ACOSH,  MODE_UNARY, acosh},
	{"atanh","Arc-Hyperb-Tangent",F_ATANH,  MODE_UNARY, atanh},

	{"ceil", "Ceiling",    F_CEIL,   MODE_UNARY, ceil},
	{"floor","Floor",      F_FLOOR,  MODE_UNARY, floor},
	{"frac", "Fractional", F_FRAC,   MODE_UNARY, frac},

// Assignment
    {"=",    "Assignment",     F_RESULT, MODE_ASSIGN, assign},  // = 0x400, Function used to update or save variable
    {"+=",   "Sum Assign",     F_RESULT, MODE_ASSIGN, assign},  // Adds to memory
    {"*=",   "Product Assign", F_RESULT, MODE_ASSIGN, assign},  // Multiplies to memory

// Parameter
	{"(", "Open Parenthesis",  F_OPEN_PAREN,  MODE_PARAM, nullptr}, // = 0x200,
	{")", "Close Parenthesis", F_CLOSE_PAREN, MODE_PARAM_END, nullptr},
	{"[", "Open Index Key",    F_OPEN_KEY,    MODE_PARAM, nullptr},
	{"]", "Close Index Key",   F_CLOSE_KEY,   MODE_PARAM_END, nullptr},
	{",", "Separator",         F_SEP,         MODE_PARAM, nullptr},       // comma or semi separator
	{"<[","Open Vector",       F_VECTOR,     MODE_PARAM, nullptr},
	{"<[[", "Open Matrix",     F_MATRIX,    MODE_PARAM, nullptr},
	{"|", "Absolute",          F_BOUND_ABS,   MODE_PARAM, nullptr},
	{"<@", "Save Memory Name", F_SAVE_MEM_NAME,  MODE_PARAM, nullptr},
	{">",  "Close Save",       F_CLOSE_SAVE,  MODE_PARAM_END, nullptr},

};

FunctionType::FunctionType()
	: m_function{"", "", F_NOP, MODE_NORMAL, nop}
{
}

FunctionType::FunctionType(const FunctionValue& type)
	: m_function{"", "", F_NOP, MODE_NORMAL, nop}
{
	Functions f;
	if (getFunc(type, f))
	{
		m_function = f;
	}
	// Otherwise keep it at its initialized state
}

bool FunctionType::run(NumStack& params)
{
	if (m_function.f == nullptr)
	{
		// Keep input as results in stack
		std::cout << "Function: '" << m_function.str.c_str() << "' - not implemented, yet" << std::endl;
		return false;
	}

	size_t len = params.size();

	if (!(m_function.type == F_NOP))
	{
		if (params.empty())
		{
			std::cout << "Function: '" << m_function.str.c_str() << "' - has no arguments" << std::endl;
			return false;
		}

		if (m_function.mode == MODE_ASSIGN)
		{
			// Assignment function requires last number to be a variable
			if ((len < 2) && !params.back().isVar())
			{
				std::cout << "Assignment Function: '" << m_function.str.c_str() << "' - missing variable" << std::endl;
				return false;
			}
		}
		else
		{
			// Check if last value needs value
			params.back().confirm();
		}

		if (m_function.mode == MODE_BINARY)
		{
			if (len < 2)
			{
				std::cout << "Binary Function: '" << m_function.str.c_str() << "' - missing second parameter" << std::endl;
				return false;
			}

			// In binary function, need to check if second to last value exits
			params[len-2].confirm();
		}
	}

	return (*m_function.f)(params);
}

void FunctionType::convertUnits(Num& result, Num& inp0, Num& inp1)
{
	if ((inp0.isDouble()) || (inp1.isDouble()))
	{
		// return results in double:
		inp0.convertTo(NUM_DOUBLE);
		inp1.convertTo(NUM_DOUBLE);
		result.setDouble();
	}
	else
	{
		// Assume integer math
		inp0.convertTo(NUM_INTEGER);
		inp1.convertTo(NUM_INTEGER);
		result.setInteger();
	}
}

bool FunctionType::findFunc(const CalString& str, int& len, Functions& fx)
{
	// Did not find function - assume function was not found
	bool found = false;

	// Clear string length, for now
	len = -1;

	for(auto &it : s_functions)
	{
		int szFnc = it.str.size();
		if( strncmp(str.c_str(), it.str.c_str(), szFnc) == 0)
		{
			// Finds the largest function in the list
			if (szFnc > len)
			{
				fx = it;
				len = szFnc;
			}
			found = true;
		}
	}

	return found;
}

// static
bool FunctionType::getFunc(const FunctionValue type, Functions& func)
{
	for(auto &it : s_functions)
	{
		if( it.type == type)
		{
			func = it;
			return true;
		}
	}
	return false;
}


// static
bool FunctionType::isDefaultRad()
{
	return s_defaultAngle == "rad";
}

// static
void FunctionType::listFunctions(const unsigned types)
{
	for (auto &it : s_functions)
	{
		char strTmp[1024];
		std::string fncType;

		if ((types == 0)
			|| ((types & 1) && (it.mode == MODE_UNARY))
			|| ((types & 2) && (it.mode == MODE_BINARY))
			|| ((types & 4) && (it.mode == MODE_PARAM)))
		{
			switch(it.mode)
			{
				case MODE_BINARY:
					fncType = "Binary";
					break;
				case MODE_UNARY:
					fncType = "Unary";
					break;
				case MODE_PARAM:
					fncType = "Param";
					break;
				default:
					fncType = "Mode";
					break;
			}
#ifdef WIN32
			sprintf_s(strTmp, 1024, "%8s  [%6s] - %s", it.str.c_str(), fncType.c_str(), it.description.c_str());
#else
			sprintf(strTmp, "%8s  [%6s] - %s", it.str.c_str(), fncType.c_str(), it.description.c_str());
#endif
			std::cout << strTmp << std::endl;
		}
	}
}

