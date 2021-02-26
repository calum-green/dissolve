// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "classes/partialsetaccumulator.h"
#include "base/lineparser.h"
#include "templates/algorithms.h"

PartialSetAccumulator::PartialSetAccumulator() : nAccumulated_(0) { }

void PartialSetAccumulator::operator+=(const PartialSet &source) { accumulate(source); }

/*
 * Partials Data
 */

// Accumulate data from supplied PartialSet
void PartialSetAccumulator::accumulate(const PartialSet &partialSet)
{
    // If there are no data accumulated yet, initialise our data structures
    if (nAccumulated_ == 0)
    {
        // Copy the PartialSet
        accumulatedSet_ = partialSet;

    }XX
}

/*
 * GenericItemBase Implementations
 */

// Return class name
std::string_view PartialSetAccumulator::itemClassName() { return "PartialSetAccumulator"; }

// Read data through specified LineParser
bool PartialSetAccumulator::read(LineParser &parser, CoreData &coreData)
{
    if (parser.readNextLine(LineParser::Defaults, objectNamePrefix_) != LineParser::Success)
        return false;
    if (parser.readNextLine(LineParser::Defaults, abscissaUnits_) != LineParser::Success)
        return false;
    if (parser.readNextLine(LineParser::Defaults, fingerprint_) != LineParser::Success)
        return false;

    // Read atom types
    atomTypes_.clear();
    if (!atomTypes_.read(parser, coreData))
        return false;
    auto nTypes = atomTypes_.nItems();

    // Read partials
    partials_.initialise(nTypes, nTypes, true);
    boundPartials_.initialise(nTypes, nTypes, true);
    unboundPartials_.initialise(nTypes, nTypes, true);
    emptyBoundPartials_.initialise(nTypes, nTypes, true);
    emptyBoundPartials_ = false;

    for (auto typeI = 0; typeI < nTypes; ++typeI)
    {
        for (auto typeJ = typeI; typeJ < nTypes; ++typeJ)
        {
            if (!partials_[{typeI, typeJ}].read(parser, coreData))
                return false;
            if (!boundPartials_[{typeI, typeJ}].read(parser, coreData))
                return false;
            if (!unboundPartials_[{typeI, typeJ}].read(parser, coreData))
                return false;
        }
    }

    // Read total
    if (!total_.read(parser, coreData))
        return false;

    // Read empty bound flags
    if (!GenericItemContainer<Array2D<char>>::read(emptyBoundPartials_, parser))
        return false;

    return true;
}

// Write data through specified LineParser
bool PartialSetAccumulator::write(LineParser &parser)
{
    // If no data has been accumulated, just write a zero and exit
    if (!parser.writeLineF("{}  # nAccumulated\n", nAccumulated_))
        return false;
    if (nAccumulated_ == 0)
        return true;

    // Write the PartialSet data
    if (!accumulatedSet_.write(parser))
        return false;

    // Write our own data
    XXX

    return true;
}

/*
 * Parallel Comms
 */

// Broadcast data from root to all other processes
bool PartialSetAccumulator::broadcast(ProcessPool &procPool, const int root, const CoreData &coreData)
{
#ifdef PARALLEL
    // The structure should have already been setup(), so arrays should be ready to copy
    for_each_pair_early(0, atomTypes_.nItems(), [&](int typeI, int typeJ) -> EarlyReturn<bool> {
        if (!partials_[{typeI, typeJ}].broadcast(procPool, root, coreData))
            return Messenger::error("Failed to broadcast partials_ array.\n");
        if (!boundPartials_[{typeI, typeJ}].broadcast(procPool, root, coreData))
            return Messenger::error("Failed to broadcast boundPartials_ array.\n");
        if (!unboundPartials_[{typeI, typeJ}].broadcast(procPool, root, coreData))
            return Messenger::error("Failed to broadcast unboundPartials_ array.\n");

        return EarlyReturn<bool>::Continue;
    });

    if (!total_.broadcast(procPool, root, coreData))
        return Messenger::error("Failed to broadcast total_.\n");

    if (!procPool.broadcast(emptyBoundPartials_))
        return Messenger::error("Failed to broadcast emptyBoundPartials_ array.\n");
    if (!procPool.broadcast(objectNamePrefix_))
        return false;
#endif
    return true;
}

// Check item equality
bool PartialSetAccumulator::equality(ProcessPool &procPool)
{
#ifdef PARALLEL
    for_each_pair_early(0, atomTypes_.nItems(), [&](int typeI, int typeJ) -> EarlyReturn<bool> {
        if (!partials_[{typeI, typeJ}].equality(procPool))
            return Messenger::error("PartialSetAccumulator full partial {}-{} is not equivalent.\n", typeI, typeJ);
        if (!boundPartials_[{typeI, typeJ}].equality(procPool))
            return Messenger::error("PartialSetAccumulator bound partial {}-{} is not equivalent.\n", typeI, typeJ);
        if (!unboundPartials_[{typeI, typeJ}].equality(procPool))
            return Messenger::error("PartialSetAccumulator unbound partial {}-{} is not equivalent.\n", typeI, typeJ);

        return EarlyReturn<bool>::Continue;
    });
    if (!total_.equality(procPool))
        return Messenger::error("PartialSetAccumulator total sum is not equivalent.\n");
    if (!procPool.equality(emptyBoundPartials_))
        return Messenger::error("PartialSetAccumulator emptyBoundPartials array is not equivalent.\n");
#endif
    return true;
}
