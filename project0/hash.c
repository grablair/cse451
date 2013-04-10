/* Implements the abstract hash table. */

#include <assert.h>
#include <stdlib.h>

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

/* TODO: implement the functions in hash.h here! */

const uint32_t DEFAULT_CAPACITY = 64;

/* Creates and returns a new hash table. The client must define a hash
 * function and a comparison function and pass pointers to these functions
 * as arguments to this function.
 *
 * Returns: pointer to the created hash table. */
hash_table* hash_create(hash_hasher *hasher, hash_compare *hc) {
  hash_table *table = (hash_table*) malloc(sizeof(hash_table));
  
  if (!table)
    return NULL;
  
  table->hasher       = hasher;
  table->hc           = hc;
  table->num_buckets  = 64;
  table->num_elements = 0;
  table->buckets      = (hash_entry **) malloc(sizeof(hash_entry*) 
                                                * DEFAULT_CAPACITY);
  
  if (!table->buckets) {
    free(table);
    return NULL;
  }
  
  return table;
}

/* Inserts a (key, value) pair into the hash table. The implementation
 * should resize the hash table once the size / capacity ratio reaches a
 * certain threshold in order to minimize space usage and maximize speed.
 * This function takes ownership of *key and *value. After insertion, the
 * caller is permitted to modify *value, but is not permitted to change
 * *key. If a value had already been inserted into the hash table for the
 * given key, then *removed_key_ptr is set to point to the replaced key,
 * and *removed_value_ptr is set to point to the replaced value. The caller
 * is responsible for freeing the returned key and value. */
void hash_insert(hash_table* ht, void* key, void* value,
                 void** removed_key_ptr, void** removed_value_ptr) {
  uint64_t hash = ht->hasher(key);
  int idx = hash % ht->num_buckets;
  
}

/* Looks up the specified key in the hash table. If the key is found, then
 * the pointer to its value is stored in *value_ptr; the caller can then
 * directly manipulate the value that is pointed to.
 *
 * Returns: true if the key was found, false if not. */
bool hash_lookup(hash_table* ht, const void* key, void** value_ptr) {
  return 0;
}

/* Checks if a key has been inserted into the hash table.
 *
 * Returns: true if the key is present in the hash table, false if not. */
bool hash_is_present(hash_table* ht, const void* key) {
  return 0;
}

/* Removes the entry for the given key from the hash table. If the key is
 * found in the hash table, then *removed_key_ptr is set to point to the
 * key previously inserted, and *removed_value_ptr is set to point to the
 * value that was previously inserted, but the caller is responsible for
 * freeing them.
 *
 * Returns: true if the entry for the key was removed, false if not. */
bool hash_remove(hash_table* ht, const void* key,
                 void** removed_key_ptr, void** removed_value_ptr) {
  return 0;
}

/* Destroys a hash table and frees the memory used by the entries and the hash
 * table itself. If the free_values argument is true, then this function
 * will call free() on each entry's value, and similarly for free_keys. Hence
 * if the values in the entries were allocated dynamically (using malloc()),
 * then free_values should be set to true; if the values were allocated on the
 * client's stack (i.e. in local variables in a test function), then free_values
 * should be set to false, and similarly for the keys.
 */
void hash_destroy(hash_table* ht, bool free_keys, bool free_values) {
  
}
