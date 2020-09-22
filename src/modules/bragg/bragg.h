// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#pragma once

#include "classes/braggreflection.h"
#include "classes/partialset.h"
#include "math/broadeningfunction.h"
#include "module/module.h"

// Forward Declarations
class PartialSet;

// Bragg Module
class BraggModule : public Module
{
    public:
    BraggModule();
    ~BraggModule();

    /*
     * Instances
     */
    public:
    // Create instance of this module
    Module *createInstance() const;

    /*
     * Definition
     */
    public:
    // Return type of module
    std::string_view type() const;
    // Return category for module
    std::string_view category() const;
    // Return brief description of module
    std::string_view brief() const;
    // Return the number of Configuration targets this Module requires
    int nRequiredTargets() const;

    /*
     * Initialisation
     */
    protected:
    // Perform any necessary initialisation for the Module
    void initialise();

    /*
     * Processing
     */
    private:
    // Run main processing
    bool process(Dissolve &dissolve, ProcessPool &procPool);

    /*
     * Members / Functions
     */
    public:
    // Calculate Bragg terms for specified Configuration
    bool calculateBraggTerms(ProcessPool &procPool, Configuration *cfg, const double qMin, const double qDelta,
                             const double qMax, Vec3<int> multiplicity, bool &alreadyUpToDate);
    // Form partial and total reflection functions from calculated reflection data
    bool formReflectionFunctions(ProcessPool &procPool, Configuration *cfg, const double qMin, const double qDelta,
                                 const double qMax);
    // Re-bin reflection data into supplied arrays
    static bool reBinReflections(ProcessPool &procPool, Configuration *cfg, Array2D<Data1D> &braggPartials);

    /*
     * GUI Widget
     */
    public:
    // Return a new widget controlling this Module
    ModuleWidget *createWidget(QWidget *parent, Dissolve &dissolve);
};
