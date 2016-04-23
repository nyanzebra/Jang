#pragma once

#include <unordered_map>
#include <list>

#include "../type_traits.h"

namespace ftl {
namespace memory {

template <typename Key, typename T>
using cache_pair = std::pair<const Key, typename std::list<T>::iterator>;

template <
	typename Key,
	typename T,
	std::size_t N,
	typename CollectionAllocator = std::allocator<T>,
	typename Compare = std::less<Key>,
	typename CacheAllocator = std::allocator<cache_pair<Key, T>>
>
class cache {
public:
	typedef Key key_type;
	typedef std::pair<const key_type, typename std::list<std::pair<key_type, T>>::iterator> value_type;
	typedef T& reference;
	typedef const reference const_reference;

	typedef std::size_t size_type;

	typedef std::list<std::pair<key_type, T>, CollectionAllocator> collection_type;
	typedef typename collection_type::iterator mapped_type;
	typedef std::map<Key, mapped_type, Compare, CacheAllocator> cache_type;

	typedef typename cache_type::iterator iterator;
	typedef typename cache_type::const_iterator const_iterator;

	cache() = default;
	cache(const cache&) = default;
	cache(cache&&) = default;
	~cache() = default;

	cache& operator=(const cache&) = default;

	reference at(const key_type& key) {
		if (!_has(key)) {
			throw std::out_of_range("ERROR:\tkey not present in cache");
		}
		return _value(key);
	}

	const_reference at(const key_type& key) const {
		if (!_has(key)) {
			throw std::out_of_range("ERROR:\tkey not present in cache");
		}
		return _value(key);
	}

	reference operator[](const key_type& key) {
		if (_is_full()) {
			_remove_least_used();
		}
		if (!_has(key)) {
			insert(key, value_type());
		}
		return _value(key);
	}

	reference operator[](key_type&& key) {
		if (_is_full()) {
			_remove_least_used();
		}
		if (!_has(std::forward<key_type>(key))) {
			insert(std::forward<key_type>(key), value_type());
		}
		return _value(std::forward<key_type>(key));
	}

	void clear() { _collection.clear(); _cache.clear(); }

	template<typename... Args>
	void insert(const key_type& key, Args&&... args) { emplace(key, std::forward<Args>(args)...); }

	void insert(const key_type& key, const T& value) { emplace(key, value); }

	void insert(const key_type& key, T&& value) { emplace(key, std::forward<T>(value)); }

	template<typename... Args>
	void emplace(const key_type& key, Args&&... args) {
		if (_is_full()) {
			_remove_least_used();
		}
		auto it = _collection.emplace(_collection.begin(), std::make_pair(key, std::forward<Args>(args)...));
		_cache.insert(value_type(key, it));
	}

	template<typename... Args>
	void emplace(key_type&& key, Args&&... args) {
		if (_is_full()) {
			_remove_least_used();
		}
		auto it = _collection.emplace(_collection.begin(), std::make_pair(key, std::forward<Args>(args)...));
		_cache.insert(value_type(std::forward<key_type>(key), it));
	}

	void erase(iterator pos) { _collection.erase(pos->second); _cache.erase(pos); }

	iterator erase(const_iterator pos) { _collection.erase(pos->second); return _cache.erase(pos); }

	void erase(iterator first, iterator last) {
		for (auto it = first; it != last; it++) {
			_collection.erase(it);
		}
		_cache.erase(first, last);
	}

	iterator erase(const_iterator first, const_iterator last) {
		for (auto it = first; it != last; it++) {
			_collection.erase(it);
		}
		return _cache.erase(first, last);
	}

	size_type erase(const key_type& key) {
		if (_has(key)) {
			_collection.erase(_cache.find(key)->second);
			_cache.erase(key);
		}
		return 0;
	}

	const size_type& size()const { return _cache.size(); }

	const size_type& capacity()const { return N; }

	const bool empty() const { return _cache.size() == 0; }

private:
	const bool _has(const key_type& key) const { return _cache.find(key) != _cache.end(); }

	const bool _has(key_type&& key) const { return _cache.find(std::forward<key_type>(key)) != _cache.end(); }

	const bool _is_full() const { return _cache.size() == N; }

	reference _value(const key_type& key) { _swap_to_front(key); return _cache[key]->second; }

	const_reference _value(const key_type& key) const { _swap_to_front(key); return _cache[key]->second; }

	reference _value(key_type&& key) { _swap_to_front(std::forward<key_type>(key)); return _cache[std::forward<key_type>(key)]->second; }

	const_reference _value(key_type&& key) const { _swap_to_front(std::forward<key_type>(key)); return _cache[std::forward<key_type>(key)]->second; }

	void _remove_least_used() { _cache.erase(std::next(_collection.end(), -1)->first); _collection.erase(std::next(_collection.end(), -1)); }

	void _swap_to_front(const key_type& key) { std::iter_swap(_collection.begin(), _cache[key]); _cache[key] = _collection.begin(); }

	void _swap_to_front(key_type&& key) { std::iter_swap(_collection.begin(), _cache[std::forward<key_type>(key)]); _cache[std::forward<key_type>(key)] = _collection.begin(); }

private:
	cache_type _cache;
	collection_type _collection;
};

}
}
