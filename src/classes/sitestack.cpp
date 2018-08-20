/*
	*** Site Stack
	*** src/classes/sitestack.cpp
	Copyright T. Youngs 2012-2018

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

#include "classes/sitestack.h"
#include "base/processpool.h"

// Constructor
SiteStack::SiteStack() : ListItem<SiteStack>()
{
	site_ = NULL;
	configuration_ = NULL;
	nSites_ = 0;
	sitesInMolecules_ = false;
	sitesHaveAxes_ = false;
}

// Destructor
SiteStack::~SiteStack()
{
}
