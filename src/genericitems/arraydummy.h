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
    // Create a new GenericItem of this type
    std::shared_ptr<GenericItem> produce(std::string_view name, int flags = 0) override
    {
        return std::make_shared<GenericItemContainer<Array<Data1D>>>(name, flags);
    }

    public:
    // Return class name contained in item
    std::string_view itemClassName() override { return "Array<DummyClass>"; }

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    bool write(LineParser &parser) { return false; }
    // Read data through specified parser
    bool read(LineParser &parser, CoreData &coreData) { return false; }

    /*
     * Parallel Comms
     */
    public:
    // Broadcast item contents
    bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData) { return false; }
    // Return equality between items
    bool equality(ProcessPool &procPool) { return false; }
};
