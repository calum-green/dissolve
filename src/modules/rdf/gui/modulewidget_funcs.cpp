// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "classes/atomtype.h"
#include "classes/isotopedata.h"
#include "gui/dataviewer.hui"
#include "gui/helpers/comboboxupdater.h"
#include "gui/render/renderabledata1d.h"
#include "gui/widgets/mimetreewidgetitem.h"
#include "main/dissolve.h"
#include "modules/rdf/gui/modulewidget.h"
#include "modules/rdf/rdf.h"
#include "templates/algorithms.h"
#include "templates/variantpointer.h"

Q_DECLARE_METATYPE(Configuration *);

RDFModuleWidget::RDFModuleWidget(QWidget *parent, const GenericList &processingData, RDFModule *module, Dissolve &dissolve)
    : ModuleWidget(parent, processingData), module_(module), dissolve_(dissolve)
{
    // Set up user interface
    ui_.setupUi(this);

    // Set up RDF graph
    rdfGraph_ = ui_.RDFPlotWidget->dataViewer();
    // -- Set view
    rdfGraph_->view().setViewType(View::FlatXYView);
    rdfGraph_->view().axes().setTitle(0, "\\it{r}, \\sym{angstrom}");
    rdfGraph_->view().axes().setMax(0, 10.0);
    rdfGraph_->view().axes().setTitle(1, "g(r)");
    rdfGraph_->view().axes().setMin(1, -1.0);
    rdfGraph_->view().axes().setMax(1, 1.0);
    rdfGraph_->groupManager().setVerticalShiftAmount(RenderableGroupManager::TwoVerticalShift);
    rdfGraph_->view().setAutoFollowType(View::AllAutoFollow);
    // -- Set group styling
    rdfGraph_->groupManager().setGroupColouring("Full", RenderableGroup::AutomaticIndividualColouring);
    rdfGraph_->groupManager().setGroupVerticalShifting("Full", RenderableGroup::IndividualVerticalShifting);
    rdfGraph_->groupManager().setGroupColouring("Bound", RenderableGroup::AutomaticIndividualColouring);
    rdfGraph_->groupManager().setGroupVerticalShifting("Bound", RenderableGroup::IndividualVerticalShifting);
    rdfGraph_->groupManager().setGroupStipple("Bound", LineStipple::HalfDashStipple);
    rdfGraph_->groupManager().setGroupColouring("Unbound", RenderableGroup::AutomaticIndividualColouring);
    rdfGraph_->groupManager().setGroupVerticalShifting("Unbound", RenderableGroup::IndividualVerticalShifting);
    rdfGraph_->groupManager().setGroupStipple("Unbound", LineStipple::DotStipple);

    // Set up total G(r) graph
//    totalsGraph_->groupManager().setVerticalShiftAmount(RenderableGroupManager::OneVerticalShift);
    // -- Set group styling
//    totalsGraph_->groupManager().setGroupColouring("Calc", RenderableGroup::AutomaticIndividualColouring);

    refreshing_ = false;

    updateControls();

    setGraphDataTargets(module_);
}

RDFModuleWidget::~RDFModuleWidget() {}

/*
 * UI
 */

// Update controls within widget
void RDFModuleWidget::updateControls(int flags)
{
    // CHANGE THIS!
    ui_.RDFPlotWidget->clearRenderableData();

    // Check / update partial set (Configuration) targets
    auto optConfig = combo_box_updater(ui_.PartialsTargetCombo, module_->targetConfigurations().begin(),
                                       module_->targetConfigurations().end(), [](auto item) { return item.item()->name(); });
    if (optConfig == module_->targetConfigurations().end())
        return;

    // Construct the first part of the object name to search for
    auto targetPrefix = fmt::format("{}//UnweightedGR", optConfig.item()->niceName());

    auto &selectedSet = processingData_.value<PartialSet>(targetPrefix, module_->uniqueName());
    printf("NATOMTYPES = %i\n", selectedSet.nAtomTypes());
    if (selectedSet.nAtomTypes() == 0)
    {
        validData_ = false;
        return;
    }
    else
        validData_ = true;


    //rdfGraph_->createRenderable<RenderableData1D>(fmt::format("{}//UnweightedGR//{}//Full", prefix
    for_each_pair(selectedSet.atomTypes().begin(), selectedSet.atomTypes().end(), [&](int n, auto at1, int m, auto at2) {
      const std::string id = fmt::format("{}-{}", at1.atomTypeName(), at2.atomTypeName());

      // Full partial
      rdfGraph_->createRenderable<RenderableData1D>(fmt::format("{}//{}//Full", targetPrefix, id),
                                                                 fmt::format("{} (Full)", id), "Full");

      // Bound partial
//      rdfGraph_->createRenderable<RenderableData1D, Data1D>(&selectedSet.boundPartial(n, m),
//                                                                 fmt::format("{} (Bound)", id), "Bound");

      // Unbound partial
//      rdfGraph_->createRenderable<RenderableData1D, Data1D>(&selectedSet.unboundPartial(n, m),
//                                                                 fmt::format("{} (Unbound)", id), "Unbound");
    });

    ui_.RDFPlotWidget->updateToolbar();
    rdfGraph_->postRedisplay();
}

/*
 * Widgets / Functions
 */

// Set data targets in graphs
void RDFModuleWidget::setGraphDataTargets(RDFModule *module)
{
    if (!module)
        return;



//    // Loop over Configurations and add total G(R)
//    for (Configuration *cfg : module->targetConfigurations())
//    {
//        // Add calculated total G(r)
//        totalsGraph_->createRenderable<RenderableData1D>(
//            fmt::format("{}//{}//UnweightedGR//Total", cfg->niceName(), module_->uniqueName()), cfg->niceName(), "Calc");
//    }
}

void RDFModuleWidget::on_SummedPartialsButton_triggered(bool checked)
{
    if (!checked)
        return;
}

void RDFModuleWidget::on_TotalsButton_triggered(bool checked)
{
    if (!checked)
    return;
}
void RDFModuleWidget::on_IndividualPartialsButton_triggered(bool checked)
{
    if (!checked)
        return;
}

void RDFModuleWidget::on_PartialsTargetCombo_currentIndexChanged(int index)
{
    // Remove any current data
    rdfGraph_->clearRenderables();

    // Get target data
//    std::string targetName;
//    currentConfiguration_ = VariantPointer<Configuration>(ui_.TargetCombo->itemData(index));
//    if (currentConfiguration_)
//        targetName = fmt::format("{}//UnweightedGR", currentConfiguration_->niceName());
//    else
//        targetName = "UnweightedGR";

    updateControls();
}
