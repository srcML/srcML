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
}

// Gets number of number buckets added to a variable bucket
size_t UnificationTable::size_of_variable_bucket(std::string_view variable_identifier) const {

    const auto numberBucket = bucket.find(variable_identifier);
    if (numberBucket == bucket.end())
        return 0;

    return numberBucket->second.size();
}

// Returns if any variable bucket has 2 or more number buckets
bool UnificationTable::will_unification_occur() const {

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

// Adds a token-address pair to a token list
void UnificationTable::add_to_token_list(std::string_view variable_identifier, int order, std::string_view token, std::uintptr_t address) {

    // Do NOT insert if element is already in
    auto& bucketOrder = bucket.find(variable_identifier)->second[order];
    for (const auto& element : bucketOrder) {
        if (element.address == address)
            return;
    }

    // add the unique element
    unique_element added{ token, address };
    bucketOrder.push_back(std::move(added));
}

// Loops through the previous order bucket and returns True if:
//     The previous bucket has a unique_element with a matching token
//         and differing address
// OR
//     The intended placement of the element is 1, which makes it
//         automatically valid
bool UnificationTable::does_element_match_variable(std::string_view variable_identifier, int order, std::string_view token, uintptr_t address) const {

    // automatically valid
    if (order == 1)
        return true;

    std::set<uintptr_t> chained_addresses{address};
    const auto bucketVar = bucket.find(variable_identifier);
    for (int i = order - 1; i > 0; --i) {
        bool inserted = false;
        for (const auto& prev_order_element : bucketVar->second.find(i)->second) {

            if (token.data() == prev_order_element.token.data() && chained_addresses.emplace(prev_order_element.address).second) {
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            return false;
        }
    }

    return true;
}

bool UnificationTable::is_element_in_bucket(std::string_view variable_identifier, int order, std::string_view t, uintptr_t address) const {
    std::string token(t);
    const auto bucketVar = bucket.find(variable_identifier);
    for (int i = size_of_variable_bucket(variable_identifier); i > 0; --i) {
        for (const auto& element : bucketVar->second.find(i)->second) {
            if (token == element.token && address == element.address) {
                return true;
            }
        }
    }
    return false;
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

void UnificationTable::add_regex_rule(std::string_view variable_identifier, std::string_view regex_string) {
    std::string identifier = std::string(variable_identifier);
    std::string rule(regex_string);
    if (regex_rules.find(identifier) == regex_rules.end()) {
        regex_rules.insert(std::make_pair(identifier, std::set<std::string>()));
    }
    auto& regex_list = regex_rules.find(identifier)->second;
    regex_list.insert(rule);
}

bool UnificationTable::check_regex(std::string_view variable_identifier, std::string_view t) {
    std::string identifier(variable_identifier);
    std::string token(t);
    if (regex_rules.find(identifier) == regex_rules.end()) {
        return true;
    }
    for (const auto& rule : regex_rules.find(identifier)->second) {
        if (!std::regex_match(token,std::regex(rule))) {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const UnificationTable& storage) {
    for(auto const& variable : storage.bucket) {
        out << variable.first << std::endl;
        for(auto const& order : variable.second) {
            out << "\t" << order.first << std::endl;
            for(auto element : order.second) {
                out << "\t\t" << element.token << " | " << element.address << std::endl;
            }
        }
    }
    return out;
}
