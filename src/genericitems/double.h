// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "genericitems/container.h"

// GenericItemContainer<double>
template <> class GenericItemContainer<double> : public GenericItem
{
    public:
    GenericItemContainer<double>(std::string_view name, int flags = 0) : GenericItem(name, flags) {}

    /*
     * Data
     */
    private:
    // Data item
    double data_;

    public:
    // Return data item
    double &data() { return data_; }
    const double &data() const { return data_; }

    /*
     * Item Class
     */
    protected:
    // Create a new GenericItem of this type
    std::shared_ptr<GenericItem> produce(std::string_view name, int flags = 0) override
    {
        return std::make_shared<GenericItemContainer<double>>(name, flags);
    }

    public:
    // Return class name contained in item
    std::string_view itemClassName() override { return "double"; }

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    bool write(LineParser &parser) { return parser.writeLineF("{:16.9e}\n", data_); }
    // Read data through specified parser
    bool read(LineParser &parser, CoreData &coreData)
    {
        if (parser.getArgsDelim(LineParser::Defaults) != LineParser::Success)
            return false;
        data_ = parser.argd(0);
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
