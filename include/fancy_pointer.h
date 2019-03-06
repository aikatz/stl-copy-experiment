//
// Created by Alex Katz on 11/13/18.
//

#ifndef FANCY_POINTER_H
#define FANCY_POINTER_H

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <type_traits>

#include "slab_lookup_table.h"
#include "config.h"

template<typename T>
struct fance_ptr_hlpr {
 using ref_t = T&;

};

template<>
struct fance_ptr_hlpr<void>{
  using ref_t = void;
};

template<typename T>
struct fancy_pointer {
  typedef std::ptrdiff_t                  difference_type;
  typedef T                               value_type;
  typedef fancy_pointer<T>                pointer;
  typedef std::random_access_iterator_tag iterator_category;
  // TODO: Fix this (can't have a reference to void)
  using reference = typename fance_ptr_hlpr<T>::ref_t;
  //typedef typename std::conditional<std::is_void<T>::value, T, T&>::type reference;

  int m_id;             // Machine id
  int s_id;             // Slab id
  std::size_t offset;   // Offset with the slab

  fancy_pointer()
  : m_id(-1)
  , s_id(-1)
  , offset(0xdeadbeef) {}


  fancy_pointer(std::nullptr_t n)
    : m_id(-1)
    , s_id(-1)
    , offset(0xdeadbeef) {}

  fancy_pointer(int m, int s, std::size_t o)
    : m_id(m)
    , s_id(s)
    , offset(o) {}

  fancy_pointer(const fancy_pointer<T>& p)
    : m_id(p.m_id)
    , s_id(p.s_id)
    , offset(p.offset) {}

  template<typename U>
  fancy_pointer(const fancy_pointer<U>& p)
    : m_id(p.m_id)
    , s_id(p.s_id)
    , offset(p.offset) {}

  static T* to_address(fancy_pointer<T> p) {
    return (T*)(slab_lookup_table[p.m_id][p.s_id] + p.offset);
  }

  template<bool V = !std::is_void_v<T>>
  static fancy_pointer pointer_to(std::enable_if_t<V, T>& r) {
    std::size_t offset = ((std::size_t)std::addressof(r) - (std::size_t)slab_lookup_table[0][1]);
    if (offset < REGION_SIZE) {
      return *(new fancy_pointer<T>(0, 1, offset));
    } else {
      printf("[INFO] pointer_to(%p) -> offset = %p\n", std::addressof(r), (void *) offset);
      printf("[INFO] region = %p\n", (void*)slab_lookup_table[0][1]);
      return *(new fancy_pointer<T>(0, 0, (std::size_t)std::addressof(r)));
    }
  }

  T* operator->() const {
    return (T*)(slab_lookup_table[m_id][s_id] + offset);
  }

  reference operator*() const {
    return *(T*)(slab_lookup_table[m_id][s_id] + offset);
  }

  explicit operator bool() const {
    return slab_lookup_table[m_id][s_id] != nullptr;
  }

  fancy_pointer<T>& operator++() {
    offset++;
    return *this;
  }

  fancy_pointer<T> operator++(int) {
    fancy_pointer tmp(*this);
    operator++();
    return tmp;
  }

  fancy_pointer<T>& operator--() {
    offset--;
    return *this;
  }

  fancy_pointer<T> operator--(int) {
    fancy_pointer tmp(*this);
    operator--();
    return tmp;
  }
};

/*
 * Equality operators for (non-const ? non-const)
 */

template<typename T>
bool operator== (const fancy_pointer<T>& a, const fancy_pointer<T>& b) {
  return (a.m_id == b.m_id) && (a.s_id == b.s_id)  && (a.offset == b.offset);
}

template<typename T>
bool operator!= (const fancy_pointer<T>& a, const fancy_pointer<T>& b) {
  return !(a == b);
}

template<typename T>
bool operator== (const fancy_pointer<T>& a, std::nullptr_t n) {
  // TODO: Possibly fix this? Might work fine as-is
  return (a.m_id == -1 && a.s_id == -1);
}

template<typename T>
bool operator!= (const fancy_pointer<T>& a, std::nullptr_t n) {
  return !(a == n);
}

/*
 * Comparison operators for (non-const ? non-const)
 */

template<typename T>
bool operator< (const fancy_pointer<T>& lhs, const fancy_pointer<T>& rhs) {
  if (lhs.m_id != rhs.m_id || lhs.s_id != rhs.s_id) {
    assert(false && "Error: Fancy pointers must have same m_id and s_id to for comparison to be valid");
  }

  return lhs.offset < rhs.offset;
}

template<typename T>
bool operator> (const fancy_pointer<T>& lhs, const fancy_pointer<T>& rhs) {
  return rhs < lhs;
}

template<typename T>
bool operator<= (const fancy_pointer<T>& lhs, const fancy_pointer<T>& rhs) {
  return !(lhs > rhs);
}

template<typename T>
bool operator>= (const fancy_pointer<T>& lhs, const fancy_pointer<T>& rhs) {
  return !(lhs < rhs);
}

/*
 * Comparison operators for (const ? non-const)
 */

template<typename T>
bool operator< (const fancy_pointer<const T>& lhs, const fancy_pointer<T>& rhs) {
  if (lhs.m_id != rhs.m_id || lhs.s_id != rhs.s_id) {
    assert(false && "Error: Fancy pointers must have same m_id and s_id to for comparison to be valid");
  }

  return lhs.offset < rhs.offset;
}

template<typename T>
bool operator> (const fancy_pointer<const T>& lhs, const fancy_pointer<T>& rhs) {
  return rhs < lhs;
}

template<typename T>
bool operator<= (const fancy_pointer<const T>& lhs, const fancy_pointer<T>& rhs) {
  return !(lhs > rhs);
}

template<typename T>
bool operator>= (const fancy_pointer<const T>& lhs, const fancy_pointer<T>& rhs) {
  return !(lhs < rhs);
}

/*
 * Comparison operators for (non-const ? const)
 */

template<typename T>
bool operator< (const fancy_pointer<T>& lhs, const fancy_pointer<const T>& rhs) {
  if (lhs.m_id != rhs.m_id || lhs.s_id != rhs.s_id) {
    assert(false && "Error: Fancy pointers must have same m_id and s_id to for comparison to be valid");
  }

  return lhs.offset < rhs.offset;
}

template<typename T>
bool operator> (const fancy_pointer<T>& lhs, const fancy_pointer<const T>& rhs) {
  return rhs < lhs;
}

template<typename T>
bool operator<= (const fancy_pointer<T>& lhs, const fancy_pointer<const T>& rhs) {
  return !(lhs > rhs);
}

template<typename T>
bool operator>= (const fancy_pointer<T>& lhs, const fancy_pointer<const T>& rhs) {
  return !(lhs < rhs);
}

template<typename T>
typename fancy_pointer<T>::difference_type operator- (const fancy_pointer<T>& lhs, const fancy_pointer<T>& rhs) {
  if (lhs.m_id != rhs.m_id || lhs.s_id != rhs.s_id) {
    assert(false && "Error: Fancy pointers must have same m_id and s_id to use operator-");
  }
  return lhs.offset - rhs.offset;
}

template<typename T>
fancy_pointer<T> operator+ (const fancy_pointer<T>& lhs, const std::size_t rhs) {
  fancy_pointer<T> tmp(lhs);
  tmp.offset += rhs;
  return tmp;
}


template<typename T>
fancy_pointer<T> operator- (const fancy_pointer<T>& lhs, const std::size_t rhs) {
  fancy_pointer<T> tmp(lhs);
  tmp.offset -= rhs;
  return tmp;
}

template<typename T>
typename fancy_pointer<T>::difference_type operator- (const fancy_pointer<const T>& lhs, const fancy_pointer<T>& rhs) {
  if (lhs.m_id != rhs.m_id || lhs.s_id != rhs.s_id) {
    assert(false && "Error: Fancy pointers must have same m_id and s_id to use operator-");
  }
  return lhs.offset - rhs.offset;
}


#endif //FANCY_POINTER_H
