// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "genericitems/item.h"
#include "templates/optionalref.h"
#include <any>
#include <functional>
#include <typeindex>
#include <unordered_map>

// GenericItem Searcher
class GenericItemSearcher
{
    private:
    GenericItemSearcher();

    public:
    GenericItemSearcher(const GenericItemSearcher &) = delete;
    GenericItemSearcher &operator=(const GenericItemSearcher &) = delete;

    /*
     * Searchers
     */
    private:
    // Searcher function type
    using SearcherFunction = std::function<std::any(const std::any &a, std::string_view dataName)>;
    // Searchers for data types
    std::unordered_map<std::type_index, SearcherFunction> searchers_;

    private:
    // Register searcher for specific class
    template <class T> void registerSearcher(SearcherFunction func) { searchers_[typeid(T)] = std::move(func); };
    // Search for data of templated type
    template <class T> std::any searchObject(const std::any &a, std::string_view dataName) const
    {
        // Find a suitable searcher and call it
        auto it = searchers_.find(a.type());
        if (it == searchers_.end())
            return {};

        return (it->second)(a, dataName);
    }

    /*
     * Instance
     */
    private:
    // Return the searcher instance
    static const GenericItemSearcher &instance();

    /*
     * Static Functions
     */
    public:
    // Search for data item of templated type in supplied object
    template <class T> static OptionalReferenceWrapper<const T> search(const std::any &a, std::string_view dataName)
    {
        auto obj = instance().searchObject<T>(a, dataName);
        if (obj.has_value())
        {
            // Check type before we attempt to cast it
            if (obj.type() != typeid(T))
                throw(std::runtime_error(fmt::format("GenericItemSearcher::search() - Data item named '{}' found, but has a different type to that requested ('{}' vs '{}').\n",
                                                     dataName,
                                                     obj.type().name(),
                                                     typeid(T).name())));
        }

        return {};
    }
};
