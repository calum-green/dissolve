// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "modules/energy/energy.h"
#include "modules/energy/gui/modulewidget.h"

// Return a new widget controlling this Module
ModuleWidget *EnergyModule::createWidget(QWidget *parent, Dissolve &dissolve) { return new EnergyModuleWidget(parent, this); }
