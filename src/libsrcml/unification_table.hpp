// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unification_table.hpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#ifndef SRCQL_UNIFICATION_TABLE_HPP
#define SRCQL_UNIFICATION_TABLE_HPP

#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_map>
#include <functional>
#include <cstdint>

struct unique_element {
    std::string_view token;
    std::uintptr_t address;
};

using token_list = std::vector<unique_element>;
using number_bucket = std::unordered_map<int, token_list>;
using variable_bucket = std::map<std::string, number_bucket, std::less<>>;

class UnificationTable {
public:
    UnificationTable() = default;

    void add_to_variable_bucket(std::string_view);

    size_t size_of_variable_bucket(std::string_view) const;

    bool will_unification_occur() const;

    void add_to_number_bucket(std::string_view, int);

    void add_to_token_list(std::string_view, int, std::string_view, std::uintptr_t);

    bool does_element_match_variable(std::string_view, int, std::string_view first, uintptr_t second) const;

    void empty_buckets();
    void empty_bucket(std::string_view);

    // void clear_storage();

    friend std::ostream& operator<<(std::ostream&, const UnificationTable&);

private:
    variable_bucket bucket;
};

#endif
