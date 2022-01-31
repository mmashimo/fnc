/// @file
///
/// @brief Implementation for Base String Utility Class
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

#include "xstring.h"
#include <cstring>

#include <stdarg.h>

#if 0
XString::XString(const char* str)
	: std::string(str == nullptr?"":str)
{
}

XString::XString(const std::string& str)
	: std::string(str)
{
}

XString::XString(const XString& ref)
	: std::string(ref)
{
}
#endif

//XString::~XString()
//{
//}


XString& XString::operator =(const char* ref)
{
	if (c_str() != ref)
	{
		assign(ref);
	}
	return *this;
}

XString& XString::operator =(const std::string& ref)
{
	if (c_str() != ref.c_str())
	{
		assign(ref);
	}
	return *this;
}

XString& XString::operator =(const XString& ref)
{
	if (this != &ref)
	{
		assign(static_cast<const std::string&>(ref));
	}
	return *this;
}

XString& XString::operator +=(const char ref)
{
	push_back(ref);
	return *this;
}

XString& XString::operator +=(const char* ref)
{
	append(ref);
	return *this;
}

XString& XString::operator +=(const std::string& ref)
{
	append(ref);
	return *this;
}

XString& XString::operator +=(const XString& ref)
{
	append(ref);
	return *this;
}

bool XString::operator ==(const char* ref) const
{
	if (c_str() == ref)
	{
		return true;
	}

	return strcmp(this->c_str(), ref) == 0;
}

bool XString::operator ==(const std::string& ref) const
{
	if (c_str() == ref.c_str())
	{
		return true;
	}
	return strcmp(this->c_str(), ref.c_str()) == 0;
}

bool XString::operator ==(const XString& ref) const
{
	if (this == &ref)
	{
		return true;
	}

	// Must check string type to be exact. If type is of no concern, use 'compare'
	return strcmp(this->c_str(), ref.c_str()) == 0;
}

bool XString::operator !=(const char* ref) const
{
	return !(*this == ref);
}

bool XString::operator !=(const std::string& ref) const
{
	return !(*this == ref);
}

bool XString::operator !=(const XString& ref) const
{
	return !(*this == ref);
}


bool XString::compare(const XString& ref, const bool caseSensitive) const
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

bool XString::compare(const XString& ref, const int firstChars, const bool caseSensitive) const
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

XString& XString::strcat(const XString& ref)
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

void XString::fill(const XString& ref, const int iterations, const bool catenate)
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

int XString::positionOf(const XString& ref) const
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
XString& XString::shiftLeft(const int shift)
{
	XString tmp = *this;
	int end = size();
	for(int i = 0, j = shift; j < end; i++, j++)
	{
		at(i) = tmp.at(j);
	}
	resize(end-shift);
	return *this;
}

XString& XString::left(const int chars)
{
	XString tmp(*this);

	clear();

	for(int i = 0; i < chars; i++)
	{
		push_back(tmp.at(i));
	}
	return *this;
}

int XString::trimLeft()
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

XString& XString::format(const char* fmt, ...)
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



