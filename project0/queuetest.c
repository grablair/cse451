#include <stdio.h>
#include "queue.h"

// Print out the index and the value of each element.
bool show_one(queue_element* elem, queue_function_args* args) {
  printf("Item %d == %d\n", *(int*) args, *(int*) elem);
  *(int*) args = *(int*) args + 1;
  return true;
}

int compare_two(queue_element* e1, queue_element* e2) {
  int x = *(int*) e1;
  int y = *(int*) e2;
  
  printf("Comparing %d, %d\n", x, y);
  
  if (x < y)
    return -1;
  else if (x == y)
    return 0;
  else
    return 1;
}

int main(int argc, char* argv[]) {
  queue* q = queue_create();

  int x = 4;
  int y = 3;
  int z = 3;
  int t = 3;
  queue_append(q, &x);
  queue_append(q, &y);
  queue_append(q, &z);
  queue_append(q, &t);
  printf("Queue size is %zu\n", queue_size(q));

  int index = 0;
  queue_apply(q, show_one, &index);
  
  queue_reverse(q);
  
  index = 0;
  queue_apply(q, show_one, &index);
  
  queue_sort(q, compare_two);
  
  index = 0;
  queue_apply(q, show_one, &index);
  
  return 0;
}
