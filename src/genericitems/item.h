// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "base/processpool.h"
#include "base/sysfunc.h"

// Forward Declarations
class CoreData;

/*
 * GenericItem - Base class for all GenericItemContainers
 */
class GenericItem
{
    public:
    GenericItem(std::string_view name, int flags);
    virtual ~GenericItem() = default;
    // Item Flags
    enum ItemFlag
    {
        NoFlag = 0,                   /* No flag */
        InRestartFileFlag = 1,        /* The item should be written to the restart file */
        IsReferencePointDataFlag = 2, /* The item was loaded as reference point data */
        ProtectedFlag = 4             /* The item will not be clear()'d unless forced */
    };

    /*
     * Item Class
     */
    public:
    // Create a new GenericItem of this type
    virtual std::shared_ptr<GenericItem> produce(std::string_view name, int flags = 0) = 0;
    // Return class name contained in item
    virtual std::string_view itemClassName() = 0;

    /*
     * Item Contents
     */
    private:
    // Name associated to the item
    std::string name_;
    // Version of the item
    int version_;
    // Flags
    int flags_;

    public:
    // Set item name
    void setName(std::string_view name);
    // Return item name
    std::string_view name() const;
    // Return version of the item
    int version() const;
    // Set version of the item
    void setVersion(int version);
    // Bump the version of the item
    void bumpVersion();
    // Set flags for item, overwriting any existing flags
    void setFlags(int flags);
    // Add (set) flag for item
    void addFlag(ItemFlag flag);
    // Remove (unset) flag for item
    void removeFlag(ItemFlag flag);
    // Return flags
    int flags() const;
    // Return whether specified flag is set
    bool hasFlag(GenericItem::ItemFlag flag) const;

    /*
     * I/O
     */
    public:
    // Write data through specified parser
    virtual bool write(LineParser &parser) = 0;
    // Read data through specified parser
    virtual bool read(LineParser &parser, CoreData &coreData) = 0;

    /*
     * Parallel Comms
     */
    public:
    // Broadcast item contents
    virtual bool broadcast(ProcessPool &procPool, const int root, const CoreData &coreData) = 0;
    // Return equality between items
    virtual bool equality(ProcessPool &procPool) = 0;
};
