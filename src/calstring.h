/// @file
///
/// @brief Header file for special handling of calculation strings.
///
/// This class overrides standard string class and manipulates numbers,
/// variables and functions. Its main function is to process incoming strings.
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

#include <string>
#include <vector>
#include "xstring.h"

enum StringType
{
	GenericStringType,	// Incoming string that has not been parsed
	NumberStringType,   // Numeric expressions
	FunctionStringType, // String identified as Function
	VariableStringType, // String identified as a Variable
	UnitStringType,     // String used for Units
	LiteralStringType,  // String with Literal expression or comments
	ErroredString       // String used for expressing an error message
};

class CalString : public XString
{
public:
	// Default constructor
	CalString(const char* string = nullptr);
	CalString(const std::string& string);
	CalString(const XString& ref);
	CalString(const CalString& ref);

	// Destructor
	// virtual ~CalString();

	CalString& operator =(const char* ref);
	CalString& operator =(const std::string& ref);
	CalString& operator =(const CalString& ref);

	CalString& operator +=(const char ref);
	CalString& operator +=(const char* ref);
	CalString& operator +=(const std::string& ref);
	CalString& operator +=(const CalString& ref);

	bool operator ==(const char* ref) const;
	bool operator ==(const std::string& ref) const;
	bool operator ==(const CalString& ref) const;

	bool operator !=(const char* ref) const        { return !(*this == ref); }
	bool operator !=(const std::string& ref) const { return !(*this == ref); }
	bool operator !=(const CalString& ref) const   { return !(*this == ref); }

	//------------- Calculation specific methods -----------------------
	bool isNumber() const;
	bool isFunction() const;
	bool isVariable() const;

	void setNumberType()    { m_type = NumberStringType; }
	void setFunctionType()  { m_type = FunctionStringType; }
	void setVariableType()  { m_type = VariableStringType; }
	void setUnitType()      { m_type = UnitStringType; }
	void setLiteralType()   { m_type = LiteralStringType; }
	void setErrorType()     { m_type = ErroredString; }

	bool isNumberType() const    { return m_type == NumberStringType; }
	bool isFunctionType() const  { return m_type == FunctionStringType; }
	bool isVariableType() const  { return m_type == VariableStringType; }
	bool isUnitType() const      { return m_type == UnitStringType; }
	bool isLiteralType() const   { return m_type == LiteralStringType; }
	bool isErrorType() const     { return m_type == ErroredString; }

	/// @brief Returns string of numbers on the left-side (keeps this string)
	/// @return CalString of digits - empty string if not found
	CalString leftNumbersOnly() const;

	/// @brief Returns string of alphas (a-z,A-Z) on the left-side (keeps this string)
	/// @return CalString of letters - empty string if not found
	CalString leftAlphaOnly() const;

	/// @brief Returns string of alpha-numerics (a-z,A-Z, 0-9) on the left-side (keeps this string)
	/// @return CalString of alphnumeric - empty string if not found
	CalString leftAlphaNumOnly() const;

	/// @brief Returns string of Hexadecimal (0-9, a-f,A-F) on the left-side (keeps this string)
	/// @return CalString of alphnumeric - empty string if not found
	CalString leftHexOnly() const;

	/// @brief Cleans up decimal values (removes trailing zeros) of self
	CalString& cleanDecimal();

	//--- Test for String types ---

	/// @brief Checks if the first few characters is a unit.
	/// @return true if unit string is found
	bool isUnitString() const;


private:
	/// @brief Keeps track of string type
	StringType  m_type;

};

