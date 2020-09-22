// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "modules/molshake/molshake.h"

/*
 * Constructor / Destructor
 */

MolShakeModule::MolShakeModule() : Module()
{
    // Initialise Module - set up keywords etc.
    initialise();
}

MolShakeModule::~MolShakeModule() {}

/*
 * Instances
 */

// Create instance of this module
Module *MolShakeModule::createInstance() const { return new MolShakeModule; }
