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

class(CFObject);

/**
 * @brief Allocates and initializes a new object of the specified class.
 *
 * This function allocates memory for an object of the given class and initializes
 * its class pointer and reference count. If the class provides a constructor
 * (`ctor`), it is called with the provided variadic arguments. If allocation or
 * construction fails, the function returns nullptr.
 *
 * @param class Pointer to the class descriptor (CFClassRef) for the object to create.
 * @param ...   Optional arguments to pass to the class constructor, if any.
 * @return Pointer to the newly created object, or nullptr on failure.
 */
void* CFNew(CFClassRef class, ...)
{
	CFObjectRef obj;

	if ((obj = malloc(class->size)) == nullptr)
		return nullptr;

	obj->cls = class;
	obj->ref_cnt = 1;

	if (class->ctor != nullptr) {
		va_list args;
		va_start(args, class);

		if (!class->ctor(obj, args)) {
			CFUnref(obj);
			return nullptr;
		}

		va_end(args);
	}

	return obj;
}

/**
 * @brief Creates a new instance of a class with optional constructor arguments.
 *
 * Allocates memory for an object of the specified class, initializes its class
 * pointer and reference count, and calls the class constructor if provided.
 * The object is then added to the reference pool. If any step fails, the
 * function cleans up and returns nullptr.
 *
 * @param class Pointer to the class descriptor (CFClassRef) for the object to create.
 * @param ...   Optional arguments to be passed to the class constructor.
 * @return Pointer to the newly created object, or nullptr on failure.
 */
void* CFCreate(CFClassRef class, ...)
{
	CFObjectRef obj;

	assert(class != CFRefPool);

	if ((obj = malloc(class->size)) == nullptr)
		return nullptr;

	obj->cls = class;
	obj->ref_cnt = 1;

	if (class->ctor != nullptr) {
		va_list args;
		va_start(args, class);

		if (!class->ctor(obj, args)) {
			CFUnref(obj);
			return nullptr;
		}

		va_end(args);
	}

	if (!CFRefPoolAdd(obj)) {
		CFUnref(obj);
		return nullptr;
	}

	return obj;
}

/**
 * @brief Increments the reference count of a CFObjectRef.
 *
 * This function takes a pointer to a CFObjectRef, checks if it is not null,
 * and increments its reference count. If the input pointer is null, it returns null.
 *
 * @param ptr Pointer to a CFObjectRef object.
 * @return The same pointer with incremented reference count, or null if input is null.
 */
void* CFRef(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return nullptr;

	obj->ref_cnt++;

	return obj;
}

/**
 * @brief Decrements the reference count of a CFObjectRef and frees it if the count reaches zero.
 *
 * This function takes a pointer to a CFObjectRef, checks if it is not null,
 * decrements its reference count, and if the reference count becomes zero,
 * it frees the object by calling CFFree().
 *
 * @param ptr Pointer to the CFObjectRef whose reference count should be decremented.
 */
void CFUnref(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return;

	if (--obj->ref_cnt == 0)
		CFFree(obj);
}

/**
 * @brief Frees a CoreFW object and its associated resources.
 *
 * This function takes a pointer to a CoreFW object, calls its destructor
 * (if defined), and then frees the memory allocated for the object.
 *
 * @param ptr Pointer to the object to be freed. If NULL, the function does nothing.
 */
void CFFree(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return;

	if (obj->cls->dtor != nullptr)
		obj->cls->dtor(obj);

	free(obj);
}

/**
 * @brief Retrieves the class reference of a given Core Framework object.
 *
 * This function takes a pointer to a Core Framework object and returns its associated
 * class reference. If the provided pointer is null, the function returns null.
 *
 * @param ptr Pointer to a Core Framework object (CFObjectRef).
 * @return CFClassRef The class reference of the object, or null if the pointer is null.
 */
CFClassRef CFClass(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return nullptr;

	return obj->cls;
}

/**
 * @brief Checks if the given object is an instance of the specified class.
 *
 * This function verifies whether the object pointed to by `ptr` is of the class
 * referenced by `cls`. It returns `true` if the object's class matches `cls`,
 * and `false` otherwise. If either `ptr` or `cls` is `nullptr`, the function
 * returns `false`.
 *
 * @param ptr Pointer to the object to check.
 * @param cls Reference to the class to compare against.
 * @return `true` if the object is an instance of the specified class, `false` otherwise.
 */
bool CFIs(void *ptr, CFClassRef cls)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr || cls == nullptr)
		return false;

	return (obj->cls == cls);
}

/**
 * @brief Compares two CFObjectRef objects for equality.
 *
 * This function checks if the two provided pointers refer to equal objects.
 * It first checks for pointer equality, then for null pointers. If both objects
 * are non-null and their class provides an 'equal' method, it uses that method
 * to determine equality. Otherwise, it falls back to pointer comparison.
 *
 * @param ptr1 Pointer to the first object.
 * @param ptr2 Pointer to the second object.
 * @return true if the objects are considered equal, false otherwise.
 */
bool CFEqual(void *ptr1, void *ptr2)
{
	CFObjectRef obj1 = ptr1, obj2 = ptr2;

	if (obj1 == obj2)
		return true;

	if (obj1 == nullptr || obj2 == nullptr)
		return false;

	if (obj1->cls->equal != nullptr) {
		return obj1->cls->equal(obj1, obj2);
	} else
		return (obj1 == obj2);
}

/**
 * @brief Computes a hash value for a CoreFW object.
 *
 * This function returns a hash value for the given object pointer.
 * If the pointer is NULL, it returns 0. If the object's class provides
 * a custom hash function, that function is used to compute the hash.
 * Otherwise, the pointer value itself is cast to a uint32_t and returned
 * as the hash.
 *
 * @param ptr Pointer to the object to hash (CFObjectRef).
 * @return uint32_t Hash value for the object.
 */
uint32_t CFHash(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return 0;

	if (obj->cls->hash != nullptr)
		return obj->cls->hash(obj);

	return (uint32_t)(uintptr_t)ptr;
}

/**
 * @brief Creates a copy of the given Core Framework object.
 *
 * This function attempts to create a copy of the object pointed to by `ptr`.
 * It first checks if the pointer is not null, then checks if the object's class
 * provides a copy method. If so, it calls the class's copy method and returns
 * the result. If any of these checks fail, it returns nullptr.
 *
 * @param ptr Pointer to the object to be copied (must be a CFObjectRef).
 * @return void* Pointer to the copied object, or nullptr if copying is not possible.
 */
void* CFCopy(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return nullptr;

	if (obj->cls->copy != nullptr)
		return obj->cls->copy(obj);

	return nullptr;
}

