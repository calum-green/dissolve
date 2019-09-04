/*
	*** Keyword - File and Format
	*** src/keywords/fileandformat.cpp
	Copyright T. Youngs 2012-2019

	This file is part of Dissolve.

	Dissolve is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Dissolve is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Dissolve.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "keywords/fileandformat.h"
#include "base/fileandformat.h"
#include "base/lineparser.h"

// Constructor
FileAndFormatKeyword::FileAndFormatKeyword(FileAndFormat& fileAndFormat) : KeywordData<FileAndFormat&>(KeywordBase::FileAndFormatData, fileAndFormat)
{
}

// Destructor
FileAndFormatKeyword::~FileAndFormatKeyword()
{
}

/*
 * Arguments
 */

// Return minimum number of arguments accepted
int FileAndFormatKeyword::minArguments() const
{
	// Must have data format as bare minimum
	return 1;
}

// Return maximum number of arguments accepted
int FileAndFormatKeyword::maxArguments() const
{
	// Data format and filename, plus some number of optional arguments
	return 99;
}

// Parse arguments from supplied LineParser, starting at given argument offset
bool FileAndFormatKeyword::read(LineParser& parser, int startArg, const CoreData& coreData)
{
	if (!data_.read(parser, startArg)) return Messenger::error("Failed to read file/format.\n");

	set_ = true;

	return true;
}

// Write keyword data to specified LineParser
bool FileAndFormatKeyword::write(LineParser& parser, const char* prefix)
{
	if (!parser.writeLineF("%s%s  %s\n", prefix, name(), data_.asString())) return false;

	return true;
}
