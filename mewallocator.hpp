#ifndef MEW_ALLOCATOR_HPP
#define MEW_ALLOCATOR_HPP

#include "mewlib.h"
// #include "mewmalloc.hpp"

namespace mew {
	template<typename T, size_t alloc_size>
	class AllocatorBase {
	private:
		typedef AllocatorBase<T, alloc_size> self;
		size_t _M_size, _M_capacity;
		T* _M_data = nullptr;

		static T* _alloc(size_t count) {
			// return (T*)malloc(sizeof(T)*count);
			T* ptr = new T[count];
			memset(ptr, 0, count*sizeof(T));
			return ptr;
		}

		static void _dealloc(T* ptr) {
			if (std::is_arithmetic<T>::value) {
				delete ptr;
			} else {
				delete[] ptr;
			}
		}

		void _copy_all(T* dist, T* src) {
			for (size_t i = 0; i < _M_size; ++i) {
				T* to = dist+i;
				T* from = src+i;
				copy_to(to, *from);
			}
		}

	public:
		AllocatorBase() {}
		
		AllocatorBase(size_t count) : _M_capacity(count*alloc_size), _M_size(0) {
			_M_data = _alloc(count*alloc_size);
		}
		
		AllocatorBase(self& ref): _M_capacity(ref._M_capacity), _M_size(ref._M_size) {
			_M_data = _alloc(_M_capacity);
			_copy_all(_M_data, ref._M_data);
			printf("COPY ALLOCATOR\n");
		}
		
		constexpr size_t AllocSize() const noexcept {
			return alloc_size;
		}

		~AllocatorBase() {
			if (_M_data) {
				_dealloc(_M_data);
			}
		}

		size_t size() const noexcept {
			return _M_size * sizeof(T);
		}

		size_t count() const noexcept {
			return _M_size;
		}

		T* alloc(size_t at_before) {
			upsise();
			MewUserAssert(at_before <= _M_size, "Index out of range");

			T* insert_pos = (T*)(_M_data) + at_before;
			memmove(insert_pos + 1, insert_pos, (_M_size - at_before) * sizeof(T));
			++_M_size;
			return insert_pos;
		}

		void upsise() {
			if (_M_size >= _M_capacity) {
				_M_capacity += alloc_size;
				T* new_data = _alloc(_M_capacity);
				_copy_all(new_data, _M_data);
				if (_M_data) {
					_dealloc(_M_data);
				}
				_M_data = new_data;
			}
		}

		T* alloc() {
			upsise();
			return (T*)(_M_data) + _M_size++;
		}

		void copy(T* list, size_t size) {
			if (_M_size + size > _M_capacity) {
				_M_capacity += alloc_size;
				T* new_data = _alloc(_M_capacity);
				_copy_all(new_data, _M_data);
				if (_M_data) {
					_dealloc(_M_data);
				}
				_M_data = new_data;
			}
			memcpy((T*)(_M_data) + _M_size, list, size * sizeof(T));
			_M_size += size;
		}

		void copy(self& other) {
			if (_M_size + other._M_size > _M_capacity) {
				_M_capacity += alloc_size;
				T* new_data = _alloc(_M_capacity);
				_copy_all(new_data, _M_data);
				if (_M_data) {
					_dealloc(_M_data);
				}
				_M_data = new_data;
			}
			memcpy((T*)(_M_data) + _M_size, other._M_data, other._M_size * sizeof(T));
			_M_size += other._M_size;
		}

		self copy() {
			self copy_alloc;
			copy_alloc._M_capacity = _M_capacity;
			copy_alloc._M_data = new T[_M_capacity];
			memcpy(copy_alloc._M_data, _M_data, _M_size * sizeof(T));
			copy_alloc._M_size = _M_size;
			return copy_alloc;
		}		

		void dealloc(T* ptr) {
			if (ptr >= (T*)(_M_data) && ptr < (T*)(_M_data) + _M_size) {
				--_M_size;
				memmove(ptr, ptr + 1, (_M_size - (ptr - (T*)(_M_data))) * sizeof(T));
			}
		}

		void clear() {
			_M_size = 0;
			memset(_M_data, 0, _M_capacity);
		}

		void fill(T value) {
			_M_size = _M_capacity;
			memset(_M_data, value, _M_capacity);
		}

		void clear(T value) {
			_M_size = 0;
			memset(_M_data, value, _M_capacity);
		}

		void shift() {
			if (_M_size > 0) {
				memmove(_M_data, (T*)(_M_data) + 1, (_M_size - 1) * sizeof(T));
				--_M_size;
			}
		}

		void erase(size_t start, size_t size = 1) {
			MewUserAssert(size <= _M_size, "Index out of range");
			T* begin = (T*)(_M_data) + start;
			T* end = (T*)(_M_data) + start + size;
			memmove(begin, end, (_M_size - size) * sizeof(T));
			_M_size -= size;
		}

		void resize(size_t new_size) {
			if (new_size > _M_capacity) {
				_M_capacity = new_size * sizeof(T);
				void* new_data = new T[_M_capacity];
				memcpy(new_data, _M_data, _M_size * sizeof(T));
				if (_M_data) {
					_dealloc(_M_data);
				}
				_M_data = new_data;
			}
			_M_size = new_size;
		}

		void reserve(size_t size) {
			if (size > _M_capacity) {
				_M_capacity = size * sizeof(T);
				void* new_data = new T[_M_capacity];
				memcpy(new_data, _M_data, _M_size * sizeof(T));
				if (_M_data) {
					_dealloc(_M_data);
				}
				_M_data = new_data;
			}
		}

		T* begin() const noexcept {
			return (T*)(_M_data);
		}

		T* end() const noexcept {
			return (T*)(_M_data) + _M_size;
		}

		byte* rbegin() const noexcept {
			return (byte*)(_M_data) + _M_size * sizeof(T);
		}

		byte* rend() const noexcept {
			return (byte*)(_M_data);
		}
		
		void pop() {
			if (_M_size > 0) {
				--_M_size;
			}
		}
		
		void pop(size_t size) {
			if (_M_size*sizeof(T) > size) {
				_M_size -= size;
			} else {
				_M_size = 0;
			}
		}
	}; 

	template<typename T>
	using Allocator = AllocatorBase<T, 1>;

	template<typename T>
	using MidAllocator = AllocatorBase<T, 10>;
	
	template<typename T>
	using BigAllocator = AllocatorBase<T, 50>;

	template<typename T>
	using LargeAllocator = AllocatorBase<T, 100>;


};

#endif