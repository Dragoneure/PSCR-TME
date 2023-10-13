#pragma once
#include <vector>
#include <forward_list>
#include <cstddef>
#include <functional>

namespace pr {

template<typename K, typename V>
class HashMap {

	struct Entry {
		public :
			const K key;
			V value;
	};
	std::vector<std::forward_list<Entry>> buckets;
	typedef std::vector<std::forward_list<Entry>>  buckets_t;
	public :
		HashMap(size_t alloc) {
			buckets = new buckets_t(alloc);
		}

		bool put(const K & key, const V & value) {
				size_t h = std::hash<K>()(key);
				size_t target = h%buckets.size();
				for (auto & ent : buckets[target]) {
					if (ent.key == key) {
						ent.value = value;
						return true;
					}
				}
				buckets[target].emplace_front(Entry(key,value));
				return false;
			}

		V get(const K & key) {
				size_t h = std::hash<K>()(key);
				size_t target = h%buckets.size();
				for (auto & ent : buckets[target]) {
					if (ent.key == key) {
						return ent.value;
					}
				}
				return nullptr;
			}

			void grow() {
				HashMap tmp( 2 * &buckets.size());
				for (auto & l = buckets;;) {
					for (auto & o = l;;) {
						tmp.put(o.key,o.value);
					}
				}
				this->buckets = tmp.buckets;
			}

};


}
