// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "genericitems/list.h"

/*
 * Item Types
 */

// Register item type
void GenericList::registerItemType(std::shared_ptr<GenericItem> itemType) { registeredTypes_.push_back(itemType); }

// Clear all items (except those that are marked protected)
void GenericList::clear()
{
    auto it =
        std::remove_if(items_.begin(), items_.end(), [](auto &item) { return !item->hasFlag(GenericItem::ProtectedFlag); });
    if (it != items_.end())
        items_.erase(it);
}

// Clear all items, including protected items
void GenericList::clearAll() { items_.clear(); }

// Create an item of the specified type
std::shared_ptr<GenericItem> GenericList::create(std::string_view name, std::string_view itemClassName, int version, int flags)
{
    // Check for existing item with this name
    if (find(name))
        throw(std::runtime_error(fmt::format("Item named '{}' already exists in the list - it cannot be duplicated.\n",
                                name)));

    // Create a new item of the specified class type - search through registered item classes list for one matching the class name provided
    for (const auto &item : registeredTypes_)
        if (item->itemClassName() == itemClassName)
        {
            auto newItem = item->produce(name, flags);
            items_.push_back(newItem);

            // Set the item's version
            newItem->setVersion(version);

            return newItem;
        }

    throw(std::runtime_error(fmt::format("GenericList::create() doesn't know how to create an item of type '{}'\n", itemClassName)));
}

// Return whether the named item is contained in the list
bool GenericList::contains(std::string_view name, std::string_view prefix) const
{
    // Construct full name
    std::string varName = prefix.empty() ? std::string(name) : fmt::format("{}_{}", prefix, name);

    return std::find_if(items_.cbegin(), items_.cend(), [varName](auto &item) { return DissolveSys::sameString(item->name(), varName); } ) != items_.end();
}

// Return item list
std::vector<std::shared_ptr<GenericItem>> &GenericList::items() { return items_; }

// Return the named item from the list
std::shared_ptr<GenericItem> GenericList::find(std::string_view name)
{
    auto it = std::find_if(items_.begin(), items_.end(), [name](auto &item) { return DissolveSys::sameString(item->name(), name); } );
    if (it != items_.end())
        return *it;

    return nullptr;
}

const std::shared_ptr<GenericItem> GenericList::find(std::string_view name) const
{
    auto it = std::find_if(items_.cbegin(), items_.cend(), [name](auto &item) { return DissolveSys::sameString(item->name(), name); } );
    if (it != items_.end())
        return *it;

    return nullptr;
}

// Return the named item from the list (with prefix)
std::shared_ptr<GenericItem> GenericList::find(std::string_view name, std::string_view prefix)
{
    // Construct full name
    std::string varName = prefix.empty() ? std::string(name) : fmt::format("{}_{}", prefix, name);

    auto it = std::find_if(items_.cbegin(), items_.cend(), [varName](auto &item) { return DissolveSys::sameString(item->name(), varName); } );
    if (it != items_.end())
        return *it;

    return nullptr;
}

// Return the version of the named item from the list
int GenericList::version(std::string_view name, std::string_view prefix) const
{
    // Construct full name
    std::string varName = prefix.empty() ? std::string(name) : fmt::format("{}_{}", prefix, name);

    auto it = std::find_if(items_.cbegin(), items_.cend(), [varName](auto &item) { return DissolveSys::sameString(item->name(), varName); } );
    if (it != items_.end())
        return it->get()->version();

    return -99;
}

// Remove named item
bool GenericList::remove(std::string_view name, std::string_view prefix)
{
    // Construct full name
    std::string varName = prefix.empty() ? std::string(name) : fmt::format("{}_{}", prefix, name);

    auto it =
        std::remove_if(items_.begin(), items_.end(), [varName](auto &item) { return DissolveSys::sameString(item->name(), varName); } );
    if (it != items_.end())
        return Messenger::error("Couldn't find named item '{}' (prefix = '{}') in the list, so can't remove it.\n", name,
                                prefix);

    items_.erase(it);

    return true;
}

// Rename item
bool GenericList::rename(std::string_view oldName, std::string_view oldPrefix, std::string_view newName,
                         std::string_view newPrefix)
{
    // First, find the named item
    GenericItem *item = find(oldName, oldPrefix);
    if (!item)
    {
        Messenger::printVerbose("Couldn't find named item '{}' (prefix = '{}') in the list, so can't rename it.\n", oldName,
                                oldPrefix);
        return false;
    }

    if (newPrefix.empty())
        item->setName(newName);
    else
        item->setName(fmt::format("{}_{}", newPrefix, newName));

    return true;
}

// Prune all items with '@suffix'
void GenericList::pruneWithSuffix(std::string_view suffix)
{
    GenericItem *nextItem = nullptr;
    GenericItem *item = items_.first();
    while (item != nullptr)
    {
        if (DissolveSys::endsWith(item->name(), suffix))
        {
            nextItem = item->next();
            items_.remove(item);
            item = nextItem;
        }
        else
            item = item->next();
    }
}

/*
 * Parallel Comms
 */

// Broadcast all data
bool GenericList::broadcast(ProcessPool &procPool, const int root, const CoreData &coreData)
{
    for (auto *item = items_.first(); item != nullptr; item = item->next())
    {
        Messenger::printVerbose("Broadcasting data '{}' ({})...\n", item->name(), item->itemClassName());
        if (!item->broadcast(procPool, root, coreData))
            return false;
    }
    return true;
}

// Check equality of all data
bool GenericList::equality(ProcessPool &procPool)
{
    // Loop over processes - the target process will loop over its module data, sending out the name to the slaves.
    // If found, we note that we've already done this data, check it for equality, and move on.
    // If we can't find the data, we'll complain but move on.
    RefList<GenericItem> checkedItems;
    std::string itemName, itemClassName;
    auto nFailed = 0;
    for (auto n = 0; n < procPool.nProcesses(); ++n)
    {
        // The master process - rank 'n' - will control the loop
        if (procPool.poolRank() == n)
        {
            // Loop over GenericItems in list
            for (auto *item = items_.first(); item != nullptr; item = item->next())
            {
                // If we have already checked this item, move on...
                if (checkedItems.contains(item))
                    continue;

                // Found an item that hasn't already been checked, so flag to the slaves that we will be sending
                // another name to check
                if (!procPool.decideTrue(n))
                    return false;

                Messenger::printVerbose("Checking equality of item '{}'...\n", item->name());

                // Send name of item - slaves will try to find it, so we need to check that everybody does...
                itemName = item->name();
                if (!procPool.broadcast(itemName, n))
                    return false;
                if (!procPool.allTrue(true))
                {
                    Messenger::error("GenericList equality check failed - item '{}' is not present on all "
                                     "processes.\n",
                                     item->name());
                    ++nFailed;
                    continue;
                }

                // Send class name of item - slaves will check it, and everybody must agree
                itemClassName = item->itemClassName();
                if (!procPool.broadcast(itemClassName, n))
                    return false;
                if (!procPool.allTrue(true))
                {
                    Messenger::error("GenericList equality check failed - item '{}' is not of the same "
                                     "type ({}) on all processes.\n",
                                     item->name(), item->itemClassName());
                    ++nFailed;
                    continue;
                }

                // Check the item for equality
                if (!item->equality(procPool))
                    return Messenger::error("Failed equality check for module data '{}'.\n", item->name());

                // Add the item to our checked list
                checkedItems.append(item);
            }

            // No more items, so send the 'false' signal
            if (!procPool.decideFalse(n))
                return false;
        }
        else
            while (procPool.decision(n))
            {
                // Receive the name of the item we're targetting
                if (!procPool.broadcast(itemName, n))
                    return false;

                Messenger::printVerbose("Checking equality of item '{}'...\n", itemName);

                // Do we have an item of this name?
                GenericItem *item = find(itemName);
                if (!procPool.allTrue(item))
                {
                    Messenger::error("GenericList equality check failed - item '{}' not found on process {}.\n", item->name(),
                                     procPool.poolRank());
                    ++nFailed;
                    continue;
                }

                // Receive the class name of the item and check it against ours
                if (!procPool.broadcast(itemClassName, n))
                    return false;
                if (!procPool.allTrue(DissolveSys::sameString(item->itemClassName(), itemClassName)))
                {
                    Messenger::error("GenericList equality check failed - item '{}' not of the correct "
                                     "type (is {}) on process {}.\n",
                                     item->name(), item->itemClassName(), procPool.poolRank());
                    ++nFailed;
                    continue;
                }

                // Check the item for equality
                if (!item->equality(procPool))
                    return Messenger::error("Failed equality check for module data '{}'.\n", item->name());

                // Add the item to our checked list
                checkedItems.append(item);
            }
    }

    // Did we succeed?
    return (nFailed == 0);
}
