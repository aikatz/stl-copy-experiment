//
// Created by Alex Katz on 10/18/18.
//

#ifndef STL_EXPLORATION_SEQUENTIAL_ALLOCATOR_H
#define STL_EXPLORATION_SEQUENTIAL_ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "config.h"

#ifdef USE_FANCY_POINTER
#include "FancyPointer.h"
#endif

template<class T>
struct SequentialAllocator {
  typedef std::size_t       size_type;
  typedef T                 value_type;
#ifdef USE_FANCY_POINTER
  typedef FancyPointer<T>   pointer;
#else
  typedef T*                pointer;
#endif

  const size_type max_size;
  static size_type offset;
  char* start_ptr;

  SequentialAllocator() : max_size (REGION_SIZE), start_ptr((char*)malloc(max_size)) {}

  explicit SequentialAllocator(const SequentialAllocator<T>& allocator) : max_size(REGION_SIZE) {
    this->start_ptr = allocator.start_ptr;
  }

  template<typename U>
  explicit SequentialAllocator(const SequentialAllocator<U>& allocator) : max_size(REGION_SIZE) {
    this->start_ptr = allocator.start_ptr;
  }

  ~SequentialAllocator() {
    // Normally we'd free the region here, but it's done in main
  }

  pointer allocate(size_type n) {
    if (n * sizeof(T) > max_size - offset)
      return nullptr;

#ifdef USE_FANCY_POINTER
    std::size_t old_offset = offset;
    offset += n * sizeof(T);
    return (FancyPointer<T>(start_ptr, old_offset));
#else
    auto newly_allocated = (pointer)((size_t)start_ptr + offset);
    offset += n * sizeof(T);
    return newly_allocated;
#endif
  }

  void deallocate(pointer ptr, size_type n) const {}
  char* get_region() const { return start_ptr; }
  bool address_was_allocated(uintptr_t addr) const {
    return (addr >= (uintptr_t)start_ptr && addr < (uintptr_t)start_ptr + max_size);
  }
};

template<class T, class U>
bool operator== (const SequentialAllocator<T>& a, const SequentialAllocator<U>& b) {
  return a.start_ptr == b.start_ptr;
}

template<class T, class U>
bool operator!= (const SequentialAllocator<T>& a, const SequentialAllocator<U>& b) {
  return !(a == b);
}

template<typename T>
typename SequentialAllocator<T>::size_type SequentialAllocator<T>::offset = 0;

#endif //STL_EXPLORATION_SEQUENTIAL_ALLOCATOR_H
