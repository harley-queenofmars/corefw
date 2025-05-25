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
#include <assert.h>

#include "CFObject.h"
#include "CFRefPool.h"

/**
 * @struct __CFRefPool
 * @brief Represents a reference pool for managing collections of pointers.
 *
 * This structure is used to manage a pool of references (pointers), allowing for efficient
 * allocation and deallocation of objects. It supports linking to previous and next pools,
 * enabling the creation of a doubly-linked list of reference pools.
 *
 * @var __CFObject obj
 *      Base object structure for reference counting or type identification.
 * @var void** data
 *      Array of pointers managed by this reference pool.
 * @var size_t size
 *      Number of pointers currently stored in the pool.
 * @var CFRefPoolRef prev
 *      Pointer to the previous reference pool in the linked list.
 * @var CFRefPoolRef next
 *      Pointer to the next reference pool in the linked list.
 */
typedef struct __CFRefPool {
	__CFObject 		obj;
	void**			data;
	size_t 			size;
	CFRefPoolRef 	prev;
	CFRefPoolRef 	next;
} __CFRefPool;

class3(CFRefPool);

static CFRefPoolRef top;

/**
 * @brief Constructor function for CFRefPoolRef objects.
 *
 * Initializes a CFRefPoolRef instance. Currently, this function simply returns
 * the provided instance without performing any additional initialization.
 *
 * @param this Pointer to the CFRefPoolRef instance to initialize.
 * @return The same CFRefPoolRef pointer passed as input.
 */
CFRefPoolRef proc Ctor(CFRefPoolRef this)
{	
	// (CFRefPoolRef)this;
	return this;
}

/**
 * @brief Constructor function for CFRefPool objects.
 *
 * Initializes a new reference pool object, setting its data pointer to nullptr and size to 0.
 * Links the new pool into a global doubly-linked list of pools, updating the 'top' pointer.
 *
 * @param ptr Pointer to the memory allocated for the CFRefPool object.
 * @param args Variable argument list (unused).
 * @return true if initialization was successful, false otherwise.
 */
static bool ctor(void *ptr, va_list args)
{
	CFRefPoolRef pool = ptr;
	(void)args;

	pool->data = nullptr;
	pool->size = 0;

	if (top != nullptr) {
		pool->prev = top;
		top->next = pool;
	} else
		pool->prev = nullptr;
	pool->next = nullptr;

	top = pool;

	return true;
}

/**
 * @brief Destructor function for CFRefPoolRef objects.
 *
 * This function releases all resources associated with a CFRefPoolRef pool.
 * It performs the following actions:
 * - Unreferences the next pool in the chain, if it exists.
 * - Unreferences all objects stored in the pool's data array.
 * - Frees the memory allocated for the data array.
 * - Updates the global 'top' pointer to the previous pool in the chain.
 * - Ensures the new top pool's 'next' pointer is set to nullptr.
 *
 * @param ptr Pointer to the CFRefPoolRef object to be destroyed.
 */
static void dtor(void *ptr)
{
	CFRefPoolRef pool = ptr;
	size_t i;

	if (pool->next != nullptr)
		CFUnref(pool->next);

	for (i = 0; i < pool->size; i++)
		CFUnref(pool->data[i]);

	if (pool->data != nullptr)
		free(pool->data);

	top = pool->prev;

	if (top != nullptr)
		top->next = nullptr;
}

/**
 * @brief Adds a pointer to the current reference pool.
 *
 * This function adds the given pointer to the top reference pool's data array.
 * It reallocates memory for the data array if necessary to accommodate the new pointer.
 *
 * @param ptr Pointer to be added to the reference pool.
 * @return true if the pointer was successfully added; false if memory allocation failed.
 *
 * @note The function asserts that the top reference pool is not null.
 *       The function updates the pool's data array and increments its size.
 */
bool CFRefPoolAdd(void *ptr)
{
	void **ndata;

	assert(top != nullptr);

	if (top->data != nullptr)
		ndata = realloc(top->data, (top->size + 1) * sizeof(void*));
	else
		ndata = malloc((top->size + 1) * sizeof(void*));

	if (ndata == nullptr)
		return false;

	ndata[top->size++] = ptr;

	top->data = ndata;

	return true;
}

