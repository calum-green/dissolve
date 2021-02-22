// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "modules/accumulate/gui/modulewidget.h"
#include "modules/accumulate/accumulate.h"

AccumulateModuleWidget::AccumulateModuleWidget(QWidget *parent, AccumulateModule *module) : ModuleWidget(parent), module_(module)
{
    // Set up user interface
    ui_.setupUi(this);

    refreshing_ = false;
}
