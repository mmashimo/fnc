/// @file
///
/// @brief Header file for special handling of calculation stringslist.
///
/// This class overrides standard vector of string class and maintains list of string contexts
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

#include "calstring.h"

// List of parsed strings
class CalcList : public std::vector<CalString>
{
public:
	/// @brief Default Contructor
	CalcList() {};

	/// @brief Copy Constructor
	CalcList(const CalcList& ref);

	/// @brief Construct from strings with default separator ("")
	CalcList(const CalString& ref);

	//--- Operators ---

	/// @brief Assignment operator
	CalcList& operator= (const CalcList& ref);

	/// @brief Assignment from breaking down strings
	CalcList& operator= (const CalString& ref);

	/// @brief Adds to list
	CalcList& operator+= (const CalcList& ref);
	CalcList& operator+= (const CalString& ref);

	CalcList operator+ (const CalcList& ref);
	CalcList operator+ (const CalString& ref);

	/// @brief adds new string to list - similar to "+=", but returns number of items
	int add(const CalString& str);

	/// @brief Updates the last entry - adds new string the last CalString
	int updateAddToLast(const CalString& str);
	int updateAddToLast(const std::string str);

	const CalString& last() const;

	/// @brief Output CalcList as string.
	std::string asString() const;

private:

	void stringSeparator(const CalString& ref, const CalString& separator =" ");

};

