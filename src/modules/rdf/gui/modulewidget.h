// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "gui/helpers/comboboxcontroller.h"
#include "gui/modulewidget.h"
#include "modules/rdf/gui/ui_modulewidget.h"

// Forward Declarations
class Configuration;
class Dissolve;
class Module;
class RDFModule;
class PartialSet;
class DataViewer;

// Module Widget
class RDFModuleWidget : public ModuleWidget
{
    // All Qt declarations derived from QObject must include this macro
    Q_OBJECT

    public:
    RDFModuleWidget(QWidget *parent, const GenericList &processingData, RDFModule *module, Dissolve &dissolve);
    ~RDFModuleWidget();

    private:
    // Associated Module
    RDFModule *module_;
    // Whether valid data is being displayed
    bool validData_;
    // DataViewers contained within this widget
    DataViewer *rdfGraph_;
    // Reference to Dissolve
    Dissolve &dissolve_;

    /*
     * UI
     */
    private:
    // Main form declaration
    Ui::RDFModuleWidget ui_;

    public:
    // Update controls within widget
    void updateControls(int flags = ModuleWidget::DefaultUpdateFlag);

    /*
     * Widgets / Functions
     */
    private:
    // Set data targets in graphs
    void setGraphDataTargets(RDFModule *module);

    private slots:
    void on_SummedPartialsButton_triggered(bool checked);
    void on_TotalsButton_triggered(bool checked);
    void on_IndividualPartialsButton_triggered(bool checked);
    void on_PartialsTargetCombo_currentIndexChanged(int index);
};
