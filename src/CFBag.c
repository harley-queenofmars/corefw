/*
 * Copyright (c) 2012, Jonathan Schleifer <js@webkeks.org>
 * Copyright (c) 2025, darkoverlordofdata
 * Copyright (c) 2025 harley davidson <harley.queenofmars@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
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

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "CFObject.h"
#include "CFBag.h"
#include "CFHash.h"
// #include "CFString.h"

/**
 * @struct __CFBag
 * @brief Represents a generic bag (multiset) data structure.
 *
 * This structure is used to store a collection of elements, allowing duplicate entries.
 *
 * @var __CFBag::obj
 *   Base object structure, possibly for reference counting or type identification.
 * @var __CFBag::data
 *   Pointer to an array of void pointers, each pointing to an element in the bag.
 * @var __CFBag::length
 *   The current number of elements stored in the bag.
 * @var __CFBag::size
 *   The total allocated capacity of the bag (number of elements it can hold before resizing).
 */
typedef struct __CFBag {
    struct __CFObject obj;
    void **data;
    size_t length;
    size_t size;
} __CFBag;

classF(CFBag);

// static struct __CFClass class = {
//     .name = "CFBag",
//     .size = sizeof(struct __CFBag),
//     .ctor = ctor,
//     .dtor = dtor,
//     .equal = equal,
//     .hash = hash,
//     .copy = copy
// };
// CFClassRef CFBag = &class;


/**
 * @brief Constructor function for initializing a CFBag object.
 *
 * This function initializes the internal data structure of a CFBag object.
 * It sets the initial size to 0, retrieves the desired initial length from
 * the variable argument list, and allocates memory for the data array.
 * If the provided length is 0, a default length of 64 is used.
 *
 * @param ptr Pointer to the CFBag object to be initialized.
 * @param args Variable argument list containing the initial length (size_t).
 * @return true if initialization is successful.
 */
static bool ctor(void *ptr, va_list args)
{
    CFBagRef this = ptr;
    void *obj;

    this->data = nullptr;
    this->size = 0;
    this->length = va_arg(args, size_t);
    if (this->length == 0) this->length = 64;
    this->data = malloc(sizeof(void*) * this->length); 
    return true;
}

/**
 * @brief Destructor function for a CFBag object.
 *
 * This function releases all resources held by the CFBag instance pointed to by `ptr`.
 * It iterates through the bag's data array, calling `CFUnref` on each element to
 * decrement their reference counts or free them as appropriate. Finally, it frees
 * the memory allocated for the data array itself if it is not null.
 *
 * @param ptr Pointer to the CFBag instance to be destroyed.
 */
static void dtor(void *ptr)
{
    CFBagRef this = ptr;
    size_t i;

    for (i = 0; i < this->size; i++)
        CFUnref(this->data[i]);

    if (this->data != nullptr)
        free(this->data);
}

/**
 * @brief Compares two CFBag objects for equality.
 *
 * This function checks if the two provided pointers refer to CFBag objects of the same class and size.
 * It then iterates through the elements of both bags and compares each corresponding element using CFEqual.
 * 
 * @param ptr1 Pointer to the first CFBag object.
 * @param ptr2 Pointer to the second CFBag object.
 * @return true if both CFBag objects are equal, false otherwise.
 */
static bool equal(void *ptr1, void *ptr2)
{
    CFObjectRef obj2 = ptr2;
    if (obj2->cls != CFBag)
        return false;

    CFBagRef this = ptr1;
    CFBagRef that = ptr2;

    if (this->size != that->size)
        return false;

    for (size_t i = 0; i < this->size; i++)
        if (CFEqual(this->data[i], that->data[i]))
            return false;

    return true;
}

/**
 * @brief Computes a hash value for a CFBagRef object.
 *
 * This function initializes a hash value, iterates over all elements in the bag,
 * and combines their individual hashes into a single hash value using the
 * CF_HASH_* macros. The final hash value is then returned.
 *
 * @param ptr Pointer to a CFBagRef object whose hash is to be computed.
 * @return uint32_t The computed hash value for the bag.
 */
static uint32_t hash(void *ptr)
{
    CFBagRef this = ptr;
    uint32_t hash;

    CF_HASH_INIT(hash);

    for (size_t i = 0; i < this->size; i++)
        CF_HASH_ADD_HASH(hash, CFHash(this->data[i]));

    CF_HASH_FINALIZE(hash);

    return hash;
}

/**
 * @brief Creates a deep copy of a CFBag object.
 *
 * This function allocates and returns a new CFBag object that is a deep copy of the input CFBag.
 * It duplicates the internal data array and increments the reference count for each element.
 *
 * @param ptr Pointer to the CFBag object to copy.
 * @return Pointer to the newly created CFBag copy, or nullptr on failure.
 */
static void* copy(void *ptr)
{
    CFBagRef this = ptr;
    CFBagRef new;

    if ((new = CFNew(CFBag, (void*)nullptr)) == nullptr)
        return nullptr;

    if ((new->data = malloc(sizeof(void*) * this->size)) == nullptr) {
        CFUnref(new);
        return nullptr;
    }
    new->size = this->size;

    for (size_t i = 0; i < this->size; i++)
        new->data[i] = CFRef(this->data[i]);

    return new;
}


/**
 * Removes and returns the element at the specified index from the bag.
 *
 * The element at the given index is replaced with the last element in the bag,
 * and the bag's size is decremented. The last element's slot is set to nullptr
 * to allow for garbage collection.
 *
 * @param this  Pointer to the CFBag instance.
 * @param index Index of the element to remove.
 * @return      Pointer to the removed element.
 */
void* CFBagRemoveAt(CFBagRef this, size_t index)
{
    CFObjectRef e = this->data[index];              // make copy of element to remove so it can be returned
    this->data[index] = this->data[--this->size];   // overwrite item to remove with last element
    this->data[this->size] = nullptr;                  // null last element, so gc can do its work
    return e;
}

/**
 * @brief Removes an element from the bag.
 *
 * Searches for the specified element in the bag and removes it if found.
 * The removal is performed by replacing the found element with the last element
 * in the bag, reducing the bag's size by one, and setting the now-unused slot to nullptr.
 *
 * @param this Pointer to the CFBag instance.
 * @param e Pointer to the element to be removed.
 * @return true if the element was found and removed; false otherwise.
 */
bool CFBagRemove(CFBagRef this, void* e)
{
    for (size_t i = 0; i<this->size; i++) {
        if (CFEqual(e, this->data[i])) {
            this->data[i] = this->data[--this->size];
            this->data[this->size] = nullptr;
            return true;
        }
    }
    return false;
}

/**
 * Removes and returns the last object from the bag.
 *
 * @return Pointer to the last object in the bag, or NULL if the bag is empty.
 */
void* CFBagRemoveLast(CFBagRef this)
{
    if (this->size>0) {
        CFObjectRef e = this->data[--this->size];
        this->data[this->size] = nullptr;
        return e;
    }
    return nullptr;
}

/**
 * @brief Checks if the specified element exists in the bag.
 *
 * Iterates through the elements of the bag and compares each element
 * with the provided element using the CFEqual function. Returns true
 * if the element is found, otherwise returns false.
 *
 * @param this Pointer to the CFBag instance.
 * @param e Pointer to the element to search for in the bag.
 * @return true if the element exists in the bag, false otherwise.
 */
bool CFBagContains(CFBagRef this, void* e)
{
    for (size_t i = 0; i<this->size; i++) {
        if (CFEqual(e, this->data[i])) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Removes all elements from 'this' bag that are also present in the 'bag' parameter.
 *
 * Iterates through each element in the provided 'bag' and removes matching elements from 'this' bag.
 * If any elements are removed, the function returns true; otherwise, it returns false.
 *
 * @param this Pointer to the CFBagRef from which elements will be removed.
 * @param bag Pointer to the CFBagRef containing elements to be removed from 'this'.
 * @return true if any elements were removed from 'this', false otherwise.
 */
bool CFBagRemoveAll(CFBagRef this, CFBagRef bag)
{
    bool modified = false;

    for (size_t i = 0; i<bag->size; i++) {
        for (size_t j=0; j<this->size; j++) {
            if (CFEqual(bag->data[j], this->data[i])) {
                CFBagRemoveAt(this, j);
                j--;
                modified = true;
                break;
            }
        }
    }
    return modified;
}

/**
 * Retrieves the element at the specified index from the CFBag.
 *
 * @param this  Pointer to the CFBag instance.
 * @param index Index of the element to retrieve.
 * @return      Pointer to the element at the specified index, or nullptr if the index is out of bounds.
 */
void* CFBagGet(CFBagRef this, size_t index)
{
    if (index >= this->length) {
        return nullptr;
    }
    return this->data[index];
}

 
/**
 * Retrieves the element at the specified index from the CFBag.
 * 
 * If the index is out of bounds (greater than or equal to the current length),
 * the function attempts to grow the bag to accommodate the index and returns nullptr.
 * Otherwise, it returns the element at the given index.
 *
 * @param this  Pointer to the CFBag instance.
 * @param index Index of the element to retrieve.
 * @return      Pointer to the element at the given index, or nullptr if the index is out of bounds.
 */
void* CFBagSafeGet(CFBagRef this, size_t index)
{
    if (index >= this->length) {
        CFBagGrow(this, (index * 7) / 4 + 1);
        return nullptr;
    }
    return this->data[index];
}

/**
 * @brief Returns the number of elements in the specified CFBag.
 *
 * @param this Pointer to the CFBag instance.
 * @return The number of elements currently stored in the bag.
 */
size_t CFBagSize(CFBagRef this)
{
    return this->size;
}

/**
 * @brief Returns the capacity of the specified CFBag.
 *
 * This function retrieves the total number of elements that the given CFBag
 * can hold without requiring a resize or reallocation.
 *
 * @param this A reference to the CFBag instance.
 * @return The capacity (maximum number of elements) of the CFBag.
 */
size_t CFBagGetCapacity(CFBagRef this)
{
    return this->length;
}

/**
 * Checks if the internal storage supports the specified index.
 *
 * This function determines whether the provided index is within the valid range
 * of the internal storage. It returns true if the index is supported, false otherwise.
 *
 * @param index The index to check for support in the internal storage.
 * @return true if the index is supported; false otherwise.
 */
bool CFBagIsIndexWithinBounds(CFBagRef this, size_t index)
{
    return index <= this->length;
}


/**
 * @brief Checks if the bag is empty.
 *
 * This function returns true if the bag contains no elements, i.e., its size is zero.
 *
 * @param this Pointer to the CFBag instance.
 * @return true if the bag is empty, false otherwise.
 */
bool CFBagIsEmpty(CFBagRef this)
{
    return this->size == 0;
}

/**
 * @brief Adds an element to the bag.
 *
 * If the bag is full, it grows the internal storage before adding the new element.
 *
 * @param this Pointer to the CFBag instance.
 * @param e Pointer to the element to be added.
 */
void CFBagSet(CFBagRef this, size_t index, void* e)
{
    if (index >= this->length) {
        CFBagGrow(this, index*2);
    }
    this->size = index+1;
    this->data[index] = e;
}

/**
 * @brief Grows the internal storage of the CFBag to the specified capacity.
 *
 * If the provided capacity is zero, the function calculates a new capacity
 * based on the current length, increasing it by 50% plus one.
 * The internal data array is then reallocated to accommodate the new capacity.
 *
 * @param this Pointer to the CFBag instance to grow.
 * @param capacity The desired new capacity. If zero, a new capacity is calculated.
 */
void CFBagGrow(CFBagRef this, size_t capacity)
{
    if (capacity == 0) {
        capacity = ((this->length * 3) / 2) + 1;
    }
    this->length = capacity;
    this->data = realloc(this->data, sizeof(void*) * (this->length + 1));


}

/**
 * @brief Ensures that the CFBag has enough capacity to accommodate the specified index.
 *
 * If the provided index is greater than or equal to the current length of the bag,
 * this function will grow the bag's capacity to at least twice the index value.
 *
 * @param this  Pointer to the CFBag instance.
 * @param index The index that needs to be accommodated.
 */
void CFBagEnsureCapacity(CFBagRef this, size_t index)
{
    if (index >= this->length) {
        CFBagGrow(this, index*2);
    }
}

/**
 * @brief Clears all elements from the CFBag.
 *
 * This function sets all elements in the bag's data array to nullptr and resets the size to zero,
 * effectively removing all items from the bag.
 *
 * @param this Pointer to the CFBag instance to be cleared.
 */
void CFBagClear(CFBagRef this)
{
    for (size_t i = 0; i<this->size; i++) {
        this->data[i] = nullptr;
    }
    this->size = 0;
}

/**
 * @brief Adds all elements from one CFBag to another.
 *
 * This function iterates over all elements in the source CFBag (`items`)
 * and adds each element to the destination CFBag (`this`) using CFBagAdd.
 *
 * @param this  The destination CFBag to which elements will be added.
 * @param items The source CFBag containing elements to add.
 */
void CFBagAddAll(CFBagRef this, CFBagRef items)
{
    for (size_t i = 0; i<CFBagSize(items); i++) {
        CFBagAdd(this, CFBagGet(items, i));
    }
}


