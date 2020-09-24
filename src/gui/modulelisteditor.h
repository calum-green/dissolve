// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#pragma once

#include "gui/ui_modulelisteditor.h"
#include "templates/refdatalist.h"

// Forward Declarations
class Configuration;
class DissolveWindow;
class LineParser;
class MimeTreeWidgetItem;
class ModuleListChart;
class ModuleLayer;
class ModulePalette;

// Module List Editor
class ModuleListEditor : public QWidget
{
    // All Qt declarations derived from QObject must include this macro
    Q_OBJECT

    public:
    ModuleListEditor(QWidget *parent = nullptr);
    ~ModuleListEditor();

    /*
     * UI
     */
    private:
    // Main form declaration
    Ui::ModuleListEditor ui_;

    private:
    // Whether the widget is currently refreshing
    bool refreshing_;

    public:
    // Update controls
    void updateControls();
    // Disable sensitive controls within widget
    void disableSensitiveControls();
    // Enable sensitive controls within widget
    void enableSensitiveControls();
    // Show / hide module palette
    void setModulePaletteVisible(bool visible);

    /*
     * Setup
     */
    private:
    // Pointer to main window
    DissolveWindow *dissolveWindow_;
    // Target ModuleLayer
    ModuleLayer *moduleLayer_;
    // Parent Configuration (if we are local to one)
    Configuration *localConfiguration_;
    // Module categories and their associated MimeTreeWidgetItems
    RefDataList<MimeTreeWidgetItem, QString> moduleCategories_;

    public:
    // Set up the ModuleListEditor for the specified ModuleLayer
    bool setUp(DissolveWindow *dissolveWindow, ModuleLayer *moduleLayer, Configuration *localConfiguration = nullptr);

    /*
     * Widget Functions
     */
    private:
    // Chart widget being displayed
    ModuleListChart *chartWidget_;

    private slots:
    void blockSelectionChanged(const QString &blockIdentifier);
    void on_AvailableModulesTree_itemDoubleClicked(QTreeWidgetItem *item);
    void chartWidgetDataModified();
    void chartWidgetSizeChanged();
    void controlsWidgetDataModified();

    /*
     * State
     */
    public:
    // Write widget state through specified LineParser
    bool writeState(LineParser &parser) const;
    // Read widget state through specified LineParser
    bool readState(LineParser &parser);
};
