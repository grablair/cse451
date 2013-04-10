/* Implements queue abstract data type. */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* Each link in the queue stores a queue_element and
 * a pointer to the next link in the queue. */
typedef struct _queue_link {
  queue_element* elem;
  struct _queue_link* next;
} queue_link;

/* This is the actual implementation of the queue struct that
 * is declared in queue.h. */
struct _queue {
  queue_link* head;
};

queue* queue_create() {
  queue* q = (queue*) malloc(sizeof(queue));

  if (!q) {
    // Failed to allocate memory.
    return NULL;
  }

  q->head = NULL;
  return q;
}

/* Private */
static queue_link* queue_new_element(queue_element* elem) {
  queue_link* ql = (queue_link*) malloc(sizeof(queue_link));

  if (!ql) {
    // Failed to allocate memory.
    return NULL;
  }

  ql->elem = elem;
  ql->next = NULL;

  return ql;
}

void queue_append(queue* q, queue_element* elem) {
  assert(q != NULL);

  if (!q->head) {
    q->head = queue_new_element(elem);
    return;
  }

  // Find the last link in the queue.
  queue_link* cur;
  for (cur = q->head; cur->next; cur = cur->next) {}

  // Append the new link.
  cur->next = queue_new_element(elem);
}

bool queue_remove(queue* q, queue_element** elem_ptr) {
  queue_link* old_head;

  assert(q != NULL);
  assert(elem_ptr != NULL);
  if (queue_is_empty(q)) {
    return false;
  }

  *elem_ptr = q->head->elem;
  old_head = q->head;
  q->head = q->head->next;

  free(old_head);

  return true;
}

bool queue_is_empty(queue* q) {
  assert(q != NULL);
  return q->head == NULL;
}

/* private */
static bool queue_count_one(queue_element* elem, queue_function_args* args) {
  size_t* count = (size_t*) args;
  *count = *count + 1;
  return true;
}

size_t queue_size(queue* q) {
  size_t count = 0;
  queue_apply(q, queue_count_one, &count);
  return count;
}

bool queue_apply(queue* q, queue_function qf, queue_function_args* args) {
  assert(q != NULL && qf != NULL);

  if (queue_is_empty(q))
    return false;

  for (queue_link* cur = q->head; cur; cur = cur->next) {
    if (!qf(cur->elem, args))
      break;
  }

  return true;
}

void queue_reverse(queue* q) {
  // If it's empty, nothing is reversible.
  if (queue_is_empty(q))
    return;

  // Find the last link in the queue.
  queue_link* last;
  for (last = q->head; last->next; last = last->next) {}

  // Reverse the queue.
  while (q->head != last) {
    queue_link* tmp = q->head;
    q->head = tmp->next;
    tmp->next = last->next;
    last->next = tmp;
  }
}

void queue_sort(queue* q, queue_compare qc) {
  // New head for the queue.
  queue_link *nh = q->head;

  if (!nh)
    return;

  queue_link *cur = nh->next, *curnext;
  nh->next = NULL;
  for (; cur; cur = curnext) {
    curnext = cur->next;

    if (qc(cur->elem, nh->elem) < 0) {
      // current value belongs at the beginning of the queue.
      cur->next = nh;
      nh = cur;
    } else {
      // belongs somewhere else.
      queue_link *nhcur;
      int placed = 0;

      // Cycle through the new-headed list to find the
      // correct location for the current value.
      for (nhcur = nh; nhcur->next; nhcur = nhcur->next) {
        // If the current element is the same as the
        // element we are currently at in the new list,
        // or less than the next one, we know it belongs
        // next in line.
        if (qc(cur->elem, nhcur->next->elem) < 0 ||
              !qc(cur->elem, nhcur->elem)) {
          cur->next = nhcur->next;
          nhcur->next = cur;

          nhcur = cur;
          placed = 1;

          break;
        }
      }

      if (!placed) {
        nhcur->next = cur;
        cur->next = NULL;
      }
    }
  }

  q->head = nh;
}
