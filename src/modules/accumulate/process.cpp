// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "base/sysfunc.h"
#include "main/dissolve.h"
#include "modules/accumulate/accumulate.h"

// Return EnumOptions for TargetPartialSet
EnumOptions<AccumulateModule::TargetPartialSet> AccumulateModule::targetPartialSet()
{
    static EnumOptionsList TargetPartialSetOptions = EnumOptionsList()
        << EnumOption(TargetPartialSet::GR, "GR")
        << EnumOption(TargetPartialSet::SQ, "SQ")
        << EnumOption(TargetPartialSet::OriginalGR, "OriginalGR");

    static EnumOptions<AccumulateModule::TargetPartialSet> options("TargetPartialSet", TargetPartialSetOptions);

    return options;
}

// Run main processing
bool AccumulateModule::process(Dissolve &dissolve, ProcessPool &procPool)
{
    // Get keyword data
    const auto &targets = keywords_.retrieve<std::vector<Module *>>("Target");
    if (targets.empty())
        return Messenger::error("No target module set.");
    const auto targetData = keywords_.enumeration<AccumulateModule::TargetPartialSet>("SetType");

    // Get the module and decide on the PartialSet data name we're looking for
    const auto *targetModule = targets.front();
    std::string moduleName = "";
    switch (targetData)
    {
        case (AccumulateModule::GR):
            if (targetModule->type() == "SQ")
                break;
            else if (targetModule->type() == "RDF")
                moduleName = fmt::format("UnweightedGR_{}", targetModule->uniqueName());
            else
                moduleName = fmt::format("WeightedGR_{}", targetModule->uniqueName());
            break;
        case (AccumulateModule::SQ):
            if (targetModule->type() == "SQ")
                moduleName = fmt::format("UnweightedSQ_{}", targetModule->uniqueName());
            else if (targetModule->type() == "RDF")
                break;
            else
                moduleName = fmt::format("WeightedSQ_{}", targetModule->uniqueName());
            break;
        case (AccumulateModule::OriginalGR):
            if (targetModule->type() == "RDF")
                moduleName = fmt::format("OriginalGR_{}", targetModule->uniqueName());
            break;
        default:
            throw(std::runtime_error(fmt::format("Unhandled partialset accumulation data type {}.\n", targetData)));
    }
    if (moduleName.empty())
        return Messenger::error("This data type ('{}') is not valid for a module of type '{}'.\n", targetPartialSet().keyword(targetData), targetModule->type());


    return true;
}
