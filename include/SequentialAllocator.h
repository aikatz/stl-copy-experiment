//
// Created by Alex Katz on 10/18/18.
//

#ifndef STL_EXPLORATION_SEQUENTIAL_ALLOCATOR_H
#define STL_EXPLORATION_SEQUENTIAL_ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "config.h"
#include "fancy_pointer.h"
#include "slab_lookup_table.h"

inline size_t& get_offset(){
  static size_t offset = 0;
  return offset;
}

template<class T>
struct SequentialAllocator {
  typedef std::size_t       size_type;
  typedef T                 value_type;
  typedef fancy_pointer<T>  pointer;

  const size_type max_size;
  static size_type& offset;
  char* start_ptr;

  SequentialAllocator()
    : max_size (REGION_SIZE) {
      
    posix_memalign(reinterpret_cast<void**>(&start_ptr), getpagesize(), REGION_SIZE);
    std::memset(start_ptr, 0, REGION_SIZE);

    /*
    // Place 0xdeadbeef at the start of the region
    start_ptr[0] = (char)0xef;
    start_ptr[1] = (char)0xbe;
    start_ptr[2] = (char)0xad;
    start_ptr[3] = (char)0xde;

    // Place 0xdeadbeef at the end of the region
    start_ptr[REGION_SIZE - 1] = (char)0xde;
    start_ptr[REGION_SIZE - 2] = (char)0xad;
    start_ptr[REGION_SIZE - 3] = (char)0xbe;
    start_ptr[REGION_SIZE - 4] = (char)0xef;
     */

    slab_lookup_table[0][1] = start_ptr;
  }

  explicit SequentialAllocator(const SequentialAllocator<T>& allocator)
    : max_size(REGION_SIZE)
    , start_ptr(allocator.start_ptr) {}

  template<typename U>
  explicit SequentialAllocator(const SequentialAllocator<U>& allocator)
    : max_size(REGION_SIZE)
    , start_ptr(allocator.start_ptr) {}

  ~SequentialAllocator() {
    // Normally we'd free the region here, but it's done in main
  }

  pointer allocate(size_type n) {
    if (n * sizeof(T) > max_size - offset)
      return nullptr;

    std::size_t old_offset = offset;
    offset += n * sizeof(T);
    printf("[INFO] Allocated %lu bytes at offset %lu (address = %p)\n", n * sizeof(T), old_offset,
           static_cast<void*>(start_ptr + old_offset));
    return (pointer(0, 1, old_offset));
  }

  void deallocate(__attribute__((unused)) pointer ptr, __attribute__((unused)) size_type n) const {}
  
  char* get_region() const { return start_ptr; }
  
  bool address_was_allocated(uintptr_t addr) const {
    return (addr >= (uintptr_t)start_ptr && addr < (uintptr_t)start_ptr + max_size);
  }
};

template<typename T>
std::size_t& SequentialAllocator<T>::offset = get_offset();

template<class T, class U>
bool operator== (const SequentialAllocator<T>& a, const SequentialAllocator<U>& b) {
  return a.start_ptr == b.start_ptr;
}

template<class T, class U>
bool operator!= (const SequentialAllocator<T>& a, const SequentialAllocator<U>& b) {
  return !(a == b);
}

#endif //STL_EXPLORATION_SEQUENTIAL_ALLOCATOR_H
