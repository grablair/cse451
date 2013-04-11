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
  hash_entry    **buckets;    // A pointer to an array of buckets.
  hash_hasher   *hasher;
  hash_compare  *hc;
};

// Dictates the default capacity of a hash table.
const uint32_t DEFAULT_CAPACITY = 64;

// Prototype for the rehash function.
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
                                               sizeof(table->buckets));

  if (!table->buckets) {
    free(table);
    return NULL;
  }

  return table;
}


void hash_insert(hash_table* ht, void* key, void* value,
                 void** removed_key_ptr, void** removed_value_ptr) {
  // If the ratio of elements to buckets is > 3/4, rehash.
  if (0.75 < ((double) ht->num_elements / ht->num_buckets))
    rehash(ht);

  uint64_t idx = ht->hasher(key);

  while (ht->buckets[idx % ht->num_buckets]) {
    hash_entry *cur = ht->buckets[idx % ht->num_buckets];

    // If the keys are equal, we found a match.
    // Lets return it.
    if (!ht->hc(cur->key, key)) {
      *removed_key_ptr = cur->key;
      *removed_value_ptr = cur->value;
      cur->key = key;
      cur->value = value;
      return;
    }

    idx++;
  }

  hash_entry *entry = (hash_entry *) malloc(sizeof(hash_entry));

  // In case malloc failed.
  assert(entry);

  // Set new entry and add to table.
  entry->key = key;
  entry->value = value;

  ht->buckets[idx % ht->num_buckets] = entry;
  ht->num_elements++;
}


bool hash_lookup(hash_table* ht, const void* key, void** value_ptr) {
  uint64_t idx = ht->hasher(key);

  while (ht->buckets[idx % ht->num_buckets]) {
    hash_entry *cur = ht->buckets[idx % ht->num_buckets];

    // If the keys match, set the value return,
    // then return true.
    if (!ht->hc(cur->key, key)) {
      *value_ptr = cur->value;
      return true;
    }

    idx++;
  }
  // The key does not exist in the hash table.
  return false;
}


bool hash_is_present(hash_table* ht, const void* key) {
  void *dummy;

  // Uses code already written.
  return hash_lookup(ht, key, &dummy);
}


bool hash_remove(hash_table* ht, const void* key,
                 void** removed_key_ptr, void** removed_value_ptr) {
  uint64_t idx = ht->hasher(key);

  while (ht->buckets[idx % ht->num_buckets]) {
    hash_entry *cur = ht->buckets[idx % ht->num_buckets];

    // If the keys are equal, we set the returns and
    // remove the entry from the table.
    if (!ht->hc(cur->key, key)) {
      *removed_key_ptr = cur->key;
      *removed_value_ptr = cur->value;
      free(cur);
      ht->buckets[idx % ht->num_buckets] = NULL;
      ht->num_elements--;

      idx++;

      // We must go down the hash table and rehash until there
      // is a gap, in order to keep the table consistent.
      void *dummykey, *dummyval;
      while (ht->buckets[idx % ht->num_buckets]) {
        ht->num_elements--;
        cur = ht->buckets[idx % ht->num_buckets];
        ht->buckets[idx % ht->num_buckets] = NULL;
        hash_insert(ht, cur->key, cur->value, &dummykey, &dummyval);
        free(cur);

        idx++;
      }

      return true;
    }

    idx++;
  }
  // The key does not exist in the table.
  return false;
}


void hash_destroy(hash_table* ht, bool free_keys, bool free_values) {
  // Go through all the buckets, freeing everything on the way.
  for (int i = 0; i < ht->num_buckets; i++) {
    hash_entry *cur = ht->buckets[i];

    if (cur) {
      if (free_keys)    free(cur->key);
      if (free_values)  free(cur->value);
      free(cur);
    }
  }

  // Free the bucket addresses, and the table itself.
  free(ht->buckets);
  free(ht);
}

// Private:
void rehash(hash_table* ht) {
  hash_entry **oldbuckets = ht->buckets;
  ht->num_buckets = ht->num_buckets * 2;
  ht->num_elements = 0;
  ht->buckets = (hash_entry **) calloc(ht->num_buckets,
                                       sizeof(oldbuckets));

  // Go through the old buckets and re-add all the entries
  // that exist, freeing the old entry struct.
  void *dummykey, *dummyval;
  for (int i = 0; i < ht->num_buckets / 2; i++) {
    hash_entry *cur = oldbuckets[i];

    if (cur) {
      hash_insert(ht, cur->key, cur->value, &dummykey, &dummyval);
      free(cur);
    }
  }

  // Free the address array.
  free(oldbuckets);
}
