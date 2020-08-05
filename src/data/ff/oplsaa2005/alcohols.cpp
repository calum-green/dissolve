/*
    *** OPLSAA (2005) Alcohols Forcefield
    *** src/data/ff/oplsaa2005/alcohols.cpp
    Copyright T. Youngs 2019-2020

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

#include "data/ff/oplsaa2005/alcohols.h"

/*
 * Set Up
 */

// Set up / create all forcefield terms
bool Forcefield_OPLSAA2005_Alcohols::setUp()
{
    // Call setup function in OPLS-AA Alkanes base FF
    if (!Forcefield_OPLSAA2005_Alkanes::setUp())
        return false;

    // Copy required types from OPLS-AA (2005) core list
    // -- Mono alcohols
    if (!copyAtomType(oplsAtomTypeById(154), "OH", "nh=1,-C(-O(root,-H))"))
        return false;
    if (!copyAtomType(oplsAtomTypeById(155), "HO", "-&154"))
        return false;
    if (!copyAtomType(oplsAtomTypeById(157), "CT", "nh>=2,-O"))
        return false;
    if (!copyAtomType(oplsAtomTypeById(158), "CT", "nh=1,-O"))
        return false;
    if (!copyAtomType(oplsAtomTypeById(159), "CT", "nh=0,-O"))
        return false;
    // -- Methanol
    if (!copyAtomType(oplsAtomTypeById(156), "HC", "-C(nh=3,-&154)"))
        return false;

    return true;
}

/*
 * Definition
 */

// Return name of Forcefield
const char *Forcefield_OPLSAA2005_Alcohols::name() const { return "OPLSAA2005/Alcohols"; }

// Return description for Forcefield
const char *Forcefield_OPLSAA2005_Alcohols::description() const
{
    static CharString desc("Alcohols from OPLS-AA (2005), covering mono-alcohols only.<br/><br/>References: %s",
                           publicationReferences());
    return desc.get();
}
