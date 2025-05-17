/*
 * Copyright (c) 2012, Jonathan Schleifer <js@webkeks.org>
 * Copyright (c) 2025, darkoverlordofdata
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

#include "object.h"
#include "bag.h"
#include "hash.h"
#include "cfstring.h"

/**
 * Collection type a bit like ArrayList but does not preserve the order of its
 * entities, speedwise it is very good, especially suited for games.
 */
struct __CFBag {
    struct __CFObject obj;
    void **data;
    size_t length;
    size_t size;
};

/**
 * Constructs an empty Bag with the specified initial capacity.
 * Constructs an empty Bag with an initial capacity of 64.
 *
 * @constructor
 * @param capacity the initial capacity of Bag
 */
static bool
ctor(void *ptr, va_list args)
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

static void
dtor(void *ptr)
{
    CFBagRef this = ptr;
    size_t i;

    for (i = 0; i < this->size; i++)
        CFUnref(this->data[i]);

    if (this->data != nullptr)
        free(this->data);
}

static bool
equal(void *ptr1, void *ptr2)
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

static uint32_t
hash(void *ptr)
{
    CFBagRef this = ptr;
    uint32_t hash;

    CF_HASH_INIT(hash);

    for (size_t i = 0; i < this->size; i++)
        CF_HASH_ADD_HASH(hash, CFHash(this->data[i]));

    CF_HASH_FINALIZE(hash);

    return hash;
}

static void*
copy(void *ptr)
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
 * Removes the element at the specified position in this Bag. does this by
 * overwriting it was last element then removing last element
 *
 * @param index
 *            the index of element to be removed
 * @return {Object} element that was removed from the Bag
 */
void* CFBagRemoveAt(CFBagRef this, size_t index)
{
    CFObjectRef e = this->data[index];              // make copy of element to remove so it can be returned
    this->data[index] = this->data[--this->size];   // overwrite item to remove with last element
    this->data[this->size] = nullptr;                  // null last element, so gc can do its work
    return e;
}

/**
 * Removes the first occurrence of the specified element from this Bag, if
 * it is present. If the Bag does not contain the element, it is unchanged.
 * does this by overwriting it was last element then removing last element
 *
 * @param e
 *            element to be removed from this list, if present
 * @return {boolean} true if this list contained the specified element
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
 * Remove and return the last object in the bag.
 *
 * @return {Object} the last object in the bag, null if empty.
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
 * Check if bag contains this element.
 *
 * @param e
 * @return {boolean}
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
 * Removes from this Bag all of its elements that are contained in the
 * specified Bag.
 *
 * @param bag
 *            Bag containing elements to be removed from this Bag
 * @return {boolean} true if this Bag changed as a result of the call
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
 * Returns the element at the specified position in Bag.
 *
 * @param index
 *            index of the element to return
 * @return {Object} the element at the specified position in bag
 */
void* CFBagGet(CFBagRef this, size_t index)
{
    if (index >= this->length) {
        return nullptr;
    }
    return this->data[index];
}

/**
 * Returns the element at the specified position in Bag. This proc
 * ensures that the bag grows if the requested index is outside the bounds
 * of the current backing array.
 *
 * @param index
 *      index of the element to return
 *
 * @return {Object} the element at the specified position in bag
 *
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
 * Returns the number of elements in this bag.
 *
 * @return {number} the number of elements in this bag
 */
size_t CFBagSize(CFBagRef this)
{
    return this->size;
}

/**
 * Returns the number of elements the bag can hold without growing.
 *
 * @return {number} the number of elements the bag can hold without growing.
 */
size_t CFBagGetCapacity(CFBagRef this)
{
    return this->length;
}

/**
 * Checks if the internal storage supports this index.
 *
 * @param index
 * @return {boolean}
 */
bool CFBagIsIndexWithinBounds(CFBagRef this, size_t index)
{
    return index <= this->length;
}

/**
 * Returns true if this list contains no elements.
 *
 * @return {boolean} true if this list contains no elements
 */
bool CFBagIsEmpty(CFBagRef this)
{
    return this->size == 0;
}

/**
 * Adds the specified element to the end of this bag. if needed also
 * increases the capacity of the bag.
 *
 * @param e
 *            element to be added to this list
 */
void CFBagAdd(CFBagRef this, void* e)
{
    // is size greater than capacity increase capacity
    if (this->size == this->length) {
        CFBagGrow(this, 0);
    }
    this->data[this->size++] = e;
}

/**
 * Set element at specified index in the bag.
 *
 * @param index position of element
 * @param e the element
 */
void CFBagSet(CFBagRef this, size_t index, void* e)
{
    if (index >= this->length) {
        CFBagGrow(this, index*2);
    }
    this->size = index+1;
    this->data[index] = e;
}

void CFBagGrow(CFBagRef this, size_t capacity)
{
    if (capacity == 0) {
        capacity = ((this->length * 3) / 2) + 1;
    }
    this->length = capacity;
    this->data = realloc(this->data, sizeof(void*) * (this->length + 1));


}

void CFBagEnsureCapacity(CFBagRef this, size_t index)
{
    if (index >= this->length) {
        CFBagGrow(this, index*2);
    }
}

void CFBagClear(CFBagRef this)
{
    for (size_t i = 0; i<this->size; i++) {
        this->data[i] = nullptr;
    }
    this->size = 0;
}

void CFBagAddAll(CFBagRef this, CFBagRef items)
{
    for (size_t i = 0; i<CFBagSize(items); i++) {
        CFBagAdd(this, CFBagGet(items, i));
    }
}


static struct __CFClass class = {
    .name = "CFBag",
    .size = sizeof(struct __CFBag),
    .ctor = ctor,
    .dtor = dtor,
    .equal = equal,
    .hash = hash,
    .copy = copy
};
CFClassRef CFBag = &class;
