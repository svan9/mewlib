#ifndef MEW_MALLOC_HPP
#define MEW_MALLOC_HPP

#include "mewlib.h"
#pragma pack(push, 1)

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#include <sys/types.h>

#define PROT_READ     0x1
#define PROT_WRITE    0x2
/* This flag is only available in WinXP+ */
#ifdef FILE_MAP_EXECUTE
#define PROT_EXEC     0x4
#else
#define PROT_EXEC        0x0
#define FILE_MAP_EXECUTE 0
#endif

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_ANONYMOUS 0x20
#define MAP_ANON      MAP_ANONYMOUS
#define MAP_FAILED    ((void *) -1)

#ifdef __USE_FILE_OFFSET64
# define DWORD_HI(x) (x >> 32)
# define DWORD_LO(x) ((x) & 0xffffffff)
#else
# define DWORD_HI(x) (0)
# define DWORD_LO(x) (x)
#endif
#else 
#include <sys/mman.h>
#endif

static void *mmap(void *start, size_t length, int prot, int flags, int fd, off_t offset) {
	if (prot & ~(PROT_READ | PROT_WRITE | PROT_EXEC))
		return MAP_FAILED;
	if (fd == -1) {
		if (!(flags & MAP_ANON) || offset)
			return MAP_FAILED;
	} else if (flags & MAP_ANON)
		return MAP_FAILED;

	DWORD flProtect;
	if (prot & PROT_WRITE) {
		if (prot & PROT_EXEC)
			flProtect = PAGE_EXECUTE_READWRITE;
		else
			flProtect = PAGE_READWRITE;
	} else if (prot & PROT_EXEC) {
		if (prot & PROT_READ)
			flProtect = PAGE_EXECUTE_READ;
		else if (prot & PROT_EXEC)
			flProtect = PAGE_EXECUTE;
	} else
		flProtect = PAGE_READONLY;

	off_t end = length + offset;
	HANDLE mmap_fd, h;
	if (fd == -1)
		mmap_fd = INVALID_HANDLE_VALUE;
	else
		mmap_fd = (HANDLE)_get_osfhandle(fd);
	h = CreateFileMapping(mmap_fd, NULL, flProtect, DWORD_HI(end), DWORD_LO(end), NULL);
	if (h == NULL)
		return MAP_FAILED;

	DWORD dwDesiredAccess;
	if (prot & PROT_WRITE)
		dwDesiredAccess = FILE_MAP_WRITE;
	else
		dwDesiredAccess = FILE_MAP_READ;
	if (prot & PROT_EXEC)
		dwDesiredAccess |= FILE_MAP_EXECUTE;
	if (flags & MAP_PRIVATE)
		dwDesiredAccess |= FILE_MAP_COPY;
	void *ret = MapViewOfFile(h, dwDesiredAccess, DWORD_HI(offset), DWORD_LO(offset), length);
	if (ret == NULL) {
		CloseHandle(h);
		ret = MAP_FAILED;
	}
	return ret;
}

static void munmap(void *addr, size_t length)
{
	UnmapViewOfFile(addr);
	/* ruh-ro, we leaked handle from CreateFileMapping() ... */
}

#undef DWORD_HI
#undef DWORD_LO

namespace mew::mem {
namespace {
	struct chunk {
		bool is_lock;
		size_t size;
		chunk* next;
		chunk* prev;
		byte* data;
	}; // [HEAD] [DATA]
	
	static const size_t _pool_size = 1024*1024;
	static byte* _memory_pool = (byte*)mmap(NULL, _pool_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);//[_pool_size];	


	#define HEAD_CHUNK_SIZE (sizeof(size_t)+sizeof(bool)+(2*sizeof(chunk*)))

	#define HEAP_SIZE_ALIGN 16

	chunk* mem2chunk(void* ptr) noexcept {
		return (chunk*)((byte*)(ptr) - HEAD_CHUNK_SIZE);
	}

	void* chunk2mem(chunk* c) noexcept {
		return (void*)((byte*)(c) + HEAD_CHUNK_SIZE);
	}


	size_t align_size(size_t& na) {
		if (na < HEAP_SIZE_ALIGN) {
			return HEAP_SIZE_ALIGN/2;
		}
		return na + (HEAP_SIZE_ALIGN - (na % HEAP_SIZE_ALIGN));
	}

	void _init_heap(chunk* c) {
		c->size = _pool_size;
		c->next = NULL;
		c->prev = NULL;
	}

	chunk* _heap_last(chunk* c) {
		if ((byte*)c == _memory_pool) {
			_init_heap(c);
			return c;
		}
		if (c->size == 0) { return c; }
		return c->next;
	}

	chunk* split_chunk(chunk* c, size_t size) {
		size_t remnant = c->size - size;
		if (remnant < sizeof(chunk)+sizeof(size_t)) { return c; }
		byte* pool_end = _memory_pool + _pool_size;
		chunk* _prev = c;
		_prev->size = remnant - (2*HEAD_CHUNK_SIZE);
		byte* _prev_data_end = (byte*)(&_prev->data) + _prev->size;
		chunk* _next = (chunk*)_prev_data_end;
		_next->size = size;
		_next->next = (chunk*)((&_next->data)+size);
		_next->prev = _prev;
		_prev->next = _next;
		if ((byte*)_next->next >= pool_end) {
			_next->next = NULL;
		}
		return _next;
	}

	#define CHUNK_LOCK(c) ((c)->is_lock = true)
	#define CHUNK_UNLOCK(c) ((c)->is_lock = false)

	chunk* find_free(size_t size) {
		chunk* c = (chunk*)_memory_pool;
		while((c = _heap_last(c)) != NULL) {
			if (c->is_lock) {continue;}
			if (c->size == size) { return c; }
			if (c->size > size) { 
				c = split_chunk(c, size);
				return c;
			}
		}
		return NULL;
	}

	chunk* try_merge_chunks(chunk* c) {
		chunk* prev = c->prev;
		chunk* next = c->next;
		if (prev && !prev->is_lock) {
			prev->size += c->size + HEAD_CHUNK_SIZE;
			prev->next = next;
			c = prev;
		}
		if (next && !next->is_lock) {
			c->size += next->size + HEAD_CHUNK_SIZE;
			c->next = next->next;
		}
		memset(&c->data, 0, c->size);
		return c;
	}
} // end iternal
	
	void* alloc(size_t size) {
		size = align_size(size);
		chunk* c = find_free(size);
		if (!c) {return NULL;}
		CHUNK_LOCK(c);
		return chunk2mem(c);
	}

	void dealloc(void* ptr) {
		chunk* c = mem2chunk(ptr);
		CHUNK_UNLOCK(c);
		while ((c = try_merge_chunks(c)) && (c->prev || c->next)) {}
	}

}

#pragma pack(pop)

#endif