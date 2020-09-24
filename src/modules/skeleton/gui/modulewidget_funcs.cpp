// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "modules/skeleton/gui/modulewidget.h"
#include "modules/skeleton/skeleton.h"

SkeletonModuleWidget::SkeletonModuleWidget(QWidget *parent, SkeletonModule *module) : ModuleWidget(parent), module_(module)
{
    // Set up user interface
    ui_.setupUi(this);

    refreshing_ = false;
}
