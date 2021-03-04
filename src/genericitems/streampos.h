// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "genericitems/container.h"
#include <ios>

// GenericItemContainer<streampos>
template <> class GenericItemContainer<std::streampos> : public GenericItem
{
    public:
    GenericItemContainer<std::streampos>(std::string_view name, int flags = 0) : GenericItem(name, flags) {}

    /*
     * Data
     */
    private:
    // Data item
    std::streampos data_;

    public:
    // Return data item
    std::streampos &data() { return data_; }
    const std::streampos &data() const { return data_; }

    /*
     * Item Class
     */
    protected:
    // Create a new GenericItem of this type
    std::shared_ptr<GenericItem> produce(std::string_view name, int flags = 0) override
    {
        return std::make_shared<GenericItemContainer<std::streampos>>(name, flags);
    }

    public:
    // Return class name contained in item
    std::string_view itemClassName() override { return "streampos"; }

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    bool write(LineParser &parser) { return parser.writeArg(data_); }
    // Read data through specified parser
    bool read(LineParser &parser, CoreData &coreData)
    {
        // NOTE Can't implicit cast streampos into the arg for readArg(), so assume long long int for now.
        long long int pos;
        if (!parser.readArg(pos))
            return false;
        data_ = pos;

        return true;
    }

    /*
     * Parallel Comms
     */
    public:
    // Broadcast item contents
    bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData)
    {
        long int pos = (long int)data_;
        return procPool.broadcast(pos, root);
        data_ = (std::streampos)pos;
    }
    // Check item equality
    bool equality(ProcessPool &procPool) { return procPool.equality((long int)data_); }
};
