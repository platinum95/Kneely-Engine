/*
* Author : Pierre-Henri Symoneaux
*/

#include <stdlib.h>
#include <string.h>
#include "hash_table.h"


// Inititalize hashtable iterator on hashtable 'ht'
#define HT_ITERATOR(ht) {ht, 0, ht->table[0]}

char err_ptr;
unsigned int HT_ERROR = -1; // Data pointing to HT_ERROR are returned in case of error

						   /* 	Internal funcion to calculate hash for keys.
						   It's based on the DJB algorithm from Daniel J. Bernstein.
						   The key must be ended by '\0' character.*/
static unsigned int ht_calc_hash(char* key) {
	unsigned int h = 5381;
	while (*(key++))
		h = ((h << 5) + h) + (*key);
	return h;
}

/* 	Create a hashtable with capacity 'capacity'
and return a pointer to it*/
hashtable_t* ht_create(unsigned int capacity) {
	hashtable_t* hasht = (hashtable_t*) malloc(sizeof(hashtable_t));
	if (!hasht)
		return NULL;
	if ((hasht->table = (hash_elem_t**) malloc(capacity*sizeof(hash_elem_t*))) == NULL) {
		free(hasht->table);
		return NULL;
	}
	hasht->capacity = capacity;
	hasht->e_num = 0;
	unsigned int i;
	for (i = 0; i < capacity; i++)
		hasht->table[i] = NULL;
	return hasht;
}

/* 	Store data in the hashtable. If data with the same key are already stored,
they are overwritten, and return by the function. Else it return NULL.
Return HT_ERROR if there are memory alloc error*/
unsigned int ht_put(hashtable_t* hasht, char* key, unsigned int data) {
	if (data == NULL)
		return NULL;
	unsigned int h = ht_calc_hash(key) % hasht->capacity;
	hash_elem_t* e = hasht->table[h];

	while (e != NULL) {
		if (!strcmp(e->key, key)) {
			unsigned int ret = e->data;
			e->data = data;
			return ret;
		}
		e = e->next;
	}

	// Getting here means the key doesn't already exist

	if ((e = (hash_elem_t*) malloc(sizeof(hash_elem_t) + strlen(key) + 1)) == NULL)
		return HT_ERROR;
	strcpy(e->key, key);
	e->data = data;

	// Add the element at the beginning of the linked list
	e->next = hasht->table[h];
	hasht->table[h] = e;
	hasht->e_num++;

	return NULL;
}

/* Retrieve data from the hashtable */
unsigned int ht_get(hashtable_t* hasht, char* key) {
	unsigned int h = ht_calc_hash(key) % hasht->capacity;
	hash_elem_t* e = hasht->table[h];
	while (e != NULL) {
		if (!strcmp(e->key, key))
			return e->data;
		e = e->next;
	}
	return NULL;
}

/* 	Remove data from the hashtable. Return the data removed from the table
so that we can free memory if needed */
unsigned int ht_remove(hashtable_t* hasht, char* key) {
	unsigned int h = ht_calc_hash(key) % hasht->capacity;
	hash_elem_t* e = hasht->table[h];
	hash_elem_t* prev = NULL;
	while (e != NULL) {
		if (!strcmp(e->key, key)) {
			unsigned int ret = e->data;
			if (prev != NULL)
				prev->next = e->next;
			else
				hasht->table[h] = e->next;
			free(e);
			e = NULL;
			hasht->e_num--;
			return ret;
		}
		prev = e;
		e = e->next;
	}
	return NULL;
}


/* Iterate through table's elements. */
hash_elem_t* ht_iterate(hash_elem_it* iterator) {
	while (iterator->elem == NULL) {
		if (iterator->index < iterator->ht->capacity - 1) {
			iterator->index++;
			iterator->elem = iterator->ht->table[iterator->index];
		}
		else
			return NULL;
	}
	hash_elem_t* e = iterator->elem;
	if (e)
		iterator->elem = e->next;
	return e;
}

/* Iterate through keys. */
char* ht_iterate_keys(hash_elem_it* iterator) {
	hash_elem_t* e = ht_iterate(iterator);
	return (e == NULL ? NULL : e->key);
}


/* 	Removes all elements stored in the hashtable.
if free_data, all stored datas are also freed.*/
void ht_clear(hashtable_t* hasht) {
	hash_elem_it it = HT_ITERATOR(hasht);
	char* k = ht_iterate_keys(&it);
	while (k != NULL) {
		ht_remove(hasht, k);
		k = ht_iterate_keys(&it);
	}
}

/* 	Destroy the hash table, and free memory.
Data still stored are freed*/
void ht_destroy(hashtable_t* hasht) {
	ht_clear(hasht); // Delete and free all.
	free(hasht->table);
	free(hasht);
}
