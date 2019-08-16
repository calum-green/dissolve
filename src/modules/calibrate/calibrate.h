/*
	*** Calibration Module
	*** src/modules/calibrate/calibrate.h
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

#ifndef DISSOLVE_CALIBRATIONMODULE_H
#define DISSOLVE_CALIBRATIONMODULE_H

#include "module/module.h"
#include "templates/refdatalist.h"

// Forward Declarations
/* none */

// Calibration Module
class CalibrationModule : public Module
{
	/*
	 * Module for testing various functions
	 */

	public:
	// Constructor
	CalibrationModule();
	// Destructor
	~CalibrationModule();


	/*
	 * Instances
	 */
	public:
	// Create instance of this module
	Module* createInstance() const;


	/*
	 * Definition
	 */
	public:
	// Return type of module
	const char* type() const;
	// Return category for module
	const char* category() const;
	// Return brief description of module
	const char* brief() const;
	// Return the maximum number of Configurations the Module can target (or -1 for any number)
	int nTargetableConfigurations() const;


	/*
	 * Options
	 */
	public:
	// IntraBroadening Fitting Targets
	enum IntraBroadeningFitTarget
	{
		IntraBroadeningTargetSQ,
		IntraBroadeningTargetGR,
		IntraBroadeningTargetBoth,
		nIntraBroadeningFitTargets
	};
	protected:
	// Set up options for Module
	void setUpKeywords();
	// Parse complex keyword line, returning true (1) on success, false (0) for recognised but failed, and -1 for not recognised
	KeywordBase::ParseResult parseComplexKeyword(KeywordBase* keyword, LineParser& parser, Dissolve* dissolve, GenericList& targetList, const char* prefix);


	/*
	 * Processing
	 */
	private:
	// Run main processing
	bool process(Dissolve& dissolve, ProcessPool& procPool);


	/*
	 * Members / Functions
	 */
	private:
	// RDF Module targets for IntraBroadening calibration
	RefList<Module> intraBroadeningModules_;
	// NeutronSQ Module targets for IntraBroadening calibration (S(Q) data)
	RefList<Module> intraBroadeningNeutronSQReferences_;
	// NeutronSQ Module targets for IntraBroadening calibration (G(r) data)
	RefList<Module> intraBroadeningNeutronGRReferences_;


	/*
	 * GUI Widget
	 */
	public:
	// Return a new widget controlling this Module
	ModuleWidget* createWidget(QWidget* parent, Dissolve& dissolve);
};

// Interface Class for Complex Cost Functions
class CalibrationModuleCostFunctions
{
	public:
	// Constructor
	CalibrationModuleCostFunctions(Dissolve& dissolve, ProcessPool& procPool, RefList<Module>& intraBroadeningModules, RefDataList<Module,CalibrationModule::IntraBroadeningFitTarget>& intraBroadeningReferences);

	private:
	// Dissolve Main Object
	Dissolve& dissolve_;
	// Target ProcessPool
	ProcessPool& processPool_;
	// RDFModule targets for IntraBroadening fitting
	RefList<Module>& intraBroadeningModules_;
	// NeutronSQModule targets for IntraBroadening fitting
	RefDataList<Module,CalibrationModule::IntraBroadeningFitTarget>& intraBroadeningReferences_;

	public:
	// Cost function for intraBroadening minimisation
	double intraBroadeningCost(const Array<double>& alpha);
};

#endif

