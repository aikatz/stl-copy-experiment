#include <algorithm>
#include <iostream>

#include <forward_list>
#include <list>
#include <set>
#include <vector>
#include <map>
#include <unordered_set>
#include <deque>
#include <unordered_map>

#include "SequentialAllocator.h"

using namespace std;

char *region;

using PairType = pair<const int, int>;
using IntAllocType = SequentialAllocator<int>;
using PairAllocType = SequentialAllocator<PairType>;
using VectorType = vector<int, IntAllocType>;
using DequeType = deque<int, IntAllocType>;
using ForwardListType = forward_list<int, IntAllocType>;
using ListType = list<int, IntAllocType>;
using SetType = set<int, std::less<>, IntAllocType>;
using MultisetType = multiset<int, std::less<>, IntAllocType>;
using MapType = map<int, int, std::less<>, PairAllocType>;
using MultimapType = multimap<int, int, std::less<>, PairAllocType>;
using UnorderedSetType = unordered_set<int, std::hash<int>, std::equal_to<>, IntAllocType>;
using UnorderedMultisetType = unordered_multiset<int, std::hash<int>, std::equal_to<>, IntAllocType>;
using UnorderedMapType = unordered_map<int, int, std::hash<int>, std::equal_to<>, PairAllocType>;
using UnorderedMultimapType = unordered_multimap<int, int, std::hash<int>, std::equal_to<>, PairAllocType>;

void testArray() {

}

void testVector() {
  get_offset() = 0;
  SequentialAllocator<VectorType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  VectorType &container = *allocator.allocate(1);
  new(&container) VectorType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 290; i < 300; i++) {
    container.push_back(i);
  }
  printf("Testing vector\n");
  for_each(begin(container), end(container), [](int i) { cout << i << " "; });
  printf("\n\n");
  free(region);
}

void testDeque() {
  get_offset() = 0;
  SequentialAllocator<DequeType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  DequeType &container = *allocator.allocate(1);
  new(&container) DequeType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 290; i < 300; i++) {
    //auto last = end(container);
    container.push_back(i);
  }
  printf("Testing deque\n");
  for_each(begin(container), end(container), [](int i) { cout << i << " "; });
  printf("\n\n");
  free(region);
}

void testForwardList() {

}

void testList() {
  get_offset() = 0;
  SequentialAllocator<ListType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  ListType &container = *allocator.allocate(1);
  new(&container) ListType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++) {
    auto last = end(container);
    container.insert(last, i);
  }
  printf("Testing list\n");
  for_each(begin(container), end(container), [](int i) { cout << i << " "; });
  printf("\n\n");
  free(region);
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
  new(&container) SetType(i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++)
    container.insert(end(container), i);
  printf("Testing set\n");
  for_each(begin(container), end(container), [](int i) { cout << i << " "; });
  printf("\n\n");
  free(region);
}

void testMultiset() {
  get_offset() = 0;
  SequentialAllocator<MultisetType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  MultisetType &container = *allocator.allocate(1);
  less<> compare;
  new(&container) MultisetType(compare, i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 5; i++) {
    container.insert(i);
    container.insert(1);
  }
  printf("Testing multiset\n");
  for_each(begin(container), end(container), [](int i) { cout << i << " "; });
  printf("\n\n");
  free(region);
}

void testMap() {
  get_offset() = 0;
  //memset(slab_lookup_table[0][1], 0, REGION_SIZE);
  SequentialAllocator<MapType> allocator;
  SequentialAllocator<PairType> i_alloc{allocator};
  MapType &container = *allocator.allocate(1);
  less<> compare;
  new(&container) MapType(compare, i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++)
    container.insert({i, i});
  printf("Testing map\n");
  for_each(begin(container), end(container), [](PairType e) { cout << e.second << " "; });
  printf("\n\n");
  free(region);
}

void testMultimap() {
  get_offset() = 0;
  SequentialAllocator<MultimapType> allocator;
  SequentialAllocator<PairType> i_alloc{allocator};
  MultimapType &container = *allocator.allocate(1);
  less<> compare;
  new(&container) MultimapType(compare, i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 5; i++) {
    container.insert({i, i});
    container.insert({1, i});
  }
  printf("Testing multimap\n");
  for_each(begin(container), end(container), [](PairType e) { cout << e.second << " "; });
  printf("\n\n");
  free(region);
}

void testUnorderedSet() {
  get_offset() = 0;
  SequentialAllocator<UnorderedSetType> allocator;
  SequentialAllocator<int> i_alloc{allocator};
  UnorderedSetType &container = *allocator.allocate(1);
  hash<int> hashVal;
  equal_to<> eq_fn;
  new(&container) UnorderedSetType(2, hashVal, eq_fn, i_alloc);
  region = container.get_allocator().get_region();
  for (int i = 0; i < 10; i++){
    container.insert(end(container), i);
  }
  printf("Testing unordered set\n");
  for_each(begin(container), end(container), [](int i) { cout << i << " "; });
  printf("\n\n");
  free(region);
}

void testUnorderedMultiset() {
    get_offset() = 0;
    SequentialAllocator<UnorderedMultisetType> allocator;
    SequentialAllocator<int> i_alloc{allocator};
    UnorderedMultisetType &container = *allocator.allocate(1);
    hash<int> hashVal;
    equal_to<> eq_fn;
    
    //breaks for all bucket values except 3 when inserting same value multiple times?
    new(&container) UnorderedMultisetType(10, hashVal, eq_fn, i_alloc);
    region = container.get_allocator().get_region();
    for (int i = 0; i < 5; i++){
        container.insert(end(container), i);
        container.insert(end(container), 6);
    }
    printf("Testing unordered multiset\n");
    for_each(begin(container), end(container), [](int i) { cout << i << " "; });
    printf("\n\n");
    free(region);
}

void testUnorderedMap() {
    get_offset() = 0;
    SequentialAllocator<UnorderedMapType> allocator;
    SequentialAllocator<PairType> i_alloc{allocator};
    UnorderedMapType &container = *allocator.allocate(1);
    hash<int> hashVal;
    equal_to<> eq_fn;
    new(&container) UnorderedMapType(2, hashVal, eq_fn, i_alloc);
    region = container.get_allocator().get_region();
    for (int i = 0; i < 10; i++){
        container.insert({i, i});
    }
    printf("Testing unordered map\n");
    for_each(begin(container), end(container), [](PairType e) { cout << e.second << " "; });
    printf("\n\n");
    free(region);
}

void testUnorderedMultimap() {
    get_offset() = 0;
    SequentialAllocator<UnorderedMultimapType> allocator;
    SequentialAllocator<PairType> i_alloc{allocator};
    UnorderedMultimapType &container = *allocator.allocate(1);
    hash<int> hashVal;
    equal_to<> eq_fn;
    new(&container) UnorderedMultimapType(3, hashVal, eq_fn, i_alloc);
    region = container.get_allocator().get_region();
    for (int i = 0; i < 5; i++){
        container.insert({i, i});
        container.insert({1, 1});
    }
    printf("Testing unordered multimap\n");
    for_each(begin(container), end(container), [](PairType e) { cout << e.second << " "; });
    printf("\n\n");
    free(region);
}

int main() {
  testFancyPointer();
  testArray();
  testVector();
  testDeque();
  testForwardList();
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
