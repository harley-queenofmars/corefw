/*
 * Copyright (c) 2012, Jonathan Schleifer <js@webkeks.org>
 * Copyright (c) 2018 Dark Overlord of Data <darkoverlordofdata@gmail.com>
 * Copyright (c) 2025 harley davidson <harley.queenofmars@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>

#include "CFObject.h"
#include "CFMap.h"
#include "CFHash.h" 		// IWYU pragma: keep
#include "CFString.h"

/**
 * @brief Static instance representing a deleted bucket in a hash map.
 *
 * This structure is used as a sentinel value to mark buckets that have been deleted
 * in the hash map implementation. It contains:
 * - `key`: Pointer to the key object (set to nullptr for deleted).
 * - `obj`: Pointer to the value object (set to nullptr for deleted).
 * - `hash`: Hash value of the key (set to 0 for deleted).
 *
 * The `deleted` instance helps distinguish between empty and previously occupied
 * (but now deleted) buckets during probing and rehashing operations.
 */
static struct bucket 
{
	CFObjectRef 	key;
	CFObjectRef 	obj;
	uint32_t 		hash;
} deleted = { nullptr, nullptr, 0 };

/**
 * @brief Represents a hash map structure.
 *
 * This structure defines a hash map with the following members:
 * - obj:      Base object for common functionality.
 * - data:     Pointer to an array of bucket pointers, representing the hash table.
 * - size:     Number of buckets in the hash table.
 * - items:    Current number of items stored in the map.
 */
typedef struct __CFMap 
{
	__CFObject 		obj;
	struct bucket** data;
	uint32_t 		size;
	size_t 			items;
} __CFMap;

classF(CFMap);

/**
 * @brief Retrieves the value associated with the specified key from the map.
 *
 * This function returns a pointer to the value stored in the map for the given key.
 *
 * @param this Pointer to the CFMap instance.
 * @param key The key whose associated value is to be retrieved.
 * @return void* Pointer to the value associated with the key, or NULL if the key is not found.
 */
proc void* Get(CFMapRef this, char* key)
{
        return CFMapGet(this, key);
}

/**
 * @brief Removes the entry associated with the specified key from the map.
 *
 * This function removes the key-value pair from the given CFMapRef by setting
 * the value associated with the key to nullptr.
 *
 * @param this Pointer to the CFMapRef map object.
 * @param key The key whose entry is to be removed.
 * @return true if the entry was successfully removed, false otherwise.
 */
proc bool Remove(CFMapRef this, char* key)
{
        return CFMapSet(this, key, nullptr);
}

/**
 * @brief Inserts or updates an entry in the CFMap.
 *
 * This function associates the specified key with the given object in the provided CFMapRef.
 * If the key already exists in the map, its value is updated to the new object.
 *
 * @param this   Pointer to the CFMapRef where the key-object pair will be stored.
 * @param key    The key string to associate with the object.
 * @param object Pointer to the object to be stored in the map.
 */
proc void Put(CFMapRef this, char* key, void* object)
{
        CFMapSet(this, key, object);
}


/**
 * @brief Iterates over all key-item pairs in the given CFMap and applies a user-provided function to each pair.
 *
 * This function initializes an iterator for the specified CFMap and, for each entry in the map,
 * calls the provided function pointer with the key and item as arguments.
 *
 * @param this Pointer to the CFMap to iterate over.
 * @param func Function pointer to apply to each key-item pair. The function should accept two void pointers:
 *             the first for the key, and the second for the item.
 */
proc void ForEach(CFMapRef this, void(*func)(void* key, void* item))
{
     CFMapIter_t iter;

        CFMapIter(this, &iter);
        while (iter.key != nullptr) {
                func(iter.key, iter.obj);
                CFMapIterNext(&iter);
        }
}

/**
 * @brief Constructor function for CFMap objects.
 *
 * Initializes a CFMap instance pointed to by `ptr` using a variable argument list.
 * The argument list should contain pairs of (key, value) pointers, terminated by a NULL key.
 * For each (key, value) pair, the function attempts to insert them into the map using CFMapSet.
 * If any insertion fails, the function returns false.
 *
 * @param ptr Pointer to the CFMap object to initialize.
 * @param args Variable argument list containing (key, value) pairs, terminated by a NULL key.
 * @return true if all pairs are successfully inserted; false otherwise.
 */
static bool ctor(void *ptr, va_list args)
{
	CFMapRef map = ptr;
	void *key;

	map->data = nullptr;
	map->size = 0;
	map->items = 0;

	while ((key = va_arg(args, void*)) != nullptr)
		if (!CFMapSet(map, key, va_arg(args, void*)))
			return false;

	return true;
}

/**
 * @brief Destructor function for CFMap objects.
 *
 * This function releases all resources associated with a CFMap instance.
 * It iterates through the map's data array, unreferencing and freeing each
 * non-null and non-deleted entry. Finally, it frees the data array itself.
 *
 * @param ptr Pointer to the CFMap object to be destroyed.
 */
static void dtor(void *ptr)
{
	CFMapRef map = ptr;
	uint32_t i;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != nullptr && map->data[i] != &deleted) {
			CFUnref(map->data[i]->key);
			CFUnref(map->data[i]->obj);
			free(map->data[i]);
		}
	}

	if (map->data != nullptr)
		free(map->data);
}

/**
 * @brief Compares two CFMap objects for equality.
 *
 * This static function checks if two CFMap objects are equal by:
 * - Verifying that the second object is of type CFMap.
 * - Comparing the number of items in both maps.
 * - Iterating through the entries of the first map and ensuring that for each
 *   non-null, non-deleted entry, the corresponding key in the second map maps
 *   to an equal object (using CFEqual).
 *
 * @param ptr1 Pointer to the first CFMap object.
 * @param ptr2 Pointer to the second CFMap object.
 * @return true if the two CFMap objects are equal, false otherwise.
 */
static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFMapRef map1, map2;
	uint32_t i;

	if (obj2->cls != CFMap)
		return false;

	map1 = ptr1;
	map2 = ptr2;

	if (map1->items != map2->items)
		return false;

	for (i = 0; i < map1->size; i++)
		if (map1->data[i] != nullptr && map1->data[i] != &deleted &&
		        !CFEqual(CFMapGet(map2, map1->data[i]->key),
		        map1->data[i]->obj))
			return false;

	return true;
}

/**
 * @brief Computes a hash value for the given CFMap.
 *
 * This function iterates over all entries in the map and accumulates their hash values.
 * Only entries that are not null and not marked as deleted are considered.
 * The final hash is the sum of the individual entry hashes and the hashes of their associated objects.
 *
 * @param ptr Pointer to the CFMapRef structure to hash.
 * @return The computed hash value as a 32-bit unsigned integer.
 */
static uint32_t hash(void *ptr)
{
	CFMapRef map = ptr;
	uint32_t i, hash = 0;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != nullptr && map->data[i] != &deleted) {
			hash += map->data[i]->hash;
			hash += CFHash(map->data[i]->obj);
		}
	}

	return hash;
}

/**
 * @brief Creates a deep copy of a CFMap object.
 *
 * This function allocates and initializes a new CFMap structure,
 * copying all buckets from the source map. Each bucket's key and object
 * are also referenced (using CFRef), ensuring that the new map is a
 * deep copy of the original. If any allocation fails, the function
 * returns nullptr.
 *
 * @param ptr Pointer to the source CFMap object to copy.
 * @return Pointer to the newly created CFMap copy, or nullptr on failure.
 */
static void* copy(void *ptr)
{
	CFMapRef map = ptr;
	CFMapRef new;
	uint32_t i;

	if ((new = CFNew(CFMap, (void*)nullptr)) == nullptr)
		return nullptr;

	if ((new->data = malloc(sizeof(*new->data) * map->size)) == nullptr)
		return nullptr;
	new->size = map->size;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != nullptr && map->data[i] != &deleted) {
			struct bucket *bucket;

			if ((bucket = malloc(sizeof(*bucket))) == nullptr)
				return nullptr;

			bucket->key = CFRef(map->data[i]->key);
			bucket->obj = CFRef(map->data[i]->obj);
			bucket->hash = map->data[i]->hash;

			new->data[i] = bucket;
		} else
			new->data[i] = nullptr;
	}

	return new;
}

/**
 * @brief Resizes the hash map to accommodate a different number of items.
 *
 * This function adjusts the size of the hash map's internal bucket array based on the
 * current number of items. If the map is too full (>= 75%), it doubles the size.
 * If the map is too empty (<= 25%), it halves the size. Otherwise, it does nothing.
 * The function rehashes and relocates all existing entries into the new bucket array.
 *
 * @param map    Pointer to the CFMap structure to resize.
 * @param items  The current number of items in the map.
 * @return true if resizing was successful or not needed, false on allocation failure or invalid input.
 */
bool resize(CFMapRef map, uint32_t items)
{
	size_t fullness = items * 4 / map->size;
	struct bucket **ndata;
	uint32_t i, nsize;

	if (items > UINT32_MAX)
		return false;

	if (fullness >= 3)
		nsize = map->size << 1;
	else if (fullness <= 1)
		nsize = map->size >> 1;
	else
		return true;

	if (nsize == 0)
		return false;

	if ((ndata = malloc(nsize * sizeof(*ndata))) == nullptr)
		return false;

	for (i = 0; i < nsize; i++)
		ndata[i] = nullptr;

	for (i = 0; i < map->size; i++) {
		if (map->data[i] != nullptr && map->data[i] != &deleted) {
			uint32_t j, last;

			last = nsize;

			j = map->data[i]->hash & (nsize - 1);
			for (; j < last && ndata[j] != nullptr; j++);

			/* In case the last bucket is already used */
			if (j >= last) {
				last = map->data[i]->hash & (nsize - 1);

				for (j = 0; j < last && ndata[j] != nullptr; j++);
			}

			if (j >= last) {
				free(ndata);
				return false;
			}

			ndata[j] = map->data[i];
		}
	}

	free(map->data);
	map->data = ndata;
	map->size = nsize;

	return true;
}

/**
 * @brief Returns the number of items in the given map.
 *
 * This function retrieves the current number of items stored in the specified CFMap.
 *
 * @param map A reference to the CFMap whose size is to be determined.
 * @return The number of items in the map.
 */
size_t CFMapSize(CFMapRef map)
{
	return map->items;
}

/**
 * Retrieves the value associated with the specified key from the given map.
 *
 * @param map Pointer to the CFMap structure from which to retrieve the value.
 * @param key Pointer to the key whose associated value is to be returned.
 * @return Pointer to the value associated with the specified key, or nullptr if the key is not found or if the key is nullptr.
 *
 * The function uses open addressing with linear probing to search for the key.
 * It skips over deleted entries and wraps around the hash table if necessary.
 */
void* CFMapGet(CFMapRef map, void *key)
{
	uint32_t i, hash, last;

	if (key == nullptr)
		return nullptr;

	hash = CFHash(key);
	last = map->size;

	for (i = hash & (map->size - 1);
	        i < last && map->data[i] != nullptr; i++) {
		if (map->data[i] == &deleted)
			continue;

		if (CFEqual(map->data[i]->key, key))
			return map->data[i]->obj;
	}

	if (i < last)
		return nullptr;

	/* In case the last bucket is already used */
	last = hash & (map->size - 1);

	for (i = 0; i < last && map->data[i] != nullptr; i++) {
		if (map->data[i] == &deleted)
			continue;

		if (CFEqual(map->data[i]->key, key))
			return map->data[i]->obj;
	}

	return nullptr;
}

/**
 * Retrieves the value associated with the specified C string key from the given CFMap.
 *
 * @param map A reference to the CFMap from which to retrieve the value.
 * @param key The C string key whose associated value is to be retrieved.
 * @return A pointer to the value associated with the key, or nullptr if the key is not found
 *         or if memory allocation for the key fails.
 */
void* CFMapGetC(CFMapRef map, const char *key)
{
	CFStringRef str;
	void *ret;

	if ((str = CFNew(CFString, key)) == nullptr)
		return nullptr;

	ret = CFMapGet(map, str);

	CFUnref(str);

	return ret;
}

/**
 * @brief Inserts or updates a key-value pair in the CFMap.
 *
 * This function sets the value associated with the specified key in the given CFMap.
 * If the key already exists, its value is updated. If the key does not exist, a new
 * entry is created. If the value (`obj`) is `nullptr`, the key is removed from the map.
 *
 * The function handles hash collisions using open addressing and resizes the map
 * as needed to accommodate new entries. Memory management for keys and values is
 * handled via CFCopy, CFRef, and CFUnref functions.
 *
 * @param map Pointer to the CFMap structure.
 * @param key Pointer to the key to insert or update. Must not be `nullptr`.
 * @param obj Pointer to the value to associate with the key. If `nullptr`, the key is removed.
 * @return true if the operation was successful, false otherwise (e.g., memory allocation failure).
 */
bool CFMapSet(CFMapRef map, void *key, void *obj)
{
	uint32_t i, hash, last;

	if (key == nullptr)
		return false;

	if (map->data == nullptr) {
		if ((map->data = malloc(sizeof(*map->data))) == nullptr)
			return false;

		map->data[0] = nullptr;
		map->size = 1;
		map->items = 0;
	}

	hash = CFHash(key);
	last = map->size;

	for (i = hash & (map->size - 1);
	        i < last && map->data[i] != nullptr; i++) {
		if (map->data[i] == &deleted)
			continue;

		if (CFEqual(map->data[i]->key, key))
			break;
	}

	/* In case the last bucket is already used */
	if (i >= last) {
		last = hash & (map->size - 1);

		for (i = 0; i < last && map->data[i] != nullptr; i++) {
			if (map->data[i] == &deleted)
				continue;

			if (CFEqual(map->data[i]->key, key))
				break;
		}
	}

	/* Key not in dictionary */
	if (i >= last || map->data[i] == nullptr || map->data[i] == &deleted ||
	        !CFEqual(map->data[i]->key, key)) {
		struct bucket *bucket;

		if (obj == nullptr)
			return true;

		if (!resize(map, map->items + 1))
			return false;

		last = map->size;

		for (i = hash & (map->size - 1); i < last &&
		        map->data[i] != nullptr && map->data[i] != &deleted; i++);

		/* In case the last bucket is already used */
		if (i >= last) {
			last = hash & (map->size - 1);

			for (i = 0; i < last && map->data[i] != nullptr &&
			        map->data[i] != &deleted; i++);
		}

		if (i >= last)
			return false;

		if ((bucket = malloc(sizeof(*bucket))) == nullptr)
			return false;

		if ((bucket->key = CFCopy(key)) == nullptr) {
			free(bucket);
			return false;
		}

		bucket->obj = CFRef(obj);
		bucket->hash = CFHash(key);

		map->data[i] = bucket;
		map->items++;

		return true;
	}

	if (obj != nullptr) {
		void *old = map->data[i]->obj;
		map->data[i]->obj = CFRef(obj);
		CFUnref(old);
	} else {
		CFUnref(map->data[i]->key);
		CFUnref(map->data[i]->obj);

		free(map->data[i]);
		map->data[i] = &deleted;

		map->items--;

		if (!resize(map, map->items))
			return false;
	}

	return true;
}

/**
 * @brief Sets a key-value pair in the specified CFMap using a C string as the key.
 *
 * This function creates a new CFString from the provided C string key,
 * inserts the key-value pair into the map, and then releases the created string.
 *
 * @param map Pointer to the CFMap in which to set the key-value pair.
 * @param key Null-terminated C string to be used as the key.
 * @param obj Pointer to the value/object to associate with the key.
 * @return true if the key-value pair was successfully set, false otherwise.
 */
bool CFMapSetC(CFMapRef map, const char *key, void *obj)
{
	CFStringRef str;
	bool ret;

	if ((str = CFNew(CFString, key)) == nullptr)
		return false;

	ret = CFMapSet(map, str, obj);

	CFUnref(str);

	return ret;
}

/**
 * @brief Initializes a CFMap iterator to the beginning of the map.
 *
 * This function sets up the provided iterator to iterate over the given map,
 * starting from the first element. It also advances the iterator to the first
 * valid entry in the map, if any.
 *
 * @param map  Pointer to the CFMap to iterate over.
 * @param iter Pointer to the CFMapIter_t structure to initialize.
 */
void CFMapIter(CFMapRef map, CFMapIter_t *iter)
{
	iter->_map = map;
	iter->_pos = 0;

	CFMapIterNext(iter);
}

/**
 * @brief Advances the iterator to the next valid entry in the map.
 *
 * This function updates the given CFMapIter_t iterator to point to the next
 * valid (non-null, non-deleted) entry in the associated map. If a valid entry
 * is found, the iterator's key and obj fields are updated accordingly and the
 * internal position is advanced. If no more valid entries are found, the key
 * and obj fields are set to nullptr.
 *
 * @param iter Pointer to the CFMapIter_t iterator to advance.
 */
void CFMapIterNext(CFMapIter_t *iter)
{
	CFMapRef map = iter->_map;

	for (; iter->_pos < map->size &&
	        (map->data[iter->_pos] == nullptr ||
	        map->data[iter->_pos] == &deleted); iter->_pos++);

	if (iter->_pos < map->size) {
		iter->key = map->data[iter->_pos]->key;
		iter->obj = map->data[iter->_pos]->obj;
		iter->_pos++;
	} else {
		iter->key = nullptr;
		iter->obj = nullptr;
	}
}

