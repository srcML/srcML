// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file JavaHashtable.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Emulation of java.util.Hashtable with String keys. ANTLR enumerates some
 * hashtables while generating code, so the bucket layout, rehashing, and
 * enumeration order of the Java implementation are reproduced exactly.
 */

#ifndef ANTLR2CPP_JAVAHASHTABLE_HPP
#define ANTLR2CPP_JAVAHASHTABLE_HPP

#include "Util.hpp"
#include <vector>
#include <string>
#include <memory>
#include <utility>

namespace antlrcpp {

template <typename V>
class JavaHashtable {
public:
    explicit JavaHashtable(int initialCapacity = 11) {
        if (initialCapacity == 0)
            initialCapacity = 1;
        table.resize(static_cast<std::size_t>(initialCapacity));
        threshold = static_cast<int>(initialCapacity * 0.75f);
    }

    JavaHashtable(const JavaHashtable& other) {
        copyFrom(other);
    }

    JavaHashtable& operator=(const JavaHashtable& other) {
        if (this != &other)
            copyFrom(other);
        return *this;
    }

    // previous value is replaced, as with java.util.Hashtable.put()
    void put(const std::string& key, const V& value) {
        std::int32_t hash = javaHashCode(key);
        int index = indexFor(hash, static_cast<int>(table.size()));
        for (Entry* e = table[static_cast<std::size_t>(index)].get(); e != nullptr; e = e->next.get()) {
            if (e->hash == hash && e->key == key) {
                e->value = value;
                return;
            }
        }
        if (count >= threshold) {
            rehash();
            index = indexFor(hash, static_cast<int>(table.size()));
        }
        auto e = std::make_unique<Entry>();
        e->hash = hash;
        e->key = key;
        e->value = value;
        e->next = std::move(table[static_cast<std::size_t>(index)]);
        table[static_cast<std::size_t>(index)] = std::move(e);
        ++count;
    }

    const V* get(const std::string& key) const {
        std::int32_t hash = javaHashCode(key);
        int index = indexFor(hash, static_cast<int>(table.size()));
        for (Entry* e = table[static_cast<std::size_t>(index)].get(); e != nullptr; e = e->next.get())
            if (e->hash == hash && e->key == key)
                return &e->value;
        return nullptr;
    }

    V* get(const std::string& key) {
        return const_cast<V*>(static_cast<const JavaHashtable*>(this)->get(key));
    }

    bool containsKey(const std::string& key) const {
        return get(key) != nullptr;
    }

    void remove(const std::string& key) {
        std::int32_t hash = javaHashCode(key);
        int index = indexFor(hash, static_cast<int>(table.size()));
        std::unique_ptr<Entry>* link = &table[static_cast<std::size_t>(index)];
        while (*link) {
            if ((*link)->hash == hash && (*link)->key == key) {
                *link = std::move((*link)->next);
                --count;
                return;
            }
            link = &(*link)->next;
        }
    }

    void clear() {
        for (auto& b : table)
            b.reset();
        count = 0;
    }

    int size() const { return count; }

    // keys and values in the order of a Java Hashtable enumeration
    std::vector<std::pair<std::string, V>> entries() const {
        std::vector<std::pair<std::string, V>> result;
        for (int i = static_cast<int>(table.size()); i-- > 0;)
            for (Entry* e = table[static_cast<std::size_t>(i)].get(); e != nullptr; e = e->next.get())
                result.emplace_back(e->key, e->value);
        return result;
    }

    std::vector<std::string> keys() const {
        std::vector<std::string> result;
        for (auto& e : entries())
            result.push_back(e.first);
        return result;
    }

    std::vector<V> values() const {
        std::vector<V> result;
        for (auto& e : entries())
            result.push_back(e.second);
        return result;
    }

private:
    struct Entry {
        std::int32_t hash = 0;
        std::string key;
        V value{};
        std::unique_ptr<Entry> next;
    };

    std::vector<std::unique_ptr<Entry>> table;
    int count = 0;
    int threshold = 0;

    static int indexFor(std::int32_t hash, int length) {
        return (hash & 0x7FFFFFFF) % length;
    }

    void rehash() {
        int oldCapacity = static_cast<int>(table.size());
        int newCapacity = (oldCapacity << 1) + 1;
        std::vector<std::unique_ptr<Entry>> newMap(static_cast<std::size_t>(newCapacity));
        threshold = static_cast<int>(newCapacity * 0.75f);
        for (int i = oldCapacity; i-- > 0;) {
            std::unique_ptr<Entry> old = std::move(table[static_cast<std::size_t>(i)]);
            while (old) {
                std::unique_ptr<Entry> e = std::move(old);
                old = std::move(e->next);
                int index = indexFor(e->hash, newCapacity);
                e->next = std::move(newMap[static_cast<std::size_t>(index)]);
                newMap[static_cast<std::size_t>(index)] = std::move(e);
            }
        }
        table = std::move(newMap);
    }

    void copyFrom(const JavaHashtable& other) {
        table.clear();
        table.resize(other.table.size());
        count = other.count;
        threshold = other.threshold;
        for (std::size_t i = 0; i < other.table.size(); ++i) {
            std::unique_ptr<Entry>* link = &table[i];
            for (Entry* e = other.table[i].get(); e != nullptr; e = e->next.get()) {
                auto copy = std::make_unique<Entry>();
                copy->hash = e->hash;
                copy->key = e->key;
                copy->value = e->value;
                *link = std::move(copy);
                link = &(*link)->next;
            }
        }
    }
};

} // namespace antlrcpp

#endif
