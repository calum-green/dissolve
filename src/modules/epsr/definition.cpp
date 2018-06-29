/*
	*** EPSR Module - Definition
	*** src/modules/epsr/definition.cpp
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

#include "modules/epsr/epsr.h"

// Return name of module
const char* EPSRModule::name()
{
	return "EPSR";
}

// Return brief description of module
const char* EPSRModule::brief()
{
	return "Refine interatomic potentials in a manner consistent with EPSR";
}

// Return instance type for module
Module::InstanceType EPSRModule::instanceType()
{
	return Module::MultipleInstance;
}

// Return the maximum number of Configurations the Module can target (or -1 for any number)
int EPSRModule::nTargetableConfigurations()
{
	return 0;
}

// Modules upon which this Module depends to have run first
const char* EPSRModule::dependentModules()
{
	// If the 'OnlyWhenEnergyStable' keyword is 'true', we require the Energy module
	// TODO This needs to be updated since Partials module doesn't exist. Just check for presence of EnergyStable while running instead?
	if (keywords_.asBool("OnlyWhenEnergyStable")) return "Energy";
	else return "";
}

// Set up supplied dependent module (only if it has been auto-added)
bool EPSRModule::setUpDependentModule(Module* depMod)
{
	if (DissolveSys::sameString(depMod->name(), "Energy"))
	{
		// Must add all Configuration targets in the associated Partials Module
		Module* partialsModule = dependentModule("Partials");
		if (!partialsModule) return false;
		depMod->copyTargetConfigurations(partialsModule);
	}
	return true;
}