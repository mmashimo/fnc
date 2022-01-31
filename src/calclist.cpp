/// @file
///
/// @brief Class implementation for special calculation string lists.
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
#include "calclist.h"



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

int CalcList::updateAddToLast(const CalString& str)
{
	back() += str;
	return size();
}

int CalcList::updateAddToLast(const std::string str)
{
	back() += str;
	return size();
}

const CalString& CalcList::last() const
{
	return back();
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
