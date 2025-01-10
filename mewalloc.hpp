#ifndef MEW_ALLOC_LIB_SO2U
#define MEW_ALLOC_LIB_SO2U

#include "mewall.h"

namespace mew {

template<size_t MSIZE>
class Alloc {
public:
	typedef struct {
		size_t size;
		byte data[];
	} m_range;
private:
	byte m_space[MSIZE];
public:
	constexpr static m_range nopos = m_range{.size=(size_t)(-1)};

	////////////////////////////////////////////////////////////
	Alloc() {
		fill();
	}

	////////////////////////////////////////////////////////////
	void fill(byte val = 0) {
		for (uint i = 0; i < MSIZE; ++i) {
			m_space[i] = val;
		}
	}

	////////////////////////////////////////////////////////////
	void mset(size_t idx, byte* val, size_t size) {
		MewAssert(idx < max_size());
		MewAssert(idx+size < max_size());
		for (size_t i = 0; i < size; ++i) {
			m_space[i+idx] = val[i];
		}
	}
	////////////////////////////////////////////////////////////
	template<typename T>
	void set(size_t idx, T&& val) {
		mset(idx, (byte*)(&val), sizeof(T));
	}
	
	////////////////////////////////////////////////////////////
	bool mcmp(size_t idx, byte* val, size_t size) {
		MewAssert(idx < max_size());
		MewAssert(idx+size < max_size());
		for (size_t i = 0; i < size; ++i) {
			if (m_space[i+idx] != val[i]) 
				{ return false; }
		}
		return true;
	}
	////////////////////////////////////////////////////////////
	template<typename T>
	bool cmp(size_t idx, T&& val) {
		return mcmp(idx, &val, sizeof(T));
	}

	////////////////////////////////////////////////////////////
	bool is_nopos(size_t idx) const noexcept {
		return cmp(idx, nopos);
	}

	////////////////////////////////////////////////////////////
	byte* begin() const noexcept {
		return (byte*)m_space;
	}

	////////////////////////////////////////////////////////////
	byte* end() const noexcept {
		return (byte*)((size_t)m_space+max_size());
	}

	////////////////////////////////////////////////////////////
	void check_range(byte* pointer) {
		MewAssert(in_range(pointer));
	}

	////////////////////////////////////////////////////////////
	bool in_range(byte* pointer) {
		return (pointer > begin()) && (pointer < end());
	}

	////////////////////////////////////////////////////////////
	constexpr size_t max_size() const noexcept {
		return MSIZE;
	}

	////////////////////////////////////////////////////////////
	constexpr m_range* ranges() const noexcept {
		return (m_range*)m_space;
	}

	////////////////////////////////////////////////////////////
	byte* space() const noexcept {
		return (byte*)m_space;
	}

	////////////////////////////////////////////////////////////
	byte* alloc(size_t size) {
		for (int i = 0; i < max_size();) {
			m_range* current = (m_range*)(m_space+i);
			if (current->size != 0) {
				i += current->size+sizeof(size_t);
				continue;
			}
			set(i, m_range{.size=size});
			return m_space+i+sizeof(size_t);			
		}
		return nullptr;
	}

	////////////////////////////////////////////////////////////
	void dealloc(byte* pointer) {
		MewAssert(pointer != nullptr);
		check_range(pointer);
		m_range* current = (m_range*)(pointer-sizeof(size_t));	
		for (uint i = 0; i < current->size; ++i) {
			current->data[i] = 0;
		}
		current->size = 0;
	}

	////////////////////////////////////////////////////////////
	byte* realloc(byte* pointer, size_t size) {
		if (pointer == nullptr) {
			return alloc(size);
		}
		check_range(pointer);
		m_range* current = (m_range*)(pointer-sizeof(size_t));	
		if (size == current->size) {
			return pointer;
		}
		if (size == 0) {
			dealloc(pointer);
			return nullptr;
		}
		byte* __new = alloc(size);
		if (__new == nullptr) 
			{ return nullptr; }
		if (size > current->size) {
			for (uint i = 0; i < current->size; ++i) {
				__new[i] = current->data[i];
			}
			return __new;
		}
		if (size < current->size) {
			for (uint i = 0; i < size; ++i) {
				__new[i] = current->data[i];
			}
			return __new;
		}
	}

	////////////////////////////////////////////////////////////
	template<typename T>
	T* alloc() {
		return (T*)alloc(sizeof(T));
	}

	////////////////////////////////////////////////////////////
	template<typename T>
	void dealloc(T* pointer) {
		return dealloc((byte*)pointer);
	}
	
	////////////////////////////////////////////////////////////
	template<typename T>
	m_range* get_data(T* pointer) {
		check_range((byte*)pointer);
		return (m_range*)(((byte*)pointer)-sizeof(size_t));	
	}

	////////////////////////////////////////////////////////////
	template<typename T>
	bool exist(T* pointer) {
		m_range* __data = get_data(pointer);
		return __data->size != 0;
	}
};

typedef Alloc<1024> Alloc1KB;
typedef Alloc<1024*1024> Alloc1MB;
typedef Alloc<1024*1024*1024> Alloc1GB;
	
};

#endif