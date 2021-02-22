// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "gui/modulewidget.h"
#include "modules/accumulate/gui/ui_modulewidget.h"

// Forward Declarations
class AccumulateModule;

// Module Widget
class AccumulateModuleWidget : public ModuleWidget
{
    // All Qt declarations derived from QObject must include this macro
    Q_OBJECT

    private:
    // Associated Module
    AccumulateModule *module_;

    public:
    AccumulateModuleWidget(QWidget *parent, AccumulateModule *module);

    /*
     * UI
     */
    private:
    // Main form declaration
    Ui::AccumulateModuleWidget ui_;
};
