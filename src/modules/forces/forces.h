/*
	*** Forces Module
	*** src/modules/forces/forces.h
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

#ifndef DISSOLVE_FORCESMODULE_H
#define DISSOLVE_FORCESMODULE_H

#include "module/module.h"

// Forward Declarations
class PotentialMap;

// Forces Module
class ForcesModule : public Module
{
	/*
	 * Calculates the total energy of the system by one of several methods
	 */

	public:
	// Constructor
	ForcesModule();
	// Destructor
	~ForcesModule();


	/*
	 * Instances
	 */
	protected:
	// List of all instances of this Module type
	static List<Module> instances_;

	public:
	// Return list of all created instances of this Module
	List<Module>& instances();
	// Create instance of this module
	Module* createInstance();


	/*
	 * Definition
	 */
	public:
	// Return name of module
	const char* name();
	// Return brief description of module
	const char* brief();
	// Return instance type for module
	InstanceType instanceType();
	// Return the maximum number of Configurations the Module can target (or -1 for any number)
	int nTargetableConfigurations();
	// Modules upon which this Module depends to have run first
	const char* dependentModules();
	// Set up supplied dependent module (only if it has been auto-added)
	bool setUpDependentModule(Module* depMod);


	/*
	 * Options
	 */
	protected:
	// Set up options for Module
	void setUpKeywords();
	// Parse complex keyword line, returning true (1) on success, false (0) for recognised but failed, and -1 for not recognised
	int parseComplexKeyword(ModuleKeywordBase* keyword, LineParser& parser, Dissolve* dissolve, GenericList& targetList, const char* prefix);


	/*
	 * Processing
	 */
	private:
	// Run main processing
	bool process(Dissolve& dissolve, ProcessPool& procPool);

	public:
	// Whether the Module has a processing stage
	bool hasProcessing();


	/*
	 * Force Methods
	 */
	public:
	// Calculate total intramolecular forces
	static void intramolecularForces(ProcessPool& procPool, Configuration* cfg, const PotentialMap& potentialMap, Array<double>& fx, Array<double>& fy, Array<double>& fz);
	// Calculate interatomic forces within the specified Configuration
	static void interatomicForces(ProcessPool& procPool, Configuration* cfg, const PotentialMap& potentialMap, Array<double>& fx, Array<double>& fy, Array<double>& fz);
	// Calculate total forces within the system
	static void totalForces(ProcessPool& procPool, Configuration* cfg, const PotentialMap& potentialMap, Array<double>& fx, Array<double>& fy, Array<double>& fz);
};

#endif

