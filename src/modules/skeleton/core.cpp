/*
	*** Skeleton Module - Core
	*** src/modules/skeleton/core.cpp
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

#include "modules/skeleton/skeleton.h"

// Static Members
List<Module> SkeletonModule::instances_;

/*
 * Constructor / Destructor
 */

// Constructor
SkeletonModule::SkeletonModule() : Module()
{
	// Add to instances list and set unique name for this instance
	instances_.own(this);
	uniqueName_.sprintf("%s%02i", name(), instances_.nItems()-1);

	// Set up variables / control parameters
	setUpKeywords();
}

// Destructor
SkeletonModule::~SkeletonModule()
{
}

/*
 * Instances
 */

// Create instance of this module
List<Module>& SkeletonModule::instances()
{
	return instances_;
}

// Create instance of this module
Module* SkeletonModule::createInstance()
{
	return new SkeletonModule;
}
