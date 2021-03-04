// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "genericitems/container.h"

// GenericItemContainer<int>
template <> class GenericItemContainer<int> : public GenericItem
{
    public:
    GenericItemContainer<int>(std::string_view name, int flags = 0) : GenericItem(name, flags) {}

    /*
     * Data
     */
    private:
    // Data item
    int data_;

    public:
    // Return data item
    int &data() { return data_; }
    const int &data() const { return data_; }

    /*
     * Item Class
     */
    protected:
    // Create a new GenericItem of this type
    std::shared_ptr<GenericItem> produce(std::string_view name, int flags = 0) override
    {
        return std::make_shared<GenericItemContainer<int>>(name, flags);
    }

    public:
    // Return class name contained in item
    std::string_view itemClassName() override { return "int"; }

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    bool write(LineParser &parser) { return parser.writeLineF("{}\n", data_); }
    // Read data through specified parser
    bool read(LineParser &parser, CoreData &coreData)
    {
        if (parser.getArgsDelim(LineParser::Defaults) != LineParser::Success)
            return false;
        data_ = parser.argi(0);
        return true;
    }

    /*
     * Parallel Comms
     */
    public:
    // Broadcast item contents
    bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData) { return procPool.broadcast(data_, root); }
    // Check item equality
    bool equality(ProcessPool &procPool) { return procPool.equality(data_); }
};
