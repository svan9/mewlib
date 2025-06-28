/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/


#ifndef MEW_MAP_LIB_SO2U
#define MEW_MAP_LIB_SO2U
#include "mewlib.h"
#include "mewtypes.h"
#include <vector>
#include <list>
#include <initializer_list>

namespace Hash {
  size_t get_hash(ID id) {
    return id;
  }
  size_t get_hash(const char* str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++))
      hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
  }
	size_t get_hash(const wchar_t* str) {
		size_t hash = 5381;
		wchar_t c;
		while ((c = *str++))
			hash = ((hash << 5) + hash) + c; // hash * 33 + c
		return hash;
	}

	template<typename T>
	size_t get_hash(const T& value) {
		return std::hash<T>{}(value);
	}
}
namespace mew {

  
  template<typename Key, typename Value>
  class map {
	private:
		struct Node {
			Key key;
			Value value;
			Node(Key &k, Value &v) : key(k), value(v) {}
		};
		
		std::vector<std::list<Node>> buckets;
		size_t size;
		static const size_t DEFAULT_CAPACITY = 16;
		static constexpr double LOAD_FACTOR = 0.75;


		size_t hash(Key key) const {
			return Hash::get_hash(key) % buckets.size();
		}

		void rehash() {
			std::vector<std::list<Node>> oldBuckets = buckets;
			buckets.clear();
			buckets.resize(oldBuckets.size() * 2);
			size = 0;
			for (auto &bucket : oldBuckets) {
				for (auto &node : bucket) {
					insert(node.key, node.value);
				}
			}
		}

	public:
		map() : buckets(DEFAULT_CAPACITY), size(0) {}

    map(std::initializer_list<std::pair<Key, Value>> init_list)
      : buckets(DEFAULT_CAPACITY), size(0) {
        for (const std::pair<Key, Value> &p : init_list) {
          insert((Key&)p.first, (Value&)p.second);
        }
    }

		void insert(Key& key, Value &value) {
			if (static_cast<double>(size) / buckets.size() >= LOAD_FACTOR) {
				rehash();
			}

			size_t index = hash(key);
			for (auto &node : buckets[index]) {
				if (node.key == key) {
					node.value = value;
					return;
				}
			}

			buckets[index].emplace_back(key, value);
			size++;
		}

		void insert(Key& key, Value&& value) {
			if (static_cast<double>(size) / buckets.size() >= LOAD_FACTOR) {
				rehash();
			}

			size_t index = hash(key);
			for (auto &node : buckets[index]) {
				if (node.key == key) {
					node.value = value;
					return;
				}
			}

			buckets[index].emplace_back(key, value);
			size++;
		}

		Value &at(Key key) {
			size_t index = hash(key);
			for (auto &node : buckets[index]) {
				if (node.key == key) {
					return node.value;
				}
			}
			throw std::out_of_range("Key not found");
		}

		bool contains(Key key) const {
			size_t index = hash(key);
			for (const auto &node : buckets[index]) {
				if (node.key == key) {
					return true;
				}
			}
			return false;
		}

		void erase(Key key) {
			size_t index = hash(key);
			auto &bucket = buckets[index];
			for (auto it = bucket.begin(); it != bucket.end(); ++it) {
				if (it->key == key) {
					bucket.erase(it);
					size--;
					return;
				}
			}
		}

		size_t getSize() const { return size; }
		bool isEmpty() const { return size == 0; }

		// for support
		class iterator {
			using bucket_iterator = typename std::list<Node>::iterator;
			using buckets_iterator = typename std::vector<std::list<Node>>::iterator;
			buckets_iterator buckets_it, buckets_end;
			bucket_iterator bucket_it;

			void advance_to_nonempty() {
				while (buckets_it != buckets_end && bucket_it == buckets_it->end()) {
					++buckets_it;
					if (buckets_it != buckets_end)
						bucket_it = buckets_it->begin();
				}
			}

		public:
			iterator(buckets_iterator b_it, buckets_iterator b_end)
				: buckets_it(b_it), buckets_end(b_end), bucket_it(b_it != b_end ? b_it->begin() : bucket_iterator()) {
				advance_to_nonempty();
			}

			iterator& operator++() {
				++bucket_it;
				advance_to_nonempty();
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return buckets_it != other.buckets_it || (buckets_it != buckets_end && bucket_it != other.bucket_it);
			}

			Node& operator*() { return *bucket_it; }
			Node* operator->() { return &(*bucket_it); }
		};

		iterator begin() {
			return iterator(buckets.begin(), buckets.end());
		}

		iterator end() {
			return iterator(buckets.end(), buckets.end());
		}

    Node* first() {
      for (auto& bucket : buckets) {
        if (!bucket.empty()) {
          return &bucket.front();
        }
      }
      return nullptr;
    }

    Node* last() {
      for (auto b_it = buckets.rbegin(); b_it != buckets.rend(); ++b_it) {
        if (!b_it->empty()) {
          return &b_it->back();
        }
      }
      return nullptr;
    }
	};

	template<typename Value>
	class hashmap {
	public:
		typedef u64 HashType; 
	private:
		struct Node {
			HashType hash;
			Value value;
			Node(HashType h, const Value& v) : hash(h), value((Value)v) {}
			Node(HashType h, Value&& v) : hash(h), value((Value)std::move(v)) {}
		};

		std::vector<std::list<Node>> buckets;
		size_t size;
		static const size_t DEFAULT_CAPACITY = 16;
		static constexpr double LOAD_FACTOR = 0.75;

		size_t bucket_index(HashType hash) const {
			return hash % buckets.size();
		}

		void rehash() {
			std::vector<std::list<Node>> oldBuckets = buckets;
			buckets.clear();
			buckets.resize(oldBuckets.size() * 2);
			size = 0;
			for (auto& bucket : oldBuckets) {
				for (auto& node : bucket) {
					insert(node.hash, node.value);
				}
			}
		}

	public:
		hashmap() : buckets(DEFAULT_CAPACITY), size(0) {}

		Value& insert(HashType hash, const Value& value) {
			if (static_cast<double>(size) / buckets.size() >= LOAD_FACTOR) {
				rehash();
			}
			size_t index = bucket_index(hash);
			for (auto& node : buckets[index]) {
				if (node.hash == hash) {
					node.value = value;
					return node.value;
				}
			}
			buckets[index].emplace_back(hash, value);
			size++;
			return buckets[index].back().value;
		}

		Value& insert(HashType hash, Value&& value) {
			if (static_cast<double>(size) / buckets.size() >= LOAD_FACTOR) {
				rehash();
			}
			size_t index = bucket_index(hash);
			for (auto& node : buckets[index]) {
				if (node.hash == hash) {
					node.value = std::move(value);
					return node.value;
				}
			}
			buckets[index].emplace_back(hash, std::move(value));
			size++;
			return buckets[index].back().value;
		}
		template<typename T>
		Value& insert(T key, const Value& value) {
			HashType hash = Hash::get_hash(key);
			if (static_cast<double>(size) / buckets.size() >= LOAD_FACTOR) {
				rehash();
			}
			size_t index = bucket_index(hash);
			for (auto& node : buckets[index]) {
				if (node.hash == hash) {
					node.value = value;
					return node.value;
				}
			}
			buckets[index].emplace_back(hash, (Value)value);
			size++;
			return buckets[index].back().value;
		}
		
		template<typename T>
		Value& insert(T key, Value&& value) {
			HashType hash = Hash::get_hash(key);
			if (static_cast<double>(size) / buckets.size() >= LOAD_FACTOR) {
				rehash();
			}
			size_t index = bucket_index(hash);
			for (auto& node : buckets[index]) {
				if (node.hash == hash) {
					node.value = std::move(value);
					return node.value;
				}
			}
			buckets[index].emplace_back(hash, std::move(value));
			size++;
			return buckets[index].back().value;
		}

		Value& at(HashType hash) {
			size_t index = bucket_index(hash);
			for (auto& node : buckets[index]) {
				if (node.hash == hash) {
					return node.value;
				}
			}
			throw std::out_of_range("Hash not found");
		}

		template<typename T>
		Value& at(T key) {
			HashType hash = Hash::get_hash(key);
			size_t index = bucket_index(hash);
			for (auto& node : buckets[index]) {
				if (node.hash == hash) {
					return node.value;
				}
			}
			throw std::out_of_range("Hash not found");
		}

		bool contains(HashType hash) const {
			size_t index = bucket_index(hash);
			for (const auto& node : buckets[index]) {
				if (node.hash == hash) {
					return true;
				}
			}
			return false;
		}

		template<typename T>
		bool contains(T key) const {
			HashType hash = Hash::get_hash(key);
			size_t index = bucket_index(hash);
			for (const auto& node : buckets[index]) {
				if (node.hash == hash) {
					return true;
				}
			}
			return false;
		}

		void erase(HashType hash) {
			size_t index = bucket_index(hash);
			auto& bucket = buckets[index];
			for (auto it = bucket.begin(); it != bucket.end(); ++it) {
				if (it->hash == hash) {
					bucket.erase(it);
					size--;
					return;
				}
			}
		}

		size_t getSize() const { return size; }
		bool isEmpty() const { return size == 0; }

		class iterator {
			using bucket_iterator = typename std::list<Node>::iterator;
			using buckets_iterator = typename std::vector<std::list<Node>>::iterator;
			buckets_iterator buckets_it, buckets_end;
			bucket_iterator bucket_it;

			void advance_to_nonempty() {
				while (buckets_it != buckets_end && bucket_it == buckets_it->end()) {
					++buckets_it;
					if (buckets_it != buckets_end)
						bucket_it = buckets_it->begin();
				}
			}

		public:
			iterator(buckets_iterator b_it, buckets_iterator b_end)
				: buckets_it(b_it), buckets_end(b_end), bucket_it(b_it != b_end ? b_it->begin() : bucket_iterator()) {
				advance_to_nonempty();
			}

			iterator& operator++() {
				++bucket_it;
				advance_to_nonempty();
				return *this;
			}

			bool operator!=(const iterator& other) const {
				return buckets_it != other.buckets_it || (buckets_it != buckets_end && bucket_it != other.bucket_it);
			}

			Node& operator*() { return *bucket_it; }
			Node* operator->() { return &(*bucket_it); }
		};

		iterator begin() {
			return iterator(buckets.begin(), buckets.end());
		}

		iterator end() {
			return iterator(buckets.end(), buckets.end());
		}

		Node* first() {
			for (auto& bucket : buckets) {
				if (!bucket.empty()) {
					return &bucket.front();
				}
			}
			return nullptr;
		}

		Node* last() {
			for (auto b_it = buckets.rbegin(); b_it != buckets.rend(); ++b_it) {
				if (!b_it->empty()) {
					return &b_it->back();
				}
			}
			return nullptr;
		}
	};

}

#endif