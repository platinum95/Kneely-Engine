#pragma once

//Hashtable element structure
typedef struct hash_elem_t {
	struct hash_elem_t* next; // Next element in case of a collision
	unsigned int data;	// Pointer to the stored element
	char key[]; 	// Key of the stored element
} hash_elem_t;

//Hashtabe structure
typedef struct {
	unsigned int capacity;	// Hashtable capacity (in terms of hashed keys)
	unsigned int e_num;	// Number of element currently stored in the hashtable
	hash_elem_t** table;	// The table containaing elements
} hashtable_t;

//Structure used for iterations
typedef struct {
	hashtable_t* ht; 	// The hashtable on which we iterate
	unsigned int index;	// Current index in the table
	hash_elem_t* elem; 	// Curent element in the list
} hash_elem_it;

hashtable_t* ht_create(unsigned int capacity);

unsigned int ht_put(hashtable_t* hasht, char* key, unsigned int data);
unsigned int ht_get(hashtable_t* hasht, char* key);
unsigned int ht_remove(hashtable_t* hasht, char* key);
hash_elem_t* ht_iterate(hash_elem_it* iterator);
char* ht_iterate_keys(hash_elem_it* iterator);
void ht_clear(hashtable_t* hasht);
void ht_destroy(hashtable_t* hasht);
