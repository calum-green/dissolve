// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "genericitems/container.h"

// GenericItemContainer< Array< Vec3<double> > >
template <> class GenericItemContainer<Array<Vec3<double>>> : public GenericItem
{
    public:
    GenericItemContainer<Array<Vec3<double>>>(std::string_view name, int flags = 0) : GenericItem(name, flags) {}

    /*
     * Data
     */
    private:
    // Data item
    Array<Vec3<double>> data_;

    public:
    // Return data item
    Array<Vec3<double>> &data() { return data_; }
    const Array<Vec3<double>> &data() const { return data_; }

    /*
     * Item Class
     */
    protected:
    // Create a new GenericItem of this type
    std::shared_ptr<GenericItem> produce(std::string_view name, int flags = 0) override
    {
        return std::make_shared<GenericItemContainer<Array<Vec3<double>>>>(name, flags);
    }

    public:
    // Return class name contained in item
    std::string_view itemClassName() override { return "Array<Vec3<double>>"; }

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    bool write(LineParser &parser)
    {
        parser.writeLineF("{}\n", data_.nItems());
        Vec3<double> *array = data_.array();
        for (auto n = 0; n < data_.nItems(); ++n)
        {
            if (!parser.writeLineF("{:16.9e} {:16.9e} {:16.9e}\n", array[n].x, array[n].y, array[n].z))
                return false;
        }
        return true;
    }
    // Read data through specified parser
    bool read(LineParser &parser, CoreData &coreData)
    {
        if (parser.getArgsDelim(LineParser::Defaults) != LineParser::Success)
            return false;
        int nItems = parser.argi(0);
        data_.createEmpty(nItems);
        for (auto n = 0; n < nItems; ++n)
        {
            if (parser.getArgsDelim(LineParser::Defaults) != LineParser::Success)
                return false;
            data_.add(parser.arg3d(0));
        }
        return true;
    }

    /*
     * Parallel Comms
     */
    public:
    // Broadcast item contents
    bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData) { return procPool.broadcast(data_, root); }
    // Return equality between items
    bool equality(ProcessPool &procPool)
    {
        // Verify array size first
        if (!procPool.equality(data_.nItems()))
            return false;
        // Keep it simple (and slow) and check/send one value at a time
        for (auto n = 0; n < data_.nItems(); ++n)
            if (!procPool.equality(data_.at(n)))
                return false;
        return true;
    }
};
