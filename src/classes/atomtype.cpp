/*
	*** AtomType Definition
	*** src/classes/atomtype.cpp
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

#include "classes/atomtype.h"
#include "base/ptable.h"
#include "base/processpool.h"
#include <string.h>

// Constructor
AtomType::AtomType() : MPIListItem<AtomType>()
{
	element_ = 0;
	name_ = "XX";
	exchangeable_ = false;
}

// Destructor
AtomType::~AtomType()
{
}

/*
 * Properties
 */

// Set name of AtomType
void AtomType::setName(const char* name)
{
	name_ = name;
}

// Return name of AtomType
const char* AtomType::name() const
{
	return name_.get();
}

// Set atomic element
void AtomType::setElement(int el)
{
	element_ = el;
}

// Return atomic element
int AtomType::element() const
{
	return element_;
}

// Return interaction Parameters
Parameters& AtomType::parameters()
{
	return parameters_;
}


// Set whether this AtomType is exchangeable
void AtomType::setExchangeable(bool b)
{
	exchangeable_ = b;
}

// Return whether this AtomType is exchangeable
bool AtomType::exchangeable() const
{
	return exchangeable_;
}

// Set index of this type in the main type index
void AtomType::setIndex(int id)
{
	index_ = id;
}

// Return index of this type in the main type index
int AtomType::index() const
{
	return index_;
}

/*
 * Parallel Comms
 */

// Broadcast data from Master to all Slaves
bool AtomType::broadcast(ProcessPool& procPool, int root)
{
#ifdef PARALLEL
	// Send name
	procPool.broadcast(name_, root);
	
	// Send element
	procPool.broadcast(element_,root);
	
	// Send Parameters
	parameters_.broadcast(procPool, root);

	// Send exchangeable flag
	procPool.broadcast(exchangeable_, root);
#endif
	return true;
}
