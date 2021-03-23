/// @file
///
/// @brief Header file for special handling of calculation strings.
///
/// This class overrides standard string class and manipulates numbers,
/// variables and functions. Its main function is to process incoming strings.
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

class CalString : public std::string
{
public:
	// Default constructor
	CalString(const char* string = nullptr);
	CalString(const std::string& string);
	CalString(const CalString& ref);

	CalString& operator =(const char* ref);
	CalString& operator =(const std::string& ref);
	CalString& operator =(const CalString& ref);

	bool isNumber() const;
	bool isFunction() const;
	bool isVariable() const;

	// Shifts itself left, discarding the contents
	CalString& left(const int shift = 1);

	/// @brief Removes whitespace at left of string. Returns # fo spaces stripped
	int trimLeft();

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

};
