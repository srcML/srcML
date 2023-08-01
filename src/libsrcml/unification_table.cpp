// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unification_table.cpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#include "unification_table.hpp"

// Adds a number bucket to the variable bucket if doesn't exist
void UnificationTable::add_to_variable_bucket(std::string_view variable_identifier) {

    // if doesn't exist, add
    bucket.try_emplace(std::string(variable_identifier));

    // Also add to last_used
    last_used.try_emplace(std::string(variable_identifier), 0);
}

// Gets number of number buckets added to a variable bucket
size_t UnificationTable::size_of_variable_bucket(std::string_view variable_identifier) {

    const auto numberBucket = bucket.find(variable_identifier);
    if (numberBucket == bucket.end())
        return 0;

    return numberBucket->second.size();
}

// Returns if any variable bucket has 2 or more number buckets
bool UnificationTable::will_unification_occur() {

    for (const auto& variable : bucket) {
        if (variable.second.size() > 1)
            return true;
    }
    return false;
}

// Adds a token list to a number bucket
void UnificationTable::add_to_number_bucket(std::string_view variable_identifier, int order) {

    const auto numberBucket = bucket.find(variable_identifier);
    if (numberBucket == bucket.end())
        return;

    // Does NOT perform checks to see if token list already exists
    // at the order location
    numberBucket->second.emplace(order, token_list());
}

// Clears a token list in a number bucket
void UnificationTable::clear_token_list(std::string_view variable_identifier, int order) {

    auto numberBucket = bucket.find(variable_identifier);
    if (numberBucket == bucket.end())
        return;

    auto& current = numberBucket->second;
    auto currentOrder = current.find(order);
    if (currentOrder != current.end()) {
        currentOrder->second.clear();
    }
}

// Adds a token-address pair to a token list
void UnificationTable::add_to_token_list(std::string_view variable_identifier, int order, std::string_view token, std::uintptr_t address) {

    // Do NOT insert if element is already in
    auto& bucketOrder = bucket.find(variable_identifier)->second[order];
    for (const auto& element : bucketOrder) {
        if (element.second == address)
            return;
    }
    bucketOrder.emplace_back(token, address);
}

// Loops through the previous order bucket and returns True if:
//     The previous bucket has a unique_element with a matching token
//         and differing address
// OR
//     The intended placement of the element is 1, which makes it
//         automatically valid
bool UnificationTable::does_element_match_variable(std::string_view variable_identifier, int order, std::string_view first, uintptr_t second) {

    // automatically valid
    if (order == 1)
        return true;

    std::set<uintptr_t> chained_addresses{second};
    const auto bucketVar = bucket.find(variable_identifier);
    for (int i = order - 1; i > 0; --i) {
        for (const auto& prev_order_element : bucketVar->second[i]) {

            if (first == prev_order_element.first && chained_addresses.emplace(prev_order_element.second).second) {

                if (chained_addresses.size() > (size_t) order)
                    return false;

                first = prev_order_element.first;
                break;
            }
        }
    }
    return chained_addresses.size() == (size_t) order;
}

// bool UnificationTable::does_element_match_in_order(std::string_view variable_identifier, int order, const unique_element& element) {

//     if (order == 1)
//         return true;

//     int count = 0;
//     for (const auto& prev_order_element : bucket.find(variable_identifier)->second[order - 1]) {

//         if (prev_order_element.first == element.first)
//             ++count;

//         if (prev_order_element.second == element.second)
//             return count >= order;
//     }
//     return true;
// }

int UnificationTable::get_last_used(std::string_view variable_identifier) {
    return last_used.find(variable_identifier)->second;
}

void UnificationTable::set_last_used(std::string_view variable_identifier, int order) {
    last_used.find(variable_identifier)->second = order;
}

void UnificationTable::empty_buckets() {

    for (auto& variable : bucket) {
        for (auto& order : variable.second) {
            order.second.clear();
        }
    }
}

void UnificationTable::empty_bucket(std::string_view variable_identifier) {

    const auto it = bucket.find(variable_identifier);
    if (it == bucket.end()) {
        return;
    }

    for (auto& orderFirst : it->second) {
        orderFirst.second.clear();
    }
}

std::ostream& operator<<(std::ostream& out, const UnificationTable& storage) {
    for(auto const& variable : storage.bucket) {
        out << variable.first << std::endl;
        for(auto const& order : variable.second) {
            out << "\t" << order.first << std::endl;
            for(auto element : order.second) {
                out << "\t\t" << element.first << " | " << element.second << std::endl;
            }
        }
    }
    return out;
}
