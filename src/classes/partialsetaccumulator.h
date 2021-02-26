// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "classes/partialset.h"
#include "templates/array2d.h"

// Accumulator for PartialSet
class PartialSetAccumulator : public GenericItemBase
{
    public:
    PartialSetAccumulator();
    ~PartialSetAccumulator() = default;
    void operator+=(const PartialSet &source);

    /*
     * Partials Data
     */
    private:
    // Number of accumulated points
    int nAccumulated_;
    // Abscissa values
    std::vector<double> abscissa_;
    // Pair matrix, containing sampling of full atom-atom partial
    Array2D<std::vector<SampledDouble>> sampledFullPartials_;
    // Bound matrix, containing sampling of atom-atom partial of bound pairs
    Array2D<std::vector<SampledDouble>> sampledBoundPartials_;
    // Sample of the total function
    std::vector<SampledDouble> total_;
    // Partial set containing the accumulated data
    PartialSet accumulatedSet_;

    public:
    // Accumulate data from supplied PartialSet
    void accumulate(const PartialSet &partialSet);

    /*
     * GenericItemBase Implementations
     */
    public:
    // Return class name
    static std::string_view itemClassName();
    // Read data through specified LineParser
    bool read(LineParser &parser, CoreData &coreData);
    // Write data through specified LineParser
    bool write(LineParser &parser);

    /*
     * Parallel Comms
     */
    public:
    // Broadcast data from root to all other processes
    bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData);
    // Check item equality
    bool equality(ProcessPool &procPool);
};
