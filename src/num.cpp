/// @file
///
/// @brief class implementation for Num - Number class
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


#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

#include <string>

#include <cstdlib>

#include "num.h"
#include "func.h"
#include "exec.h"

#include "numUnit.h"

#include <boost/math/constants/constants.hpp>

/// @brief PI used as a constant.
// From: https://stackoverflow.com/questions/14954066/does-the-c-standard-library-define-pi
// long double __pi = boost::math::constants::pi<long double>();
constexpr double __pi = boost::math::double_constants::pi;

std::vector<ConstantVars> Num::s_constants
{
	{"pi", __pi, NUM_DOUBLE | NUM_CONSTS, UNIT_NUMBER, "rad"}
};


Num::Num(const double value,
		const NumberType numType,
		const std::string& unit,
		const std::string& format)
	: m_dValue(value)
	, m_type(numType)
	, m_unit(unit)
	, m_format(format)
	, m_autoFormat(true)
{
}


Num::Num(const ConstantVars& var)
    : m_dValue(var.value)
    , m_type(var.num_type)
	, m_unit(var.units)
    , m_varName(var.varName)
	, m_autoFormat(true)
{
}

/// @brief Copy constructor
Num::Num(const Num& ref)
{
	copyHelper(ref);
}

/// @brief Assignment operator
Num& Num::operator=(const Num& ref)
{
	copyHelper(ref);
	return *this;
}

void Num::copyHelper(const Num& ref)
{
	m_complex = ref.m_complex;
	m_type = ref.m_type;
	m_unit = ref.m_unit;
	m_format = ref.m_format;
	m_varName = ref.m_varName;
	m_autoFormat = ref.m_autoFormat;
}

bool Num::operator==(const Num& ref)
{
    if (m_type != ref.m_type)
        return false;

	if (m_unit != ref.m_unit)
		return false;

    // No need to check if m_format is the same

    // Numeric interpretation of int64_t should be sufficient
    return m_lValue == ref.m_lValue;
}

bool Num::operator==(const ConstantVars& var)
{
    // Variable name is not the same - all bets off
    if (m_varName != var.varName)
        return false;
    if (m_type != var.num_type)
        return false;
    // Check units by type and string
	if (m_unit != var.units)
		return false;

    return m_dValue == var.value;
}

bool Num::parse(CalString& eq, CalcList& message)
{
	// Remove leading spaces
	eq.trimLeft();

	// Function is assuming we have a number
	m_type = NUM_DEFAULT;

	bool bDone = false;

	while(!bDone && !eq.empty())
	{
		if(eq.isNumber())
		{
			if (!parseNumber(eq, message))
			{
				std::cout << "Num::parse() - errored: " << message.asString() << std::endl;
				return false;
			}
		}
		else if (eq[0] == ':')
		{
			// If we have "::" then we are done with numbers
			if ((eq.size() > 1) && (eq[1] == ':'))
			{
				// we are done-done
				bDone = true;
			}
			else if ((eq.size() > 1) && (eq[1] == '%'))
			{
				// Process format or units or conversion strings
				if (!parseFormat(eq, message))
				{
					return false;
				}
				bDone = true;
			}
			else
			{
				// If units were not found, then stop searching
				if(!parseUnits(eq, message))
					bDone = true;
			}
		}
		else if (isalpha(eq[0]) || eq.isUnitString())
		{
			// If units were not found, the stop searching
			if(!parseUnits(eq, message))
				bDone = true;
		}
		else // if (isspace(eq[i]))
		{
			// Finished numeric entry
			bDone = true;
		}
	}

	return true;
}

/// @brief Parses number from equation
bool Num::parseNumber(CalString& eq, CalcList& message)
{
	if (eq.empty())
	{
		CalString msg;
		msg = "Num::parseNumber() - empty string";
		msg.setErrorType();
		message.add(msg);
		return false;
	}

	int pos = eq.size();
	int i = 0;
	int decPt = 0;

	// Local loop done
	bool done = false;

	// Check for negative number, first
	if ((eq[0] == '-') && (pos > 1) && isdigit(eq[1]))
	{
		m_numString.push_back('-');
		eq.shiftLeft(1);
		--pos;
	}
	// Check for hexidecimal number
	else if ((pos > 2) && (eq[0] == '0') && (eq[1]=='x'))
	{
		eq.shiftLeft(2); // Remove "0x" - expose hex numbers
		CalString tmp = eq.leftHexOnly();
		// There is no decimal point, so collect alpha-numerics using CalString check
		pos = tmp.size();

		m_varName = "0x";

		// If no hex numbers, parse as number
		if (pos > 0)
		{
			char *ptr;
			m_lValue = strtol(tmp.c_str(), &ptr, 16);
			setInteger();
			eq.shiftLeft(pos);
			m_varName += tmp;
			m_varName.setNumberType();
			message.push_back(m_varName);
		}
		else
		{
			m_lValue = 0;
		}
		// Default formatting
		m_format = "0x%X";
		return true;
	}

	while(!done && (pos > 0))
	{
		if (isdigit(eq[0]))
		{
			m_numString.push_back(eq[0]);
			eq.shiftLeft(1);
			pos--;
			i++;
		}
		else if ((eq[0] == '.') && (decPt == 0))
		{
			// Decimal value
			if (i == 0)
			{
				m_numString.push_back('0');
			}
			m_numString.push_back('.');
			eq.shiftLeft(1);
			pos--;
			i++;
			decPt++;
		}
		else if (eq[0] == ':')
		{
			// If ":" is found after a number, check for unit, format or date/time format
			if ((eq.size() == 0) || isdigit(eq[1]))
			{
				// parse date/time format
				done = parseDateTime(eq, message);
			}
			else if (eq[1] == ':')
			{
				// We have "::" which is a conversion function. Let Func::parseFunction() take care of it
				done = true;
			}
			else if (eq[1] == '\'')
			{
				m_numString.push_back(':');
				eq.shiftLeft(1);
				// parse number format
				parseFormat(eq, message);
			}

		}
		else if (i == 0)
		{
			CalString msg;
			msg = "Num::parseNumer '";
			msg += eq.c_str();
			msg += "' <= Not a number";
			msg.setErrorType();
			message.add(msg);
			return false;
		}
		else
		{
			done = true;
		}
	}

	if (m_numString.empty())
		return false;

	m_numString.setNumberType();
	message.push_back(m_numString);

    // Make sure number or variable type is not set
	if ((decPt == 0) && !isDouble())
	{
		setInteger();
		m_lValue = atoi(m_numString.c_str());
	}
	else
	{
		setDouble();
		m_dValue = atof(m_numString.c_str());
	}

	if (m_varName.empty())
	{
		m_varName = m_numString;
	}

	return true;
}

bool Num::parseUnits(CalString& eq, CalcList& message)
{
	UnitDefs def;
	int findPos = NumUnit::findUnits(eq, def);
	if (findPos > 0)
	{
		CalString tmp(eq);
		tmp.left(findPos);

		if (message.back().isNumberType())
		{
			// Add to message type
			tmp.setNumberType();
			message.back() += tmp;
		}
		else
		{
			tmp.setUnitType();
			message.push_back(tmp);
		}

		m_unit = def;

		// Shift out the unit string
		eq.shiftLeft(findPos);
		return true;
	}
	else
	{
		CalString msg;
		msg = "Unit '";
		msg += eq.c_str();
		msg += "' in number/variable: ";
		msg += m_varName.c_str();
		msg += " - is invalid";
		msg.setErrorType();
		message.add(msg);
		return false;
	}

	return false;
}

/// @brief Parses format
bool Num::parseFormat(CalString& eq, CalcList& message)
{
	CalString tmp;
	if (eq.empty())
	{
		tmp = "Num::parseFormat() - empty string!";
		tmp.setErrorType();
		message.add(tmp);
		return false;
	}

	if (eq[0] != ':')
	{
		tmp = "Num::parseFormat(";
		tmp += eq.c_str();
		tmp += ") - is NOT formatted";
		tmp.setErrorType();
		message.add(tmp);
		return false;
	}

	// Remove ":"
	eq.shiftLeft(1);
	if (eq[0] != '%')
	{
		return parseUnits(eq, message);
	}

	// Look for space after formatting
	while (!eq.empty() && !isspace(eq[0]))
	{
		m_format.push_back(eq[0]);
		eq.shiftLeft(1);
	}
	return true;
}

/// @brief Parse variables - returns -1 if not a variable
bool Num::parseVar(CalString& eq, CalcList& message)
{
	int pos = 0;
	UnitDefs def;

	ConstantVars tmpVar{"", 0., NUM_DEFAULT, UNIT_NUMBER, ""};

	if (isVariable(eq, pos, tmpVar))
	{
        updateFromVariable(tmpVar);
	}
	else
	{
		// If not saved variable, make it unset variable - so it will search variable list
		CalString tmp = eq.leftAlphaOnly();

        // Variable name is not set
        if (tmp.empty())
        {
			CalString msg;
            msg += "Variable has no name: ";
            msg += eq;
			msg.setErrorType();
			message.add(msg);
            return false;
        }
		pos = tmp.size();

		// Number type is a variable
        setAsVariable(true);
		// m_type = tmpVar.num_type | NUM_VAR | NUM_VAR_UNSET;

		// Set this number up as variable to be added later
		m_varName = tmp;
	}

	// Shift string populate self
	eq.shiftLeft(pos);

	if (eq.empty())
	{
		return true;
	}

    // Add variable set in list
    if (!tmpVar.units.empty())
    {
        pos = NumUnit::findUnits(tmpVar.units, def);
        if (pos >= 0)
        {
            m_unit = def;
        }
    }

 	// Variable has units
	if (eq[0] == ':')
	{
		CalString tmp(eq);
		tmp.shiftLeft(1);

		// Override tmpVar units, if available
		if (parseUnits(tmp, message))
		{
			eq = tmp;
            if (tmp[0] != '=')
            {
                return true;
            }
		}
        else
        {
            // Must be conversion
            return true;
        }
	}

    // Assign number to variable - must be set at parsing
    if ((eq[0] == '=') && (eq.size() > 1) && (isdigit(eq[1]) || (eq[1] == '-')))
    {
        // It is an assignment, so look for numbers/units/format
        eq.shiftLeft(1);
        if (parse(eq, message))
        {
            addOrUpdateVariable();
            // NOTE returning false prevents variable being added, but continue
        }
        // If there are issues, we will have error msgs
        return false;
    }

	return true;
}

bool Num::parseDateTime(CalString& eq, CalcList& message)
{
	if ((eq.size() == 0) || (eq[1] == ':'))
	{
		return true;
	}

	bool done = false;
	do
	{
	}
	while (!done);

	return true;
}


bool Num::confirm()
{
    // For now, Check if we have a variable and confirm variable has data in it
    if (isConstant() || !isVar())
    {
        // value was derived from s_constant and should not change
        return true;
    }

    ConstantVars tmpVar{ "", 0., NUM_DEFAULT, UNIT_NUMBER, "" };
    int len = -1;
    bool varInList = isVariable(m_varName, len, tmpVar);

    if (varInList)
    {
        updateFromVariable(tmpVar);
    }
    else if (isUnsetVar())
    {
        updateFromUser();
    }

    return varInList;
}


//static
bool Num::isVariable(const CalString& string, int& len, ConstantVars& var)
{
	int i = 0;
	CalString tmp = string.leftAlphaOnly();

	if (tmp.empty())
		return false;

    len = tmp.size();
	for (auto &it : s_constants)
	{
		int sz = it.varName.size();
		if ((sz == len) && (strncmp(it.varName.c_str(), tmp.c_str(), len) == 0))
		{
			var = it;
			return true;
		}
	}
	return false;
}

bool Num::setNumber(const Num& value)
{
    // This number must be a variable, otherwise it cannot override 
    if (!isVar())
    {
        return false;
    }
    // Assumes this variable is a double
    if (value.isDouble())
    {

        m_dValue = value.m_dValue;
        if (!isDouble())
            setDouble();
    }
    else
    {
        if (isDouble())
        {
            m_dValue = static_cast<double>(value.m_lValue);
        }
        else
        {
            // Odd case that this variable is NOT a double
            m_lValue = value.m_lValue;
        }
    }
	return true;
}

void Num::setAsVariable(const bool unset)
{
    if (unset)
    {
        // Make sure all unset variables are doubles
        m_type |= (NUM_DOUBLE | NUM_VAR | NUM_VAR_UNSET);
    }
    else
    {
        // Reset "unset" condition
        m_type &= ~NUM_VAR_UNSET;
        m_type |= NUM_VAR;
    }
}

bool Num::isRad() const
{
	if (m_unit.isRad())
		return true;

	// Default angle setting
	return FunctionType::isDefaultRad();
}

bool Num::updateFromUser()
{
    CalString tmp;
    CalcList message;
    bool updated = false;
    bool done = false;

    // Most likely when it gets here, "isUnsetVar" returned true
	if (Exec::s_showUndefinedVarMsg)
	{
		std::cout << "Functions: variable '" << m_varName << "' is not set. Enter value to continue calculations." << std::endl;
		Exec::s_showUndefinedVarMsg = false;
	}

    do {
        std::cout << m_varName << "=";
        getline(std::cin, tmp);
        if (tmp.empty())
        {
            std::cout << m_varName << "= 0 (integer)" << std::endl;
            done = true;
            // NOTE: if default (0), then variable is not updated
        }
        else if (parse(tmp, message))
        {
            done = true;
            updated = true;
            // Add to variable list
            addOrUpdateVariable();
        }
        else
        {
            std::cout << "Error parsing: '" << tmp << "' - errored: " << message.asString() << " - try again? (<CR> to enter 0 and continue)" << std::endl;
        }
    } while (!done);

	return updated;
}

void Num::updateFromVariable(const ConstantVars& var)
{
    m_varName = var.varName;
    m_dValue = var.value;
    // Number type is a variable - it should have been updated
    m_type = var.num_type;
    setAsVariable();
}

CalString Num::asString() const
{
	CalString tmp;
	const char* fmt;
	bool defaultFormat = m_format.empty();

	if (isInteger())
	{
#ifdef _MSC_VER
		fmt = defaultFormat ? "%lld" : m_format.c_str();
		tmp.format(fmt, m_lValue);
#else
		fmt = defaultFormat ? "%ld" : m_format.c_str();
	    tmp.format(fmt, m_lValue);
#endif
	}
	else
	{
		fmt = defaultFormat ? "%.9f" : m_format.c_str();
		tmp.format(fmt, m_dValue);
		if (defaultFormat)
		{
			tmp.cleanDecimal();
		}
	}

	if (!m_unit.asString().empty())
	{
		tmp += m_unit.asString();
	}

	return tmp;
}

// static
bool Num::isNumber(const CalString& string)
{
	if (string.empty())
		return false;

	if (isdigit(string[0]))
		return true;

	ConstantVars tmpVar{"", 0., NUM_DEFAULT, UNIT_NUMBER, ""};

	int pos = -1;

	// Process negative number
	if (string[0] == '-')
	{
		if (string.size() == 1)
		{
			// This would be a function
			return false;
		}
		return isdigit(string[1]);
	}
	else if (string[0] == ':')
	{
		// TODO: format only
		return true;
	}
	else if (Num::isVariable(string, pos, tmpVar))
	{
		return true;
	}

	return false;
}


bool Num::addOrUpdateVariable()
{
    ConstantVars var;
    int len = m_varName.size();
    const char* vName = m_varName.c_str();

    // This will reset the "unset" flag
    setAsVariable();

    var.varName = m_varName;
    var.value = m_dValue;
    var.num_type = m_type;
    var.unit_type = m_unit.unitType();
    var.units = m_unit.keyString();

    for (auto &it : s_constants)
    {
        int sz = it.varName.size();
		// Make sure it is not a constant
        if ( (sz == len) && (strncmp(it.varName.c_str(), vName, len) == 0))
        {
			if (it.num_type & NUM_CONSTS)
			{
				std::cout << "!!! Attempting to update a constant '" << m_varName << "'" << std::endl;
				return false;
			}
			else if (!(*this == it))
            {
                it = var;
            }
            return true;
        }
    }

    s_constants.push_back(var);

    return false;
}


bool Num::convertTo(const NumberType type)
{
	bool bConverted = false;

	// If the types do not match, convert
	if ((type & m_type) == 0)
	{
		if ((type & NUM_DOUBLE) || (type & NUM_COMPLEX))
		{
			if (isInteger() || (m_type & NUM_INT_PARTS))
			{
				double tmp = (double)m_lValue;
				m_dValue = tmp;
				bConverted = true;
			}
		}
		else if ((type & NUM_INTEGER) || (type & NUM_INT_PARTS))
		{
			if (isDouble() || (m_type & NUM_COMPLEX))
			{
				int64_t tmp = static_cast<int64_t>(m_dValue);
				m_lValue = tmp;
				bConverted = true;
			}
		}
		m_type = type;
	}

	return bConverted;
}

bool Num::convertUnitTo(Num& to)
{
	CalString func;
	if (!convertUnit(to, func))
	{
		std::cout << "Conversion from " << asString()
			<< " to " << to.asString() << " <= not implemented yet" << std::endl;
		return false;
	}

	Exec ex;
	ex.inputParseAndRun(*this, func);

	return true;
}


bool Num::convertUnit(const Num& to, CalString& func)
{
	if (m_unit.findConversion(to.m_unit, func))
	{
		// If conversion completed, units on origin number
		m_unit = to.m_unit;
		convertTo(to.m_unit.numberType());
		return true;
	}
	return false;
}

bool Num::convertToRads()
{
	// Recomputes the number from assumed default (degrees to radians)
	if (isRad())
		return true;
	
	NumUnit conv("deg");
	NumUnit to("rad");

	CalString func;
	if (conv.findConversion(to, func))
	{
		// We cannot modify unit - especially if used
		convertTo(NUM_DOUBLE);

		Exec ex;
		ex.inputParseAndRun(*this, func);

		return true;
	}
	return false;
}

bool Num::convertFromRads()
{
	// Recomputes the number to units defined
	if (isRad())
		return true;

	UnitDefs def;
	int pos = NumUnit::findUnits("rad", def);
	NumUnit conv(def);
	pos = NumUnit::findUnits("deg", def);

	CalString func;
	if (conv.findConversion(def, func))
	{
		// We cannot modify unit
		convertTo(NUM_DOUBLE);

		Exec ex;
		ex.inputParseAndRun(*this, func);

		return true;
	}
	return false;
}

// static
void Num::showVariables(const bool showAll)
{
	int count{0};
	for (auto &it : s_constants)
	{
		Num no(it);
		if (no.isVar())
		{
			std::cout << no.varName() << "=" << no.asString() << std::endl;
			count++;
		}
	}

	if (showAll && (count == 0))
	{
		std::cout << "(none)" << std::endl;
	}
}

void NumStack::list(bool all)
{
	if (empty())
	{
		if (all)
		{
			std::cout << "[Empty]" << std::endl;
		}
		else
		{
			std::cout << "!!! Exec::run() - No results!" << std::endl;
		}
		return;
	}
	else if (!all && (size() == 1))
	{
		std::cout << back().asString() << std::endl;
	}
	else
	{
		// NOTE: Origin 1
		int i = 1;
		for(auto &it: *this)
		{
			std::cout << "[" << i << "] " << it.asString() << std::endl;
			i++;
		}
	}
}
