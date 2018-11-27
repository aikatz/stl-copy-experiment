#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "SequentialAllocator.h"

using namespace std;

int main() {
  // Make a set, fill it with 0..9
  using container_t = set<int, less<int>, SequentialAllocator<int>>;
  container_t *my_container = new container_t();
  for (int i = 0; i < 10; i++)
    my_container->insert(i);

  // Get the allocator and the region it uses
  using allocator_t = container_t::allocator_type;
  const allocator_t &alloc = my_container->get_allocator();
  auto original_region = alloc.get_region();

  // Allocate a new region and copy the original into it
  uintptr_t *new_region;
  posix_memalign((void **) &new_region, getpagesize(), REGION_SIZE);
  memcpy(new_region, original_region, REGION_SIZE);

  // Rewrite the pointers inside the new region that point to the original region
  for (int i = 0; i < REGION_SIZE; i++)
    if (alloc.address_was_allocated(new_region[i]))
      new_region[i] = (uintptr_t) new_region + ((uintptr_t) new_region[i] - (uintptr_t) original_region);

  // Rewrite the structure's internal pointers that point to the original region
  for (int i = 0; i < sizeof(*my_container); i++)
    if (alloc.address_was_allocated(((uintptr_t *) my_container)[i]))
      ((uintptr_t *) my_container)[i] =
        (uintptr_t) new_region + (((uintptr_t *) my_container)[i] - (uintptr_t) original_region);

  // Zero and free the original version
  memset(original_region, 0, REGION_SIZE);
  free(original_region);

  // Print the set to make sure things still work. Should print 0..9
  for (auto e : *my_container) {
    cout << e << " ";
  }
  cout << endl;

  free(new_region);
  delete my_container;
}
