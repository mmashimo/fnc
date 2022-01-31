/// @file
///
/// @brief Class implementation for special calculation strings.
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

#include <cstring>
#include "calstring.h"
#include "xstring.h"

#include "func.h"	// Look for operators
#include "num.h"    // Look for constants

#include <stdarg.h>

#include "numUnit.h"	// Used to check if the string is a Unit type

CalString::CalString(const char* str)
	: XString(str == nullptr?"":str)
	, m_type{GenericStringType}
{
}

CalString::CalString(const std::string& str)
	: XString(str)
	, m_type{GenericStringType}
{
}

CalString::CalString(const XString& ref)
	: XString(ref)
	, m_type{GenericStringType}
{
}

CalString::CalString(const CalString& ref)
	: XString(ref)
	, m_type(ref.m_type)
{
}

//CalString::~CalString()
//{
//}

CalString& CalString::operator =(const char* ref)
{
	if (c_str() != ref)
	{
		assign(ref);
		// static_cast<std::string&>(*this) = ref;
		m_type = GenericStringType;
	}
	return *this;
}

CalString& CalString::operator =(const std::string& ref)
{
	if (c_str() != ref.c_str())
	{
		assign(ref);
		// static_cast<std::string&>(*this) = ref;
		m_type = GenericStringType;
	}
	return *this;
}

CalString& CalString::operator =(const CalString& ref)
{
	if (this != &ref)
	{
		assign(static_cast<const std::string&>(ref));
		// static_cast<std::string&>(*this) = static_cast<const std::string&>(ref);
		m_type = ref.m_type;
	}
	return *this;
}

CalString& CalString::operator +=(const char ref)
{
	push_back(ref);
	return *this;
}

CalString& CalString::operator +=(const char* ref)
{
	append(ref);
	return *this;
}

CalString& CalString::operator +=(const std::string& ref)
{
	append(ref);
	return *this;
}

CalString& CalString::operator +=(const CalString& ref)
{
	append(ref);
	return *this;
}

bool CalString::operator ==(const char* ref) const
{
	if (c_str() == ref)
	{
		return true;
	}

	return strcmp(this->c_str(), ref) == 0;
}

bool CalString::operator ==(const std::string& ref) const
{
	if (c_str() == ref.c_str())
	{
		return true;
	}
	return strcmp(this->c_str(), ref.c_str()) == 0;
}

bool CalString::operator ==(const CalString& ref) const
{
	if (this == &ref)
	{
		return true;
	}

	// Must check string type to be exact. If type is of no concern, use 'compare'
	return strcmp(this->c_str(), ref.c_str()) == 0;
}

// Calculation specific functions

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
			tmp.shiftLeft(1);
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

CalString& CalString::cleanDecimal()
{
	// Do only if there is a decimal point
	if (strchr(c_str(), '.') != nullptr)
	{
		int len = size();
		bool done = false;

		while(!done)
		{
			int c = back();
			if ((c == '0') || (c == 0))
			{
				// Strip '0' from end
				len--;
				left(len-1);
			}
			else if (c == '.')
			{
				len--;
				left(len-1);
				done = true;
			}
			else
			{
				done = true;
			}
		}
		return *this;
	}

	return *this;
}

bool CalString::isUnitString() const
{
	UnitDefs def;
	int pos = NumUnit::findUnits(*this, def);
	return pos >= 0;
}


