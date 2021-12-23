/// @file
///
/// @brief Implements Unit-Conversion and Unit defintions lists.
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


#include <vector>
#include <string.h>

#include "numUnit.h"
#include "num.h"

#include "calstring.h"

//----------------------------------------------------------
// Unit Definition
//----------------------------------------------------------

/// @brief Unit-definition list
std::vector<UnitDefs> NumUnit::s_units
{
// UNIT_ANGLE,
	{ "deg", "D", UNIT_ANGLE, NUM_DOUBLE , "deg", "degrees"},
	{ "rad", "R", UNIT_ANGLE, NUM_DOUBLE , "rad", "radians"},
	{ "deg", "deg", UNIT_ANGLE, NUM_DOUBLE , "deg", "degrees"},
	{ "rad", "rad", UNIT_ANGLE, NUM_DOUBLE , "rad", "radians"},
// UNIT_MEMORY,         // Used for conversion of digital storage

// UNIT_TEMPERATURE,    // Scalar
	{ "C", "C",  UNIT_TEMPERATURE, NUM_DOUBLE, "C", "Celsius" },
	{ "F", "F",   UNIT_TEMPERATURE, NUM_DOUBLE, "F", "Fahrenheit" },
	{ "K", "K",   UNIT_TEMPERATURE, NUM_DOUBLE, "K", "Kalvin" },
	{ "C", "degC",   UNIT_TEMPERATURE, NUM_DOUBLE, "C", "Celsius" },
	{ "F", "degF",   UNIT_TEMPERATURE, NUM_DOUBLE, "F", "Fahrenheit" },
	{ "K", "degK",   UNIT_TEMPERATURE, NUM_DOUBLE, "K", "Kalvin" },

// UNIT_LENGTH,         // Scalar
	{ "mm", "mm",  UNIT_LENGTH, NUM_DOUBLE, "mm", "millimeters" },
	{ "cm", "cm",  UNIT_LENGTH, NUM_DOUBLE, "cm", "centimeters" },
	{ "m", "m",   UNIT_LENGTH, NUM_DOUBLE, "m", "meters" },
	{ "km", "km",  UNIT_LENGTH, NUM_DOUBLE, "km", "kilometers" },
	{ "in", "\"",  UNIT_LENGTH, NUM_DOUBLE | NUM_FRACTION, "in", "inches" },
	{ "in", "in",  UNIT_LENGTH, NUM_DOUBLE | NUM_FRACTION, "in", "inches" },
	{ "ft", "ft", UNIT_LENGTH, NUM_DOUBLE, "ft", "feet" },
	{ "yds", "yds", UNIT_LENGTH, NUM_DOUBLE, "yds", "yards" },
	{ "mi", "mi", UNIT_LENGTH, NUM_DOUBLE, "mi", "miles" },
	{ "au", "au", UNIT_LENGTH, NUM_DOUBLE, "au", "Astronomical Units" },
	{ "pars", "pars", UNIT_LENGTH, NUM_DOUBLE, "pars", "Parsecs" },

// UNIT_AREA,
// UNIT_VOLUME,

// UNIT_FREQUENCY
	{ "Hz", "hz", UNIT_FREQUENCY, NUM_DOUBLE, "Hz", "Hertz" },
	{ "Hz", "Hz", UNIT_FREQUENCY, NUM_DOUBLE, "Hz", "Hertz" },
	{ "kHz", "kHz", UNIT_FREQUENCY, NUM_DOUBLE, "kHz", "kilo-Hertz" },
	{ "kHz", "khz", UNIT_FREQUENCY, NUM_DOUBLE, "kHz", "kilo-Hertz" },
	{ "MHz", "MHz", UNIT_FREQUENCY, NUM_DOUBLE, "MHz", "mega-Hertz" },
	{ "MHz", "mhz", UNIT_FREQUENCY, NUM_DOUBLE, "MHz", "mega-Hertz" },
	{ "GHz", "GHz", UNIT_FREQUENCY, NUM_DOUBLE, "GHz", "giga-Hertz" },
	{ "GHz", "ghz", UNIT_FREQUENCY, NUM_DOUBLE, "GHz", "giga-Hertz" },

// UNIT_WAVELENGTH,
	{ "Ang", "Ang", UNIT_WAVELENGTH, NUM_DOUBLE, "A", "Angstroms" },
	{ "Ang", "ang", UNIT_WAVELENGTH, NUM_DOUBLE, "A", "Angstroms" },
	{ "waves", "w", UNIT_WAVELENGTH, NUM_DOUBLE, "waves", "Waves" },
	{ "waves", "wave", UNIT_WAVELENGTH, NUM_DOUBLE, "waves", "Waves" },
	{ "ftw", "ftw", UNIT_WAVELENGTH, NUM_DOUBLE, "ft-wave", "Foot-Wavelength" },

// UNIT_SPEED,
	{ "mph", "mph", UNIT_SPEED, NUM_DOUBLE, "mph", "miles-per-hour" },
	{ "kph", "kph", UNIT_SPEED, NUM_DOUBLE, "kph", "kilometers-per-hour"},

//	UNIT_ACCELERATION,

// 	UNIT_MASS, // Mass as in weight, also
	{ "g",  "g", UNIT_MASS, NUM_DOUBLE, "g", "grams" },
	{ "mg", "mg", UNIT_MASS, NUM_DOUBLE, "mg", "milli-grams" },
	{ "kg", "kg", UNIT_MASS, NUM_DOUBLE, "kg", "killogram" },

//	UNIT_FORCE, // Torque
	{ "N",  "N", UNIT_FORCE, NUM_DOUBLE, "N", "Newtons" },
	{ "ftlbs",  "ftlbs", UNIT_FORCE, NUM_DOUBLE, "ftlbs", "Foot-Pounds" },
	{ "ftlbs",  "ftlb", UNIT_FORCE, NUM_DOUBLE, "ftlbs", "Foot-Pounds" },

//	UNIT_ENERGY,
	{ "J",  "J", UNIT_ENERGY, NUM_DOUBLE, "J", "Joules" },

//	UNIT_WORK,

// 	UNIT_PRESSURE,
	{ "atm", "atm", UNIT_PRESSURE, NUM_DOUBLE, "atm", "Atmostpheres" },
	{ "psi", "psi", UNIT_PRESSURE, NUM_DOUBLE, "psi", "Pounds-per-square-inch" },
	{ "kPa", "kpa", UNIT_PRESSURE, NUM_DOUBLE, "kPa", "kilo-Pascals" },
	{ "kPa", "kPa", UNIT_PRESSURE, NUM_DOUBLE, "kPa", "kilo-Pascals" },

// UNIT_VOLTAGE,
	{ "kV",  "kV", UNIT_VOLTAGE, NUM_DOUBLE, "kV", "Kilo-Volts" },
	{ "V",  "V", UNIT_VOLTAGE, NUM_DOUBLE, "V", "Volts" },
	{ "mV",  "mV", UNIT_VOLTAGE, NUM_DOUBLE, "mV", "Milli-Volts" },
	{ "uV",  "uV", UNIT_VOLTAGE, NUM_DOUBLE, "uV", "Micro-Volts" },

// UNIT_CURRENT,
	{ "A",  "A", UNIT_CURRENT, NUM_DOUBLE, "A", "Amps" },
	{ "mA", "mA", UNIT_CURRENT, NUM_DOUBLE, "mA", "Milli-Amps" },
	{ "uA", "uA", UNIT_CURRENT, NUM_DOUBLE, "mA", "Micro-Amps" },
// UNIT_RESISTANCE,
	{ "ohm",  "Ohm", UNIT_RESISTANCE, NUM_DOUBLE, "Ohm", "Ohms" },
	{ "Ohm",  "Ohm", UNIT_RESISTANCE, NUM_DOUBLE, "Ohm", "Ohms" },
	{ "kOhm",  "kOhm", UNIT_RESISTANCE, NUM_DOUBLE, "kOhm", "Kilo-Ohms" },
	{ "kOhm",  "kohm", UNIT_RESISTANCE, NUM_DOUBLE, "kOhm", "Kilo-Ohms" },
	{ "MOhm",  "MOhm", UNIT_RESISTANCE, NUM_DOUBLE, "MOhm", "Mega-Ohms" },
	{ "MOhm",  "mohm", UNIT_RESISTANCE, NUM_DOUBLE, "MOhm", "Mega-Ohms" },

// UNIT_CAPACITANCE,
	{ "capF",  "cF", UNIT_CAPACITANCE, NUM_DOUBLE, "F", "Farads" },
	{ "capF",  "capF", UNIT_CAPACITANCE, NUM_DOUBLE, "F", "Farads" },
	{ "mF",  "mF", UNIT_CAPACITANCE, NUM_DOUBLE, "mF", "Milli-Farads" },
	{ "uF",  "uF", UNIT_CAPACITANCE, NUM_DOUBLE, "uF", "Micro-Farads" },
	{ "nF",  "nF", UNIT_CAPACITANCE, NUM_DOUBLE, "nF", "Nano-Farads" },
	{ "pF",  "pF", UNIT_CAPACITANCE, NUM_DOUBLE, "pF", "Pico-Farads" },

// UNIT_INDUCTANCE,
	{ "indH",  "iH", UNIT_INDUCTANCE, NUM_DOUBLE, "H", "Henrys" },
	{ "indmH", "mH", UNIT_INDUCTANCE, NUM_DOUBLE, "mH", "Milli-Henrys" },

// UNIT_TIME - Also Duration
	{ "ns", "ns", UNIT_TIME, NUM_TIME, "ns", "nano-seconds" },
	{ "us", "us", UNIT_TIME, NUM_TIME, "us", "micro-seconds" },
	{ "ms", "ms", UNIT_TIME, NUM_TIME, "ms", "milli-seconds" },
	{ "sec", "S", UNIT_TIME, NUM_TIME, "sec", "Seconds" },
	{ "min", "M", UNIT_TIME, NUM_TIME, "min", "Minutes" },
	{ "hrs", "H", UNIT_TIME, NUM_TIME, "hrs", "Hours" },
	{ "sec", "sec", UNIT_TIME, NUM_TIME, "sec", "Seconds" },
	{ "min", "min", UNIT_TIME, NUM_TIME, "min", "Minutes" },
	{ "hr", "hr", UNIT_TIME, NUM_TIME, "hrs", "Hours" },
	{ "am", "am", UNIT_TIME, NUM_TIME, "am", "AM" },
	{ "pm", "pm", UNIT_TIME, NUM_TIME, "pm", "PM" },
// UNIT_DATETIME,       // Expresses Date Date/Time
	{ "days", "dy", UNIT_TIME, NUM_TIME, "days", "Days" },
	{ "mon", "mon", UNIT_TIME, NUM_TIME, "mon", "Months" },
	{ "yrs", "yrs", UNIT_TIME, NUM_TIME, "yrs", "Years" },
	{ "Jd", "JD", UNIT_TIME, NUM_TIME, "Jd", "Julian-Days" },
	{ "Jd", "Jd", UNIT_TIME, NUM_TIME, "Jd", "Julian-Days" },
	{ "J2000", "J2k", UNIT_TIME, NUM_TIME, "J2000", "J2000" },
	{ "J2000", "J2000", UNIT_TIME, NUM_TIME, "J2000", "J2000" },

// UNIT_TIME_T  = 0x80, // Described in time_t
// UNIT_TIME_MS,        // Described in MS time
// UNIT_DATE_JULIAN,    // Double described in Julian UTC date-time
// UNIT_DATE_J2000,     // Double described in J2000 UTC date-time
// UNIT_DATE_LOCAL      //

};



// Conversion is a method used to convert units into another
// For most conversions, there is a fixed numeric value - either
// multiplication or division to get the conversion. The structure
// enables from-to to follow these cases in a scripted manner. Few cases
// such as temperature and radian/degree requires a formula. This
// is conversion for both of these conversion functions.

std::vector<ConversionFunction> s_conversions
{
	{UNIT_ANGLE,       "deg", "rad", "*pi/180"},
	{UNIT_ANGLE,       "rad", "deg", "*180/pi"},
//	{UNIT_FRACTION,       // Unit conversion for Number as fractions
//	{UNIT_COMPLEX,        // Unit conversion for Complex number
//	{UNIT_MEMORY,         // Used for conversion of digital storage
	{UNIT_TEMPERATURE, "C",   "F", "*9/5+32."},
	{UNIT_TEMPERATURE, "F",   "C", "- 32.*5/9"},

	{UNIT_LENGTH,      "mm",  "cm", "/10."},
	{UNIT_LENGTH,      "mm",  "m", "/1000."},
	{UNIT_LENGTH,      "mm",  "km", "/1000000."},
	{UNIT_LENGTH,      "mm",  "in", "/25.4"},
	{UNIT_LENGTH,      "mm",  "ft", "/304.8"},
	{UNIT_LENGTH,      "mm",  "yds", "/914.4"},
	{UNIT_LENGTH,      "mm",  "mi", "/1609344."},
	{UNIT_LENGTH,      "cm",  "mm", "*10."},
	{UNIT_LENGTH,      "cm",  "m", "/100."},
	{UNIT_LENGTH,      "cm",  "km", "/100000."},
	{UNIT_LENGTH,      "cm",  "in", "/2.54"},
	{UNIT_LENGTH,      "cm",  "ft", "/30.48"},
	{UNIT_LENGTH,      "cm",  "yds", "/91.44"},
	{UNIT_LENGTH,      "cm",  "mi", "/160934.4"},
	{UNIT_LENGTH,      "m",  "mm", "*1000."},
	{UNIT_LENGTH,      "m",  "cm", "100."},
	{UNIT_LENGTH,      "m",  "km", "/1000."},
	{UNIT_LENGTH,      "m",  "in", "*100./2.54"},
	{UNIT_LENGTH,      "m",  "ft", "/0.3048"},
	{UNIT_LENGTH,      "m",  "yds", "/0.9144"},
	{UNIT_LENGTH,      "m",  "mi", "/1609.344"},
	{UNIT_LENGTH,      "km",  "mm", "*1000000."},
	{UNIT_LENGTH,      "km",  "cm", "100000."},
	{UNIT_LENGTH,      "km",  "m", "*1000."},
	{UNIT_LENGTH,      "km",  "in", "*100000./2.54"},
	{UNIT_LENGTH,      "km",  "ft", "*1000./0.3048"},
	{UNIT_LENGTH,      "km",  "yds", "*1000./0.9144"},
	{UNIT_LENGTH,      "km",  "mi", "/1.609344"},

	{UNIT_LENGTH,      "in",  "mm", "*25.4"},
	{UNIT_LENGTH,      "in",  "cm", "*2.54"},
	{UNIT_LENGTH,      "in",  "m", "*2.54/100."},
	{UNIT_LENGTH,      "in",  "km", "*2.54/100000."},
	{UNIT_LENGTH,      "in",  "ft", "/12."},
	{UNIT_LENGTH,      "in",  "yds", "/36."},
	{UNIT_LENGTH,      "in",  "mi", "/63360."},
	{UNIT_LENGTH,      "ft",  "mm", "*304.8"},
	{UNIT_LENGTH,      "ft",  "cm", "*30.48"},
	{UNIT_LENGTH,      "ft",  "m", "*0.3048"},
	{UNIT_LENGTH,      "ft",  "km", "*0.3048/1000."},
	{UNIT_LENGTH,      "ft",  "in", "*12."},
	{UNIT_LENGTH,      "ft",  "yds", "/3."},
	{UNIT_LENGTH,      "ft",  "mi", "/5280."},
	{UNIT_LENGTH,      "yds",  "mm", "*914.4"},
	{UNIT_LENGTH,      "yds",  "cm", "*91.44"},
	{UNIT_LENGTH,      "yds",  "m", "*0.9144"},
	{UNIT_LENGTH,      "yds",  "km", "*0.9144/1000."},
	{UNIT_LENGTH,      "yds",  "in", "*36."},
	{UNIT_LENGTH,      "yds",  "ft", "*3."},
	{UNIT_LENGTH,      "yds",  "mi", "/1760."},
	{UNIT_LENGTH,      "mi",  "mm", "*1609344."},
	{UNIT_LENGTH,      "mi",  "cm", "*160934.4"},
	{UNIT_LENGTH,      "mi",  "m", "*1609.344"},
	{UNIT_LENGTH,      "mi",  "km", "*1.609344"},
	{UNIT_LENGTH,      "mi",  "in", "*63360."},
	{UNIT_LENGTH,      "mi",  "ft", "*5280"},
	{UNIT_LENGTH,      "mi",  "yds", "*1760."},
//	{UNIT_LENGTH, !!!
//	{UNIT_AREA,   !!!
//	{UNIT_VOLUME,
//	{UNIT_TIME,   !!!
//	{UNIT_FREQUENCY, !!!
//	{UNIT_WAVELENGTH,
	{UNIT_SPEED,       "mph", "kph", "/1.609344"},
	{UNIT_SPEED,       "kph", "mph", "*1.609344"},
//	{UNIT_ACCELERATION,
//	{UNIT_MASS,
//	{UNIT_FORCE,
//	{UNIT_ENERGY,
//	{UNIT_WORK,
//	{UNIT_PRESSURE,
//	{UNIT_VOLTAGE,
//	{UNIT_CURRENT,
//	{UNIT_RESISTANCE,
//	{UNIT_CAPACITANCE,
//	{UNIT_INDUCTANCE,
//	{UNIT_TIME_T  = 0x80, // Described in time_t
//	{UNIT_TIME_MS,        // Described in MS time
//	{UNIT_DATE_JULIAN,    // Double described in Julian UTC date-time
//	{UNIT_DATE_J2000,     // Double described in J2000 UTC date-time
//	{UNIT_DATE_LOCAL,      //
	
};

/// @brief Default constructors
NumUnit::NumUnit()
	: m_units{"", "", UNIT_NUMBER,NUM_DEFAULT, "", ""}
	, m_defined{false}
{
	// Intensionally left blank
}

NumUnit::NumUnit(const CalString& type)
	: m_units{"", "", UNIT_NUMBER,NUM_DEFAULT, "", ""}
	, m_defined{false}
{
	// If not found, leaves the units alone
	findUnits(type, m_units);
}

bool NumUnit::operator==(const NumUnit& unit)
{
	return *this == unit.m_units;
}

bool NumUnit::operator==(const UnitDefs& unit)
{
	// Only need to check if the key-string is the same
	return compareKeyString(unit.unitKey);
}

bool NumUnit::operator==(const std::string& unitKey)
{
	return compareKeyString(unitKey);
}

bool NumUnit::operator!=(const NumUnit& unit)
{
	return *this != unit.m_units;
}

bool NumUnit::operator!=(const UnitDefs& unit)
{
	if (m_units.unitType != unit.unitType)
		return true;
	
	if (m_units.expectType != unit.expectType)
		return true;

	return !compareKeyString(unit.unitKey);
}

bool NumUnit::operator!=(const std::string& unitKey)
{
	return !compareKeyString(unitKey);
}

bool NumUnit::compareKeyString(const std::string& strUnit) const
{
	return m_units.unitKey == strUnit;
}

bool NumUnit::isUnitType(const UnitType& unitType) const
{
	return m_units.unitType == unitType;
}

bool NumUnit::isRad() const
{
	return (isUnitType(UNIT_ANGLE) && compareKeyString("rad"));
}

bool NumUnit::isDefault() const
{
	return m_units.expectType == NUM_DEFAULT;
}

bool NumUnit::findConversion(const NumUnit& to, CalString& func)
{
	return findConversion(to.m_units, func);
}

bool NumUnit::findConversion(const UnitDefs& to, CalString& func)
{
	if ((m_units.unitType != UNIT_NUMBER) && (m_units.unitType != to.unitType))
		return false;

	for (auto &it : s_conversions)
	{
		if((it.type == to.unitType)
			&& (strcmp(m_units.unitKey.c_str(), it.from.c_str()) == 0)
			&& (strcmp(to.unitKey.c_str(), it.to.c_str()) == 0))
		{
			func = it.formula;
			return true;
		}
	}
	return false;
}


// static
int NumUnit::findUnits(const CalString& string, UnitDefs& def)
{
	int len = string.size();
	int foundLen = -1;
	if (len > 0)
	{
		for (auto &it : s_units)
		{
			int sz = it.unitStr.size();
			if (strncmp(string.c_str(), it.unitStr.c_str(), sz) == 0)
			{
				if (sz > foundLen)
				{
					foundLen = sz;
					def = it;
				}
			}
		}
	}
	return foundLen;
}

const CalString& NumUnit::keyString() const
{
	return m_units.unitKey;
}

const UnitType& NumUnit::unitType() const
{
	return m_units.unitType;
}

const NumberType& NumUnit::numberType() const
{
	return m_units.expectType;
}

const CalString& NumUnit::asString() const
{
	return m_units.displayed;
}
