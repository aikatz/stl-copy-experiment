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
struct reference_helper {
  using reference = T&;
};

template<>
struct reference_helper<void> {
  using reference = void;
};

template<typename T>
struct fancy_pointer {
  typedef std::ptrdiff_t                           difference_type;
  typedef T                                        value_type;
  typedef fancy_pointer<T>                         pointer;
  typedef typename reference_helper<T>::reference  reference;
  typedef const reference                          const_reference;
  typedef std::random_access_iterator_tag          iterator_category;

  int m_id;                 // Machine id
  int s_id;                 // Slab id
  difference_type offset;   // Offset with the slab

  fancy_pointer()
    : m_id(-1)
    , s_id(-1)
    , offset(0) {}

  fancy_pointer(__attribute__((unused)) std::nullptr_t n)
    : m_id(-1)
    , s_id(-1),
    offset(0) {}

  fancy_pointer(int m, int s, std::size_t o)
    : m_id(m)
    , s_id(s)
    , offset(o) {}

  fancy_pointer(const fancy_pointer<T> &p)
    : m_id(p.m_id)
    , s_id(p.s_id)
    , offset(p.offset) {}

  template<typename U, typename ignore = std::enable_if_t<!std::is_const_v<U> || std::is_const_v<T>>* >
  fancy_pointer(const fancy_pointer<U> &p)
    : m_id(p.m_id)
    , s_id(p.s_id)
    , offset(p.offset) {}

  static T *to_address(fancy_pointer<T> p) {
    return (T *) (slab_lookup_table[p.m_id][p.s_id] + p.offset);
  }

  template<bool V = !std::is_void_v<T>>
  static fancy_pointer pointer_to(std::enable_if_t<V, T> &r) {
    std::size_t offset = ((std::size_t) std::addressof(r) - (std::size_t) slab_lookup_table[0][1]);
    if (offset < REGION_SIZE) {
      return *(new fancy_pointer<T>(0, 1, offset));
    } else {
      printf("[INFO] pointer_to(%p) -> offset = %p\n", (void *) std::addressof(r), (void *) offset);
      printf("[INFO] region = %p\n", (void *) slab_lookup_table[0][1]);
      return *(new fancy_pointer<T>(0, 0, (std::size_t) std::addressof(r)));
    }
  }

  explicit operator bool() const { return slab_lookup_table[m_id][s_id] != nullptr; }

  /*
   * De-reference operators
   */
  T *operator->() const { return (T *) (slab_lookup_table[m_id][s_id] + offset); }
  reference operator*() const { return *(T *) (slab_lookup_table[m_id][s_id] + offset); }

  /*
   * Subscript operator
   */
  reference operator[](std::size_t index) { return *(to_address(*this) + index); }
  const_reference operator[](std::size_t index) const { return *(to_address(*this) + index); }

  /*
   * Equality operators
   */
  bool operator==(const fancy_pointer<T> &rhs) const {
    return (m_id == rhs.m_id) && (s_id == rhs.s_id) && (offset == rhs.offset);
  }
  bool operator!=(const fancy_pointer<T> &rhs) const { return !(*this == rhs); }
  bool operator==(__attribute__((unused)) std::nullptr_t rhs) const { return (m_id == -1 && s_id == -1); }
  bool operator!=(std::nullptr_t n) const { return !(*this == n); }

  /*
   * Comparison operators
   */
  bool operator<(const fancy_pointer<const T> &rhs) const {
    if (m_id != rhs.m_id || s_id != rhs.s_id) {
      assert(false && "Error: operands have different s_id or m_id fields");
    }

    return offset < rhs.offset;
  }
  bool operator>(const fancy_pointer<const T> &rhs) const { return rhs < *this; }
  bool operator<=(const fancy_pointer<const T> &rhs) const { return !(*this > rhs); }
  bool operator>=(const fancy_pointer<const T> &rhs) const { return !(*this < rhs); }

  /*
   * Pre/post increment/decrement
   */
  fancy_pointer<T> &operator++() {
    offset += sizeof(T);
    return *this;
  }

  fancy_pointer<T> operator++(int) {
    fancy_pointer tmp(*this);
    operator++();
    return tmp;
  }

  fancy_pointer<T> &operator--() {
    offset -= sizeof(T);
    return *this;
  }

  fancy_pointer<T> operator--(int) {
    fancy_pointer tmp(*this);
    operator--();
    return tmp;
  }

  /*
   * Pointer-pointer arithmetic
   */
  difference_type operator-(const fancy_pointer<const T> &rhs) const {
    if (m_id != rhs.m_id || s_id != rhs.s_id) {
      assert(false && "Error: operands have different s_id or m_id fields");
    }
    return (offset - rhs.offset) / sizeof(T);
  }

  fancy_pointer<T> operator+(const fancy_pointer<const T> &rhs) const {
    if (m_id != rhs.m_id || s_id != rhs.s_id) {
      assert(false && "Error: operands have different s_id or m_id fields");
    }
    fancy_pointer<T> tmp(*this);
    tmp.offset += rhs.offset;
    return tmp;
  }

  /*
   * Pointer-value arithmetic
   */
  fancy_pointer<T>& operator+=(const difference_type& rhs) {
    offset += rhs * sizeof(T);
    return *this;
  }

  fancy_pointer<T>& operator-=(const difference_type& rhs) {
    offset -= rhs * sizeof(T);
    return *this;
  }

  friend fancy_pointer<T> operator+(const fancy_pointer<T> &lhs, const std::size_t rhs) {
    fancy_pointer<T> tmp(lhs);
    tmp.offset += rhs * sizeof(T);
    return tmp;
  }

  friend fancy_pointer<T> operator-(const fancy_pointer<T> &lhs, const std::size_t rhs) {
    fancy_pointer<T> tmp(lhs);
    tmp.offset -= rhs * sizeof(T);
    return tmp;
  }
};

#endif //FANCY_POINTER_H
