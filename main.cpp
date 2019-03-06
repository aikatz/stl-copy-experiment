#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>
#include <unistd.h>
#include <functional>

#include <forward_list>
#include <list>
#include <set>
#include <vector>
#include <map>

#include "config.h"
#include "SequentialAllocator.h"

using namespace std;

char* region;

typedef pair<const int, int>                                      PairType;
typedef vector<int, SequentialAllocator<int>>                     VectorType;
typedef forward_list<int, SequentialAllocator<int>>               ForwardListType;
typedef list<int, SequentialAllocator<int>>                       ListType;
typedef set<int, std::less<>, SequentialAllocator<int>>           SetType;
typedef map<int, int, std::less<>, SequentialAllocator<PairType>> MapType;

template<class ContainerType>
void test_container_with_insert(std::string container_name) {
  using allocator_t = typename ContainerType::allocator_type;
  ContainerType my_container;

  for (int i = 0; i < 10; i++)
    if (std::is_same<ContainerType, MapType>::value)
      my_container.insert({i, i});
    else
      my_container.insert(end(my_container), i);

  printf("Testing container %s\n", container_name.c_str());
  for_each(begin(my_container), end(my_container), [](int i){cout << i << " ";});
  printf("\n\n");
}

void testArray() {

}

/*
void testVector() {
  VectorType container;
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++) {
    auto last = end(container);
    container.insert(last, i);
  }
  printf("Testing forward_list\n");
  for_each(begin(container), end(container), [](int i){cout << i << " ";});
  printf("\n\n");
}
*/

void testDeque() {

}

void testForwardList() {

}

void testList() {
  //get_offset() = 0;
  SequentialAllocator<ListType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  ListType &container = *allocator.allocate(1);
  new (&container) ListType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++) {
    auto last = end(container);
    container.insert(last, i);
  }
  printf("Testing list\n");
  for_each(begin(container), end(container), [](int i){cout << i << " ";});
  printf("\n\n");
}

void testStack() {

}

void testQueue() {

}

void testPriorityQueue() {

}

void testSet() {
  get_offset() = 0;
  SequentialAllocator<SetType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  SetType &container = *allocator.allocate(1);
  new (&container) SetType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++)
    container.insert(end(container), i);
  printf("Testing set\n");
  for_each(begin(container), end(container), [](int i){cout << i << " ";});
  printf("\n\n");
}

void testMultiset() {

}

void testMap() {
  get_offset() = 0;
  SequentialAllocator<MapType> allocator;
  SequentialAllocator<PairType> i_alloc{allocator};
  MapType &container = *allocator.allocate(1);
  less<> compare;
  new (&container) MapType(compare, i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++)
    container.insert({i, i});
  printf("Testing map\n");
  for_each(begin(container), end(container), [](PairType e){cout << e.second << " ";});
  printf("\n\n");
}

void testMultimap() {

}

void testUnorderedSet() {

}

void testUnorderedMultiset() {

}

void testUnorderedMap() {

}

void testUnorderedMultimap() {

}

int main() {
  printf("sizeof(fancy_pointer) = %lu\n", sizeof(fancy_pointer<int>));
  testArray();
  //testVector();
  testDeque();
  //testForwardList();
  testList();
  testStack();
  testQueue();
  testPriorityQueue();
  testSet();
  testMultiset();
  testMap();
  testMultimap();
  testUnorderedSet();
  testUnorderedMultiset();
  testUnorderedMap();
  testUnorderedMultimap();
}
