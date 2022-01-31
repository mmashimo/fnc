/// @file
///
/// @brief Header for Base String Utility Class
///
/// This class is used for generic file parsing util, but here, we added (few)
/// functions specific to FNC project just so we can keep this somewhere safe.
/// (NOTE: use of FNC_PROJECT - for 'fnc' project)
///
/// Much of the original code (written in C) has been modified for C++
/// but modified for MS-VC from gcc and returned back to gcc. There are known
/// issues and dissimilarities that are not fully vetted.
///
/// @copyright 2002-2022 - M.Mashimo and all licensors. All rights reserved.
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

class XString : public std::string
{
public:
	// Constructor / Copy constructor
	XString(const char* str = nullptr) : std::string(str == nullptr?"":str) {}
	XString(const std::string& str) : std::string(str) {}
	XString(const XString& ref) : std::string(ref) {}

	// Destructor
	//virtual ~XString();

	// Assignment operator
	XString& operator =(const char* ref);
	XString& operator =(const std::string& ref);
	XString& operator =(const XString& ref);

	XString& operator +=(const char ref);
	XString& operator +=(const char* ref);
	XString& operator +=(const std::string& ref);
	XString& operator +=(const XString& ref);

	bool operator ==(const char* ref) const;
	bool operator ==(const std::string& ref) const;
	bool operator ==(const XString& ref) const;

	bool operator !=(const char* ref) const;
	bool operator !=(const std::string& ref) const;
	bool operator !=(const XString& ref) const;

	// Comparators
	bool compare(const XString& ref, const bool caseSensitive = true) const;
	bool compare(const XString& ref, const int firstChars, const bool caseSensitive = true) const;


	// Functions
	/// @brief Concatenates reference string to itself (same as append) - similar to cstring
	XString& strcat(const XString& ref);

	/// @brief Finds the position of the reference string.
	/// @param[in] ref - reference string to find
	/// @returns -1 (or negative number) if string not found
	int positionOf(const XString& ref) const;

	/// @brief fills entire string with iteration of the another string
	void fill(const XString& ref, const int iterations, const bool catenate = false);

	/// @brief Shifts itself left, discarding the contents
	/// @param[in] shift - number of characters to shift
	/// @return itself with characters shifted
	XString& shiftLeft(const int shift = 1);

	/// @brief Truncates string (on right) keeping chars-length to the left.
	/// @param[in] - number of characters to keep on the left
	/// @return itself only with the left characters
	XString& left(const int chars);

	/// @brief Removes whitespace at left of string. Returns # fo spaces stripped
	/// @return number of characters stripped
	int trimLeft();

	/// @brief C-formats string.
	XString& format(const char* fmt, ...);

};
