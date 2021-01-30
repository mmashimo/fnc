/// @file
///
/// @brief Class implementation for special calcuation strings.
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


#include "calstring.h"

#include "func.h"	// Look for operators
#include "num.h"    // Look for constants

CalString::CalString(const char* string)
	: std::string(string == nullptr?"":string)
{
}

CalString::CalString(const std::string& string)
	: std::string(string)
{
}

CalString::CalString(const CalString& ref)
	: std::string(ref)
{
}

CalString& CalString::operator =(const char* ref)
{
	static_cast<std::string&>(*this) = ref;
	return *this;
}

CalString& CalString::operator =(const std::string& ref)
{
	static_cast<std::string&>(*this) = ref;
	return *this;
}

CalString& CalString::operator =(const CalString& ref)
{
	static_cast<std::string&>(*this) = static_cast<const std::string&>(ref);
	return *this;
}

bool CalString::isNumber() const
{
	if (empty())
		return false;

	char firstChar = at(0);
	if (isdigit(firstChar))
		return true;

	// Process negative number or decimal
	if (size() < 2)
	{
		return false;
	}

	// If second is not digit, then
	if(!isdigit(at(1)))
	{
		return false;
	}

	return ((firstChar == '-') || (firstChar == '.'));
}

bool CalString::isFunction() const
{
	if (empty())
		return false;

	if (isNumber())
		return false;

	// Step through to see if there is a function inside
	CalString tmp = *this;

	Functions func;
	int pos = -1;
	bool done = false;
	while (!done && !tmp.empty())
	{
		if (FunctionType::findFunc(tmp, pos, func))
		{
			done = true;
		}
		else
		{
			// No function on left - shift one char and check
			tmp.left(1);
		}
	}

	return done;
}

bool CalString::isVariable() const
{
	if (empty())
		return false;

	if (!isalpha(at(0)))
		return false;

	ConstantVars var;
	int len = -1;
	return Num::isVariable(*this, len, var);
}


// Shifts itself left, discarding the contents
CalString& CalString::left(const int shift)
{
	CalString tmp = *this;
	int end = size();
	for(int i = 0, j = shift; j < end; i++, j++)
	{
		at(i) = tmp.at(j);
	}
	resize(end-shift);
	return *this;
}

int CalString::trimLeft()
{
	int pos = 0;
	while (!empty() && isspace(at(0)))
	{
		// Strip off spaces
		left(1);
		pos++;
	}
	return pos;
}

CalString CalString::leftNumbersOnly() const
{
	int pos = 0;
	int len = size();
	CalString tmp;
	while((pos < len) && isdigit(at(pos)))
	{
		tmp += at(pos);
		pos++;
	}
	return tmp;
}

CalString CalString::leftAlphaOnly() const
{
	int pos = 0;
	int len = size();
	CalString tmp;
	while((pos < len) && isalpha(at(pos)))
	{
		tmp += at(pos);
		pos++;
	}
	return tmp;
}

CalString CalString::leftAlphaNumOnly() const
{
	int pos = 0;
	int len = size();
	CalString tmp;

	while((pos < len) && isalnum(at(pos)))
	{
		tmp += at(pos);
		pos++;
	}

	return tmp;
}

CalString CalString::leftHexOnly() const
{
	int pos = 0;
	int len = size();
	bool done = false;
	CalString tmp;
	while(!done && (pos<len))
	{
		char ch = at(pos);
		if (((ch >= 'a') && (ch <= 'f'))
			|| ((ch >= 'A') && (ch <='F'))
			|| isdigit(ch))
		{
			tmp += ch;
			pos++;
		}
		else
		{
			done = true;
		}
	}
	return tmp;
}


