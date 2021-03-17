// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "genericitems/producers.h"
#include "classes/neutronweights.h"
#include "classes/partialset.h"
#include "classes/xrayweights.h"
#include "genericitems/list.h"
#include "math/data1d.h"
#include "math/data2d.h"
#include "math/data3d.h"
#include "math/histogram1d.h"
#include "math/histogram2d.h"
#include "math/histogram3d.h"
#include <ios>

GenericItemSearcher::GenericItemSearcher()
{
    // Custom Classes
    registerSearcher<PartialSet>([](const std::any &a, std::string_view dataName) {
        auto &ps = std::any_cast<const PartialSet &>(a);
        return ps.total();
    });
}

/*
 * Instance
 */

// Return the producer instance
const GenericItemSearcher &GenericItemSearcher::instance()
{
    static GenericItemSearcher _instance;

    return _instance;
}
