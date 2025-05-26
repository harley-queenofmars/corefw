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
#include <stdint.h>

#include "CFObject.h"
#include "CFArray.h"
#include "CFHash.h"

/**
 * @brief Represents a dynamic array structure.
 *
 * This structure encapsulates a dynamic array, including its metadata and data storage.
 *
 * @typedef __CFArray
 * @struct __CFArray
 * 
 * @var __CFObject obj
 *      Base object for common object functionality.
 * @var void** data
 *      Pointer to the array of elements.
 * @var size_t size
 *      Number of elements currently stored in the array.
 */
typedef struct __CFArray 
{
	__CFObject	obj;
	void**		data;
	size_t 		size;
} __CFArray;

/**
 * @brief Static instance of the __CFClass structure for CFArray objects.
 *
 * This structure defines the class metadata and function pointers for the CFArray type,
 * including its name, size, constructor, destructor, equality, hash, and copy operations.
 *
 * Members:
 * - name: Name of the class ("CFArray").
 * - size: Size of the __CFArray structure.
 * - ctor: Pointer to the constructor function.
 * - dtor: Pointer to the destructor function.
 * - equal: Pointer to the equality comparison function.
 * - hash: Pointer to the hash function.
 * - copy: Pointer to the copy function.
 */
static __CFClass class = {
	.name = "CFArray",
	.size = sizeof(__CFArray),
	.ctor = ctor,
	.dtor = dtor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};

/**
 * @brief Reference to the CFArray class object.
 *
 * This variable holds a pointer to the class structure representing the CFArray type.
 * It is typically used for type identification, object creation, and class-level operations
 * within the CoreFW framework.
 */
CFClassRef CFArray = &class;

/**
 * @brief Clears all elements from the specified CFArray.
 *
 * This function removes all elements from the given CFArrayRef, effectively resetting its size to zero.
 * The memory allocated for the array itself is not freed, only the contents are cleared.
 *
 * @param this A reference to the CFArray to be cleared.
 */
proc void Clear(CFArrayRef this)
{

	for (size_t i = 0; i < this->size; i++)
		CFUnref(this->data[i]);

	if (this->data != nullptr)
		free(this->data);
        this->size = 0;
}

/**
 * @brief Retrieves the element at the specified index from the array.
 *
 * @param this Pointer to the CFArray instance.
 * @param i Index of the element to retrieve.
 * @return Pointer to the element at the specified index.
 */
proc void* Get(CFArrayRef this, int i)
{
    return CFArrayGet(this, i);
}

/**
 * @brief Adds an item to the end of the array.
 *
 * @param this Pointer to the CFArray instance.
 * @param item Pointer to the item to add.
 */
proc void Add(CFArrayRef this, void* item)
{
    CFArrayPush(this, item);
}

/**
 * @brief Inserts or replaces an object at the specified index in the array.
 *
 * This function sets the object at the given index in the CFArray.
 *
 * @param this   Reference to the CFArray.
 * @param index  The position at which to insert or replace the object.
 * @param object Pointer to the object to be inserted.
 */
proc void Put(CFArrayRef this, int index, void* object)
{
    CFArraySet(this, index, object);
}

/**
 * @brief Returns the number of elements in the given CFArray.
 *
 * This function retrieves the current size (number of elements) of the specified CFArray.
 *
 * @param this A reference to the CFArray whose length is to be determined.
 * @return The number of elements in the array.
 */
proc size_t Length(CFArrayRef this)
{
    return CFArraySize(this);
}

/**
 * @brief Constructor function for initializing a CFArrayRef object.
 *
 * This function initializes the given CFArrayRef structure by setting its data pointer to nullptr
 * and its size to 0. It then iterates over a variable argument list, adding each object to the array
 * using CFArrayPush until a nullptr is encountered. If any insertion fails, the function returns false.
 *
 * @param ptr   Pointer to the CFArrayRef object to initialize.
 * @param args  Variable argument list containing pointers to objects to add to the array, terminated by nullptr.
 * @return true if all objects are successfully added; false otherwise.
 */
static bool ctor(void *ptr, va_list args)
{
	CFArrayRef array = ptr;
	void *obj;

	array->data = nullptr;
	array->size = 0;

	while ((obj = va_arg(args, void*)) != nullptr)
		if (!CFArrayPush(array, obj))
			return false;

	return true;
}

/**
 * @brief Destructor function for CFArrayRef objects.
 *
 * This function releases all elements stored in the array by calling CFUnref
 * on each element. After releasing the elements, it frees the memory allocated
 * for the array's data if it is not null.
 *
 * @param ptr Pointer to the CFArrayRef object to be destroyed.
 */
static void dtor(void *ptr)
{
	CFArrayRef array = ptr;
	size_t i;

	for (i = 0; i < array->size; i++)
		CFUnref(array->data[i]);

	if (array->data != nullptr)
		free(array->data);
}

/**
 * @brief Compares two CFArray objects for equality.
 *
 * This function checks whether the two provided pointers refer to CFArray objects
 * and whether their contents are equal. It first verifies that the second object
 * is of the CFArray class. Then, it compares the sizes of the arrays. If the sizes
 * match, it iterates through each element and uses CFEqual to compare corresponding
 * elements. If any pair of elements are not equal, the function returns false.
 * Otherwise, it returns true if all elements are equal.
 *
 * @param ptr1 Pointer to the first CFArray object.
 * @param ptr2 Pointer to the second object, expected to be a CFArray.
 * @return true if both arrays are of the same size and all their elements are equal; false otherwise.
 */
static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFArrayRef array1, array2;
	size_t i;

	if (obj2->cls != CFArray)
		return false;

	array1 = ptr1;
	array2 = ptr2;

	if (array1->size != array2->size)
		return false;

	for (i = 0; i < array1->size; i++)
		if (CFEqual(array1->data[i], array2->data[i]))
			return false;

	return true;
}

/**
 * @brief Computes a hash value for a CFArrayRef object.
 *
 * This function calculates a hash value for the given array by iterating over
 * each element in the array and combining their individual hash values using
 * the CF_HASH_* macros. The resulting hash is suitable for use in hash tables
 * or other data structures requiring a hash function.
 *
 * @param ptr Pointer to a CFArrayRef object whose hash is to be computed.
 * @return The computed 32-bit hash value for the array.
 */
static uint32_t hash(void *ptr)
{
	CFArrayRef array = ptr;
	size_t i;
	uint32_t hash;

	CF_HASH_INIT(hash);

	for (i = 0; i < array->size; i++)
		CF_HASH_ADD_HASH(hash, CFHash(array->data[i]));

	CF_HASH_FINALIZE(hash);

	return hash;
}

/**
 * @brief Creates a deep copy of a CFArray object.
 *
 * This function allocates and returns a new CFArray object that is a deep copy of the input array.
 * Each element in the original array is referenced using CFRef and added to the new array.
 * If memory allocation fails at any point, the function returns nullptr.
 *
 * @param ptr Pointer to the CFArray object to be copied.
 * @return Pointer to the newly created CFArray copy, or nullptr on failure.
 */
static void* copy(void *ptr)
{
	CFArrayRef array = ptr;
	CFArrayRef new;
	size_t i;

	if ((new = CFNew(CFArray, (void*)nullptr)) == nullptr)
		return nullptr;

	if ((new->data = malloc(sizeof(void*) * array->size)) == nullptr) {
		CFUnref(new);
		return nullptr;
	}
	new->size = array->size;

	for (i = 0; i < array->size; i++)
		new->data[i] = CFRef(array->data[i]);

	return new;
}

/**
 * Retrieves the element at the specified index from the given CFArray.
 *
 * @param array Pointer to the CFArray structure.
 * @param index Zero-based index of the element to retrieve.
 * @return Pointer to the element at the specified index, or nullptr if the index is out of bounds.
 */
void* CFArrayGet(CFArrayRef array, size_t index)
{
	if (index >= array->size)
		return nullptr;

	return array->data[index];
}

/**
 * @brief Returns the number of elements in the given array.
 *
 * @param array Pointer to the CFArray structure.
 * @return The number of elements currently stored in the array.
 */
size_t CFArraySize(CFArrayRef array)
{
	return array->size;
}

/**
 * @brief Sets the element at the specified index in the array to the given object pointer.
 *
 * This function replaces the object at the specified index in the array with the provided pointer.
 * It increments the reference count of the new object and decrements the reference count of the old object.
 *
 * @param array Pointer to the CFArrayRef array.
 * @param index Index at which to set the new object.
 * @param ptr Pointer to the new object to be set at the specified index.
 * @return true if the operation was successful; false if the index is out of bounds.
 */
bool CFArraySet(CFArrayRef array, size_t index, void *ptr)
{
	CFObjectRef obj = ptr;
	CFObjectRef old;

	if (index >= array->size)
		return false;

	CFRef(obj);
	old = array->data[index];
	array->data[index] = obj;
	CFUnref(old);

	return true;
}

/**
 * @brief Adds a new element to the end of the CFArray.
 *
 * This function appends the given object pointer to the array, resizing the internal
 * storage as necessary. The object is wrapped with CFRef before being stored.
 *
 * @param array Pointer to the CFArrayRef structure to which the element will be added.
 * @param ptr Pointer to the object to be added to the array.
 * @return true if the element was successfully added; false if memory allocation failed.
 */
bool CFArrayPush(CFArrayRef array, void *ptr)
{
	CFObjectRef obj = ptr;
	void **new;

	if (array->data == nullptr)
		new = malloc(sizeof(void*));
	else
		new = realloc(array->data, sizeof(void*) * (array->size + 1));

	if (new == nullptr)
		return false;

	new[array->size] = CFRef(obj);

	array->data = new;
	array->size++;

	return true;
}

/**
 * @brief Returns the last element of the given CFArray.
 *
 * This function retrieves the last element stored in the specified CFArray.
 * If the array is empty (size is 0), it returns nullptr.
 *
 * @param array Pointer to the CFArrayRef structure.
 * @return Pointer to the last element in the array, or nullptr if the array is empty.
 */
void* CFArrayLast(CFArrayRef array)
{
	if (array->size == 0)
		return nullptr;

	return array->data[array->size - 1];
}

/**
 * @brief Removes the last element from the array.
 *
 * This function pops (removes) the last element from the given CFArrayRef.
 * If the array is empty, it returns false. If the array has only one element,
 * it unreferences and frees the data, sets the data pointer to nullptr, and
 * resets the size to zero. For arrays with more than one element, it unreferences
 * the last element, reallocates the data array to a smaller size, and decrements
 * the size.
 *
 * @param array Pointer to the CFArrayRef from which to pop the last element.
 * @return true if the element was successfully removed, false otherwise.
 */
bool CFArrayPop(CFArrayRef array)
{
	void **new;
	void *last;

	if (array->size == 0)
		return false;

	if (array->size == 1) {
		CFUnref(array->data[0]);
		free(array->data);
		array->data = nullptr;
		array->size = 0;
		return true;
	}

	last = array->data[array->size - 1];

	new = realloc(array->data, sizeof(void*) * (array->size - 1));
	if (new == nullptr)
		return false;

	CFUnref(last);

	array->data = new;
	array->size--;

	return true;
}

/**
 * @brief Checks if the specified pointer exists in the given CFArray.
 *
 * Iterates through the elements of the array and compares each element
 * with the provided pointer using the CFEqual function. Returns true
 * if a match is found, otherwise returns false.
 *
 * @param array Pointer to the CFArray to search.
 * @param ptr Pointer to the element to search for in the array.
 * @return true if the element is found in the array, false otherwise.
 */
bool CFArrayContains(CFArrayRef array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (CFEqual(array->data[i], ptr))
			return true;

	return false;
}

/**
 * @brief Checks if a given pointer exists in the array.
 *
 * Iterates through the elements of the specified CFArrayRef and determines
 * whether the provided pointer is present in the array.
 *
 * @param array Pointer to the CFArrayRef to search.
 * @param ptr Pointer to search for within the array.
 * @return true if the pointer is found in the array, false otherwise.
 */
bool CFArrayContainsPtr(CFArrayRef array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (array->data[i] == ptr)
			return true;

	return false;
}

/**
 * @brief Searches for a pointer in the given CFArray and returns its index.
 *
 * Iterates through the elements of the specified CFArray and compares each element
 * with the provided pointer using the CFEqual function. If a match is found, the
 * index of the matching element is returned. If the pointer is not found, SIZE_MAX
 * is returned to indicate failure.
 *
 * @param array Pointer to the CFArray to search.
 * @param ptr Pointer to the element to find in the array.
 * @return size_t Index of the found element, or SIZE_MAX if not found.
 */
size_t CFArrayFind(CFArrayRef array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (CFEqual(array->data[i], ptr))
			return i;

	return SIZE_MAX;
}

/**
 * @brief Searches for a pointer in the array and returns its index.
 *
 * This function iterates through the elements of the given CFArrayRef array,
 * comparing each element's pointer value to the specified ptr. If a match is found,
 * the index of the matching element is returned. If the pointer is not found,
 * SIZE_MAX is returned.
 *
 * @param array Pointer to the CFArrayRef array to search.
 * @param ptr Pointer value to search for in the array.
 * @return Index of the found pointer, or SIZE_MAX if not found.
 */
size_t CFArrayFindPtr(CFArrayRef array, void *ptr)
{
	size_t i;

	for (i = 0; i < array->size; i++)
		if (array->data[i] == ptr)
			return i;

	return SIZE_MAX;
}

