/// @file
///
/// @brief Header for Num - Number class.
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

#include "calstring.h"
#include "calclist.h"

#include "numDefs.h"

#include "numUnit.h"

class Num
{
public:
	/// @brief Default constructor
	Num(const double value = 0.,
		const NumberType numType = NUM_DEFAULT,
		const std::string& unit = "",
		const std::string& format = "" );

    Num(const ConstantVars& var);

	/// @brief Copy constructor
	Num(const Num& ref);

	/// @brief Assignment operator
	Num& operator=(const Num& ref);

    /// @brief Direct valur comparison
    bool operator==(const Num& ref);

    /// @brief Compares variables list
    bool operator==(const ConstantVars& var);

	/// @brief Parses string - returns remainder
	bool parse(CalString& eq, CalcList& message);

	/// @brief Parses number from equation
	bool parseNumber(CalString& eq, CalcList& message);

	/// @brief Looks for ":" and parses number format or units 
	bool parseFormat(CalString& eq, CalcList& message);

	/// @brief Parses units after a number
	bool parseUnits(CalString& eq, CalcList& message);

	/// @brief After first set of ##:#, we can parse the rest
	bool parseDateTime(CalString& eq, CalString& num, CalcList& message);

	/// @brief Parse variables - returns -1 if not a variable
	bool parseVar(CalString& eq, CalcList& message);

	/// @brief Parse Date-Time string - return true if number parsing is done (and no need for units)
	bool parseDateTime(CalString& eq, CalcList& message);

	/// @brief Converts numeric type (NUM_INTEGER to NUM_DOUBLE as opposed to units)
	bool convertTo(const NumberType type);

	/// @brief Converts Units (F>C, for example)
	bool convertUnitTo(Num& to);

	/// @brief Convert number to radians (for sin/cos/tan computation), but keep units
	bool convertToRads();

	/// @brief Converts arc-func in rads back to previous unit-type setting
	bool convertFromRads();

    /// @brief Checks if value or variables need updating.
    bool confirm(CalcList& message);

	// Types
	bool isDouble() const 	{ return (m_type & NUM_DOUBLE) != 0; }
	bool isInteger() const	{ return (m_type & NUM_INTEGER) != 0; }
	void setDouble()        { m_type |= NUM_DOUBLE; m_type &= ~NUM_INTEGER; }
	void setInteger()       { m_type |= NUM_INTEGER; m_type &= ~NUM_DOUBLE; }

	bool isVar() const      { return m_type & NUM_VAR; }
	bool isUnsetVar() const { return m_type & (NUM_VAR | NUM_VAR_UNSET); }
    bool isConstant() const { return m_type & NUM_CONSTS; }

	// Returns number by it value
	const double& asDouble() const { return m_dValue; }
	const int64_t& asInt64() const { return m_lValue; }

    /// @brief Sets a new value to a variable (if settable).
    /// @return true if variable set, otherwise false if this number is not a variable
    bool setNumber(const Num& value);

	/// @brief Sets the unit of the number
	Num& setUnit(const NumUnit& unit);

    void setAsVariable(const bool unset = false);

	// Unit types
	bool isRad() const;

	bool updateFromUser();

    // Adds itself as a variable
    bool addOrUpdateVariable(CalcList& message);

	// Prints number as formatted value
	CalString asString() const;

	// Gets the unit object
	const NumUnit& unit() const { return m_unit; }

	const NumberType& type() const { return m_type; }

	const CalString& varName() const { return m_varName; }

	const CalString& numString() const { return m_numString; }

	static bool isNumber(const CalString& string);

	static bool isFormat(const std::string& string);

	static bool isVariable(const CalString& str, int& len, ConstantVars& var);

	/// @brief If Variable is found, use the "Num" value preserved in variable list
	static bool isVariable(const std::string& str, Num& out);

    static void showVariables(bool showAll = false);


private:
	void copyHelper(const Num& ref);

	// Does the number conversion. "to" Num is populated with new value
	bool convertUnit(const Num& to, CalString& func);

	/// @brief Update this Num object using the 'ConstantVars' from pick list
	void updateFromVariable(const ConstantVars& var);

public:

	// Number description
	union {
		double   m_dValue;
		int64_t  m_lValue;
		struct {
			int64_t m_hiValue;
			int64_t m_loValue;
		} m_intParts;
		struct
		{
			double m_realValue;
			double m_imagValue;
		} m_complex;
	};

	// String used to form the numeric value (as opposed to the string used to this Num)
	CalString m_numString;

	// Number-Type - also describes which numeric descriptor is in use
	NumberType	m_type;

	// Units - used to extract conversion methods - allows
	NumUnit  m_unit;

	// Units - desired numeric representation if complex, like time
	CalString m_format;

	// Variable Name used to extract data or the actual number string used to create this Num
	CalString m_varName;

	// Automatic formatting - defaults to true (cleans up decimal/format)
	bool      m_autoFormat;

	static std::vector<ConstantVars> s_constants;

};

class NumStack : public std::vector<Num>
{
public:
	/// @brief Default constructor
	NumStack() {};

	/// @brief Lists the contents
	void list(bool all = false);
};

