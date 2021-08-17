/// @file
///
/// @brief Header for Unit Conversion and Number definitions.
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

#include <vector>

#include "calstring.h"

#include "numDefs.h"

/// @brief Struct to construct Conversion functions.
/// The list is in convert.cpp and constructed to add more unit conversions
/// that are not compiled with the code.
/// TODO: Conversion of different types such as Force or Energy
/// TODO: Conversion unit names from its constructs
typedef struct
{

	UnitType     type;
	CalString   from;
	CalString   to;
	CalString   formula;

} ConversionFunction;

/// @brief Unit Definition class.
/// Currently used to convert like units.
/// TODO: Conversion to different types of units when compiling an expression
class NumUnit
{
public:
	/// @brief Default constructor
	NumUnit();

	/// @brief Construct Unit from input string (either definition or parsed)
	NumUnit(const CalString& type);

	/// @brief Copy constructor
	NumUnit(const NumUnit& ref) {copyHelper(ref);}

	/// @brief Construct from definition struct (especially from unit-def list)
	NumUnit(const UnitDefs& def) {copyHelper(def);}

	/// @brief Destructor - not used as of now
	~NumUnit() {}

	/// @brief Assignment operator
	NumUnit& operator=(const NumUnit& ref)
	{
		copyHelper(ref);
		return *this;
	}

	/// @brief Assignment operation from UnitDef
	NumUnit& operator=(const UnitDefs& def)
	{
		copyHelper(def);
		return *this;
	}

	/// @brief Comparison operator - used to check if the UnitKey is correct.
	/// NOTE: Only for functional comparison, but not for unit-string differences
	bool operator==(const NumUnit& unit);
	bool operator==(const UnitDefs& unit);
	bool operator==(const std::string& unitKey);

	/// @brief More stringent check for unit variable, if changed.
	bool operator!=(const NumUnit& unit);
	bool operator!=(const UnitDefs& unit);
	bool operator!=(const std::string& unitKey);

	/// @brief Compares unit string, either keyed or parsed
	bool compareKeyString(const std::string& strUnit) const;

	/// @brief Checks if the unit is of UnitType
	bool isUnitType(const UnitType& unitType) const;

	/// @brief Checks if the UNit is an Radian Angle type
	bool isRad() const;

	/// @brief Check if Unit is Default (just number)
	bool isDefault() const;

	/// @brief Returns unit-key-string
	const CalString& keyString() const;

	/// @brief Returns UnitType for saving in variables
	const UnitType& unitType() const;

	/// @brief Returns expected NumberType
	const NumberType& numberType() const;

	/// @brief Shows displayed string
	const CalString& asString() const;

	/// @brief Finds the expression that converts to another unit
	bool findConversion(const NumUnit& to, CalString& func);

	/// @brief Finds the expression that converts to another unit
	bool findConversion(const UnitDefs& to, CalString& func);

	/// @brief Finds the Unit Def for a given string
	static int findUnits(const CalString& string, UnitDefs& def);

	/// @brief List of Units other functions can use, if needed
	static std::vector<UnitDefs> s_units;

private:
	void copyHelper(const NumUnit& ref)
	{
		m_units = ref.m_units;
	}
	void copyHelper(const UnitDefs& def)
	{
		m_units = def;
	}

	/// @brief - Internally uses the UnitDef in a list
	UnitDefs m_units;

	/// @brief - Checks if the units are defined
	bool     m_defined;
};



