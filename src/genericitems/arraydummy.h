// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "base/dummyclass.h"
#include "genericitems/container.h"

// GenericItemContainer< Array<DummyClass> >
template <> class GenericItemContainer<Array<DummyClass>> : public GenericItem
{
    public:
    GenericItemContainer<Array<DummyClass>>(std::string_view name, int flags = 0) : GenericItem(name, flags) {}

    /*
     * Item Class
     */
    protected:
    // Create a new GenericItem containing same class as current type
    GenericItem *createItem(std::string_view className, std::string_view name, int flags = 0)
    {
        if (DissolveSys::sameString(className, "Array<Data1D>"))
            return new GenericItemContainer<Array<Data1D>>(name, flags);
        return nullptr;
    }

    public:
    // Return class name contained in item
    std::string_view itemClassName() { return "Array<DummyClass>"; }

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    bool write(LineParser &parser) { return false; }
    // Read data through specified parser
    bool read(LineParser &parser, const CoreData &coreData) { return false; }

    /*
     * Parallel Comms
     */
    public:
    // Broadcast item contents
    bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData) { return false; }
    // Return equality between items
    bool equality(ProcessPool &procPool) { return false; }
};
