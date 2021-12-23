/// @file
///
/// @brief Class implementation for special calculation strings.
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

#include <cstring>
#include "calstring.h"

#include "func.h"	// Look for operators
#include "num.h"    // Look for constants

#include <stdarg.h>

#include "numUnit.h"	// Used to check if the string is a Unit type

CalString::CalString(const char* string)
	: std::string(string == nullptr?"":string)
	, m_type{GenericStringType}
{
}

CalString::CalString(const std::string& string)
	: std::string(string)
	, m_type{GenericStringType}
{
}

CalString::CalString(const CalString& ref)
	: std::string(ref)
	, m_type(ref.m_type)
{
}

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

bool CalString::compare(const CalString& ref, const bool caseSensitive) const
{
	if (caseSensitive)
	{
		return strcmp(this->c_str(), ref.c_str()) == 0;
	}

#ifdef WIN32
	return _stricmp(this->c_str(), ref.c_str()) == 0;
#else
	return strcasecmp(this->c_str(), ref.c_str()) == 0;
#endif
}

bool CalString::compare(const CalString& ref, const int firstChars, const bool caseSensitive) const
{
	if (caseSensitive)
	{
		return strncmp(this->c_str(), ref.c_str(), firstChars) == 0;
	}

#ifdef WIN32
	return _strnicmp(this->c_str(), ref.c_str(), firstChars) == 0;
#else
	return strncasecmp(this->c_str(), ref.c_str(), firstChars) == 0;
#endif
}

CalString& CalString::strcat(const CalString& ref)
{
	append(ref);
#if 0
	size_t count = ref.size();
	for (int i = 0; i < count; i++)
	{
		push_back(ref[i]);
	}
#endif
	return *this;
}

void CalString::fill(const CalString& ref, const int iterations, const bool catenate)
{
	if (!catenate)
	{
		clear();
	}

	for (int i = 0; i < iterations; i++)
	{
		strcat(ref);
	}
}

int CalString::positionOf(const CalString& ref) const
{
	int pos{-1};
	bool bDone{false};

	const char* pstr = strstr(c_str(), ref.c_str());
	if (pstr != nullptr)
	{
		pos = pstr - c_str();
	}

	return pos;
}

// Shifts itself left, discarding the contents
CalString& CalString::shiftLeft(const int shift)
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

CalString& CalString::left(const int chars)
{
	CalString tmp(*this);

	clear();

	for(int i = 0; i < chars; i++)
	{
		push_back(tmp.at(i));
	}
	return *this;
}

int CalString::trimLeft()
{
	int pos = 0;
	while (!empty() && isspace(at(0)))
	{
		// Strip off spaces
		shiftLeft(1);
		pos++;
	}
	return pos;
}

CalString& CalString::format(const char* fmt, ...)
{
	char buffer[1024];
	va_list args;
	va_start (args, fmt);

#ifdef WIN32
	if (vsprintf_s(buffer, 1024, fmt, args) < 0)
#else
	if (vsprintf(buffer, fmt, args) < 0)
#endif
	{
		perror (buffer);
	}

	va_end (args);

	assign(buffer);

	return *this;
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




///////////////////////////////////////////////////////////////////////
// CalcList Class
///////////////////////////////////////////////////////////////////////

/// @brief Copy Constructor
CalcList::CalcList(const CalcList& ref)
{
}

/// @brief Construct from strings with default separator ("")
CalcList::CalcList(const CalString& ref)
{

}

//--- Operators ---

/// @brief Assignment operator
CalcList& CalcList::operator= (const CalcList& ref)
{
	return *this;
}

/// @brief Assignment from breaking down strings
CalcList& CalcList::operator= (const CalString& ref)
{
	push_back(ref);
	return *this;
}

/// @brief Adds to list
CalcList& CalcList::operator+= (const CalcList& ref)
{
	return *this;
}

CalcList& CalcList::operator+= (const CalString& ref)
{
	push_back(ref);
	return *this;
}


CalcList CalcList::operator+ (const CalcList& ref)
{
	return *this;
}

CalcList CalcList::operator+ (const CalString& ref)
{
	return *this;
}

void CalcList::stringSeparator(const CalString& ref, const CalString& separator)
{
	// Start from 0 and look for the next occurence
	CalString tmp = ref;
	int len = separator.size();
	if (len == 0 || (ref.size() == 0))
	{
		return;
	}

	bool bDone{false};

	while(!bDone)
	{
		int pos = tmp.positionOf(separator);
		if (pos > 0)
		{
			// Push into this list
			CalString tmp0 = tmp.left(pos);
			push_back(tmp0);
			tmp.shiftLeft(pos+len);
		}
		else if (pos < 0)
		{
			push_back(tmp);
			bDone = true;
		}
		else
		{
			// if 0, continue searching
			tmp.shiftLeft(len);
		}
	}
}

int CalcList::add(const CalString& str)
{
	push_back(str);
	return size();
}

std::string CalcList::asString() const
{
	std::string out;
	if (!empty())
	{
		for (auto& itr : *this)
		{
			out += itr;
			out += "\n";
		}
	}
	return out;
}
