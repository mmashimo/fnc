/// @file
///
/// @brief Header for Number and Unit Definitions
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

#include <string>

using NumberType = uint32_t;

constexpr NumberType NUM_DEFAULT  {0};    // un-initialized state
constexpr NumberType NUM_DOUBLE   {0x100}; // General use
constexpr NumberType NUM_INTEGER  {0x200}; // Integer 64 use (NOTE: use of radix last byte)
constexpr NumberType NUM_COMPLEX  {0x400}; // Real/imaginery components
constexpr NumberType NUM_INT_PARTS{0x800}; // Used to describe numbers with split integers or unsigned hexidecimal (radix last byte)
constexpr NumberType NUM_FRACTION {0x1000}; // Double described as fraction
constexpr NumberType NUM_TIME     {0x2000};
constexpr NumberType NUM_CONSTS   {0x4000}; // Cannot change (read-only), must be set
constexpr NumberType NUM_VAR      {0x8000}; // Can change - bitwise Number type, also
constexpr NumberType NUM_VAR_UNSET{0x10000}; // Variable value is undefined

enum UnitType : uint16_t
{
	UNIT_NUMBER    = 0,  // value has no units
	UNIT_ANGLE,
	UNIT_FRACTION,       // Unit conversion for Number as fractions
	UNIT_COMPLEX,        // Unit conversion for Complex number
	UNIT_MEMORY,         // Used for conversion of digital storage
	UNIT_TEMPERATURE,    // Scalar
	UNIT_LENGTH,         // Scalar
	UNIT_AREA,
	UNIT_VOLUME,
	UNIT_FREQUENCY,
	UNIT_WAVELENGTH,
	UNIT_SPEED,
	UNIT_ACCELERATION,
	UNIT_MASS,
	UNIT_FORCE,          // Torque
	UNIT_ENERGY,
	UNIT_WORK,
	UNIT_PRESSURE,
	UNIT_VOLTAGE,
	UNIT_CURRENT,
	UNIT_RESISTANCE,
	UNIT_CAPACITANCE,
	UNIT_INDUCTANCE,

	UNIT_TIME,           // Also Duration
	UNIT_DATETIME,       // Expresses Date Date/Time
	//--- Numbers using structures other than doubles
	UNIT_TIME_T  = 0x80, // Described in time_t
	UNIT_TIME_MS,        // Described in MS time
	UNIT_DATE_JULIAN,    // Double described in Julian UTC date-time
	UNIT_DATE_J2000,     // Double described in J2000 UTC date-time
	UNIT_DATE_LOCAL      //
};

/// @brief Unit Definition struct.
/// Used to construct s_units
typedef struct _unit_defs
{
	std::string unitKey;    // Used for Unit conversion
	std::string unitStr;	// Parsed unit
	UnitType    unitType;
	NumberType  expectType; // Expected number type, if any
	std::string displayed;
	std::string description;
} UnitDefs;

/// @brief Constant definition struct.
/// Used to construct Constants and Variables
typedef struct _constantVars
{
	std::string varName;
	double      value;
	NumberType  num_type;
	UnitType    unit_type;
	std::string units;

} ConstantVars;
