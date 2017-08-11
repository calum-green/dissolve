/*
	*** Generic Item Container - Vec3<Int>
	*** src/templates/genericitemcontainer_vec3int.h
	Copyright T. Youngs 2012-2017

	This file is part of dUQ.

	dUQ is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	dUQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with dUQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DUQ_GENERICITEMCONTAINER_VEC3INT_H
#define DUQ_GENERICITEMCONTAINER_VEC3INT_H

#include "templates/genericitemcontainer.h"

// GenericItemContainer< Vec3<int> >
template <> class GenericItemContainer< Vec3<int> > : public GenericItem
{
	public:
	// Constructor
	GenericItemContainer< Vec3<int> >(const char* name, int flags = 0) : GenericItem(name, flags)
	{
		itemClass_ = GenericItem::Vec3IntClass;
	}
	// Data item
	Vec3<int> data;


	/*
	 * Parallel Comms
	 */
	public:
	// Broadcast item contents
	bool broadcast(ProcessPool& procPool, int root)
	{
		return procPool.broadcast(data, root);
	}
};

#endif
