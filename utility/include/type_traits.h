#pragma once

#include <type_traits>

#include <list>
#include <vector>
#include <deque>
#include <forward_list>

#include <stack>
#include <queue>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace ftl {

template<typename T>
struct is_container : std::false_type {};

template<typename T, typename Allocator>
struct is_container<std::list<T, Allocator>> : std::true_type {};

template<typename T, typename Allocator>
struct is_container<std::vector<T, Allocator>> : std::true_type {};

template<typename T, typename Allocator>
struct is_container<std::deque<T, Allocator>> : std::true_type {};

template<typename T, typename Allocator>
struct is_container<std::forward_list<T, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Compare, typename Allocator>
struct is_container<std::map<Key, T, Compare, Allocator>> : std::true_type {};

template<typename Key, typename Compare, typename Allocator>
struct is_container<std::set<Key, Compare, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Compare, typename Allocator>
struct is_container<std::multimap<Key, T, Compare, Allocator>> : std::true_type {};

template<typename Key, typename Compare, typename Allocator>
struct is_container<std::multiset<Key, Compare, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_container<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct is_container<std::unordered_set<Key, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_container<std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct is_container<std::unordered_multiset<Key, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename T, typename Container>
struct is_container<std::stack<T, Container>> : std::true_type {};

template<typename T, typename Container>
struct is_container<std::queue<T, Container>> : std::true_type {};

template<typename T, typename Container, typename Compare>
struct is_container<std::priority_queue<T, Container, Compare>> : std::true_type {};


template<typename T>
struct is_value_container : std::false_type {};

template<typename T, typename Allocator>
struct is_value_container<std::list<T, Allocator>> : std::true_type {};

template<typename T, typename Allocator>
struct is_value_container<std::vector<T, Allocator>> : std::true_type {};

template<typename T, typename Allocator>
struct is_value_container<std::deque<T, Allocator>> : std::true_type {};

template<typename T, typename Allocator>
struct is_value_container<std::forward_list<T, Allocator>> : std::true_type {};

template<typename T, typename Container>
struct is_value_container<std::stack<T, Container>> : std::true_type {};

template<typename T, typename Container>
struct is_value_container<std::queue<T, Container>> : std::true_type {};

template<typename T, typename Container, typename Compare>
struct is_value_container<std::priority_queue<T, Container, Compare>> : std::true_type {};


template<typename T>
struct is_keyvalue_container : std::false_type {};

template<typename Key, typename T, typename Compare, typename Allocator>
struct is_keyvalue_container<std::map<Key, T, Compare, Allocator>> : std::true_type {};

template<typename Key, typename Compare, typename Allocator>
struct is_keyvalue_container<std::set<Key, Compare, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Compare, typename Allocator>
struct is_keyvalue_container<std::multimap<Key, T, Compare, Allocator>> : std::true_type {};

template<typename Key, typename Compare, typename Allocator>
struct is_keyvalue_container<std::multiset<Key, Compare, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_keyvalue_container<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct is_keyvalue_container<std::unordered_set<Key, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_keyvalue_container<std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator>> : std::true_type {};

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct is_keyvalue_container<std::unordered_multiset<Key, Hash, KeyEqual, Allocator>> : std::true_type {};

}
