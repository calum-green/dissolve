// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "modules/analyse/analyse.h"
#include "modules/analyse/gui/modulewidget.h"

AnalyseModuleWidget::AnalyseModuleWidget(QWidget *parent, AnalyseModule *module, const CoreData &coreData)
    : ModuleWidget(parent), module_(module)
{
    // Set up user interface
    ui_.setupUi(this);

    // Set Procedure target
    ui_.EditorWidget->setUp(&module_->analyser(), coreData);

    refreshing_ = false;
}
