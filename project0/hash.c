/* Implements the abstract hash table. */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash.h"

/* Using an entry of this type is just a suggestion. You're
 * free to change this. */
typedef struct _hash_entry {
  void* key;
  void* value;
} hash_entry;

struct _hash_table {
  size_t        num_buckets;
  size_t        num_elements;
  hash_entry    **buckets;
  hash_hasher   *hasher;
  hash_compare  *hc;
};

// Dictates the default capacity of a hash table.
const uint32_t DEFAULT_CAPACITY = 64;

void rehash(hash_table *ht);


hash_table* hash_create(hash_hasher *hasher, hash_compare *hc) {
  hash_table *table = (hash_table*) malloc(sizeof(hash_table));

  if (!table)
    return NULL;

  table->hasher       = hasher;
  table->hc           = hc;
  table->num_buckets  = 64;
  table->num_elements = 0;
  table->buckets      = (hash_entry **) calloc(DEFAULT_CAPACITY,
                                               sizeof(ht->buckets));

  if (!table->buckets) {
    free(table);
    return NULL;
  }

  return table;
}


void hash_insert(hash_table* ht, void* key, void* value,
                 void** removed_key_ptr, void** removed_value_ptr) {
  if (0.75 < ((double) ht->num_elements / ht->num_buckets))
    rehash(ht);

  uint64_t hash = ht->hasher(key) - 1;
  int idx = hash % ht->num_buckets;

  while (ht->buckets[++idx % ht->num_buckets]) {
    hash_entry *cur = ht->buckets[idx];

    if (!ht->hc(cur->key, key)) {
      *removed_key_ptr = cur->key;
      *removed_value_ptr = cur->value;
      cur->key = key;
      cur->value = value;
      return;
    }
  }

  hash_entry *entry = (hash_entry *) malloc(sizeof(hash_entry));

  assert(entry);

  entry->key = key;
  entry->value = value;

  ht->buckets[idx] = entry;
  ht->num_elements++;
}


bool hash_lookup(hash_table* ht, const void* key, void** value_ptr) {
  uint64_t hash = ht->hasher(key) - 1;
  int idx = hash % ht->num_buckets;

  while (ht->buckets[++idx % ht->num_buckets]) {
    hash_entry *cur = ht->buckets[idx];

    if (!ht->hc(cur->key, key)) {
      *value_ptr = cur->value;
      return true;
    }
  }
  return false;
}


bool hash_is_present(hash_table* ht, const void* key) {
  void *dummy;
  return hash_lookup(ht, key, &dummy);
}


bool hash_remove(hash_table* ht, const void* key,
                 void** removed_key_ptr, void** removed_value_ptr) {
  uint64_t hash = ht->hasher(key) - 1;
  int idx = hash % ht->num_buckets;

  while (ht->buckets[++idx % ht->num_buckets]) {
    hash_entry *cur = ht->buckets[idx];

    if (!ht->hc(cur->key, key)) {
      *removed_key_ptr = cur->key;
      *removed_value_ptr = cur->value;
      free(cur);
      ht->buckets[idx] = NULL;
      ht->num_elements--;

      // We must go down the hash table and rehash until there
      // is a gap
      void *dummykey, *dummyval;
      while (ht->buckets[++idx % ht->num_buckets]) {
        ht->num_elements--;
        cur = ht->buckets[idx];
        ht->buckets[idx] = NULL;
        hash_insert(ht, cur->key, cur->value, &dummykey, &dummyval);
        free(cur);
      }

      return true;
    }
  }
  return false;
}


void hash_destroy(hash_table* ht, bool free_keys, bool free_values) {
  for (int i = 0; i < ht->num_buckets; i++) {
    hash_entry *cur = ht->buckets[i];

    if (cur) {
      if (free_keys)    free(cur->key);
      if (free_values)  free(cur->value);
      free(cur);
    }
  }

  free(ht->buckets);
  free(ht);
}

void rehash(hash_table* ht) {
  hash_entry **oldbuckets = ht->buckets;
  ht->num_buckets = ht->num_buckets * 2;
  ht->num_elements = 0;
  ht->buckets = (hash_entry **) calloc(ht->num_buckets,
                                       sizeof(ht->buckets));

  void *dummykey, *dummyval;
  for (int i = 0; i < ht->num_buckets / 2; i++) {
    hash_entry *cur = oldbuckets[i];

    if (cur) {
      hash_insert(ht, cur->key, cur->value, &dummykey, &dummyval);
      free(cur);
    }
  }

  free(oldbuckets);
}

void print_all(hash_table *ht) {
  for (int i = 0; i < ht->num_buckets; i++) {
    hash_entry *cur = ht->buckets[i];

    printf("Bucket %d = ", i);

    if (cur) {
      printf("%d\n", *(int *) cur->value);
    } else {
      printf("EMPTY\n");
    }
  }
}
