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
#include <unordered_set>

#include "config.h"
#include "SequentialAllocator.h"

using namespace std;

char* region;

typedef pair<const int, int>                                      PairType;
typedef vector<int, SequentialAllocator<int>>                     VectorType;
typedef forward_list<int, SequentialAllocator<int>>               ForwardListType;
typedef list<int, SequentialAllocator<int>>                       ListType;
typedef set<int, std::less<>, SequentialAllocator<int>>           SetType;
typedef multiset<int, std::less<>, SequentialAllocator<int>>      MultisetType;
typedef map<int, int, std::less<>, SequentialAllocator<PairType>> MapType;
typedef multimap<int, int, std::less<>, SequentialAllocator<PairType>> MultimapType;

typedef unordered_set<int, std::hash<int>, std::equal_to<int>, SequentialAllocator<int>>    UnorderedSetType;

void testArray() {

}

void testVector() {
/*  SequentialAllocator<VectorType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  VectorType &container = *allocator.allocate(1);
  new (&container) VectorType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++) {
    auto last = end(container);
    container.insert(last, i);
  }
  printf("Testing list\n");
  for_each(begin(container), end(container), [](int i){cout << i << " ";});
  printf("\n\n");
  */
}

void testDeque() {

}

void testForwardList() {

}

void testList() {
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
    get_offset() = 0;
    SequentialAllocator<MultisetType> allocator;
    SequentialAllocator<int> i_alloc{allocator};
    MultisetType &container = *allocator.allocate(1);
    less<> compare;
    new (&container) MultisetType(compare, i_alloc);
    region = container.get_allocator().get_region();
    for (int i = 0; i < 5; i++){
        container.insert(i);
        container.insert(1);
    }
    printf("Testing multiset\n");
    for_each(begin(container), end(container), [](int i){cout << i << " ";});
    printf("\n\n");
}

void testMap() {
  get_offset() = 0;
  //memset(slab_lookup_table[0][1], 0, REGION_SIZE);
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
    get_offset() = 0;
    SequentialAllocator<MultimapType> allocator;
    SequentialAllocator<PairType> i_alloc{allocator};
    MultimapType &container = *allocator.allocate(1);
    less<> compare;
    new (&container) MultimapType(compare, i_alloc);
    region = container.get_allocator().get_region();
    for (int i = 0; i < 5; i++){
        container.insert({i, i});
        container.insert({1, i});
    }
    printf("Testing multimap\n");
    for_each(begin(container), end(container), [](PairType e){cout << e.second << " ";});
    printf("\n\n");
    
}

void testUnorderedSet() {
    get_offset() = 0;
    SequentialAllocator<UnorderedSetType> allocator;
    SequentialAllocator<int> i_alloc{allocator};
    UnorderedSetType &container = *allocator.allocate(1);
    hash<int> hashVal;
    equal_to<int> eq_fn;
    new (&container) UnorderedSetType(hashVal, eq_fn, i_alloc);
    region = container.get_allocator().get_region();
    for (int i = 0; i < 10; i++)
        container.insert(end(container), i);
    printf("Testing unordered set\n");
    for_each(begin(container), end(container), [](int i){cout << i << " ";});
    printf("\n\n");
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