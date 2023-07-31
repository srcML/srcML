// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file unification_table.hpp
 *
 * @copyright Copyright (C) 2023 srcML, LLC. (www.srcML.org)
 */

#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_map>
#include <functional>

#ifndef SRCQL_UNIFICATION_TABLE_HPP
#define SRCQL_UNIFICATION_TABLE_HPP

typedef std::pair<std::string, std::uintptr_t> unique_element;
typedef std::vector<unique_element> token_list;
typedef std::unordered_map<int, token_list> number_bucket;
typedef std::map<std::string, number_bucket, std::less<>> variable_bucket;
typedef std::map<std::string, int, std::less<>> last_bucket;

class UnificationTable {
public:
    UnificationTable() = default;

    void add_to_variable_bucket(std::string_view);

    size_t size_of_variable_bucket(std::string_view);

    bool will_unification_occur();

    void add_to_number_bucket(std::string_view, int);

    void clear_token_list(std::string_view, int);

    void add_to_token_list(std::string_view, int, std::string_view, std::uintptr_t);

    bool does_element_match_variable(std::string_view, int, std::string_view first, uintptr_t second);
    // bool does_element_match_in_order(std::string_view, int, const unique_element&);

    int get_last_used(std::string_view);
    void set_last_used(std::string_view, int);

    void empty_buckets();
    void empty_bucket(std::string_view);

    // void clear_storage();

    friend std::ostream& operator<<(std::ostream&, const UnificationTable&);

private:
    variable_bucket bucket;
    last_bucket last_used;
};

#endif
