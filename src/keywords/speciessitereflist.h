// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "keywords/data.h"
#include "templates/reflist.h"

// Forward Declarations
class SpeciesSite;

// Keyword with SpeciesSiteReference Data
class SpeciesSiteRefListKeyword : public KeywordData<RefList<SpeciesSite> &>
{
    public:
    SpeciesSiteRefListKeyword(RefList<SpeciesSite> &references, bool axesRequired = false);
    ~SpeciesSiteRefListKeyword();

    /*
     * Specification
     */
    private:
    // Whether sites in the list must have a defined orientation
    bool axesRequired_;

    public:
    // Return whether axes are required for the site
    bool axesRequired() const;

    /*
     * Arguments
     */
    public:
    // Return minimum number of arguments accepted
    int minArguments() const;
    // Return maximum number of arguments accepted
    int maxArguments() const;
    // Parse arguments from supplied LineParser, starting at given argument offset
    bool read(LineParser &parser, int startArg, CoreData &coreData);
    // Write keyword data to specified LineParser
    bool write(LineParser &parser, std::string_view keywordName, std::string_view prefix);

    /*
     * Object Management
     */
    protected:
    // Prune any references to the supplied Species in the contained data
    void removeReferencesTo(Species *sp);
    // Prune any references to the supplied SpeciesSite in the contained data
    void removeReferencesTo(SpeciesSite *spSite);
};
