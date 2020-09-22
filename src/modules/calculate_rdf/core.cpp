// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "modules/calculate_rdf/rdf.h"

/*
 * Constructor / Destructor
 */

CalculateRDFModule::CalculateRDFModule() : Module(), analyser_(ProcedureNode::AnalysisContext)
{
    // Set unique name for this instance of the Module
    static int instanceId = 0;
    uniqueName_ = fmt::format("{}{:02d}", type(), instanceId++);

    // Initialise Module - set up keywords etc.
    initialise();
}

CalculateRDFModule::~CalculateRDFModule() {}

/*
 * Instances
 */

// Create instance of this module
Module *CalculateRDFModule::createInstance() const { return new CalculateRDFModule; }
