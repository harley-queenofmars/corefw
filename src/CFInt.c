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
#include "CFObject.h"
#include "CFInt.h"

/**
 * @brief Represents an integer object in the Core Framework.
 *
 * The __CFInt structure encapsulates an integer value along with
 * a base object (__CFObject) for use within the framework's object system.
 *
 * @var __CFObject obj
 *      The base object structure, providing common object functionality.
 * @var intmax_t value
 *      The integer value stored in this object.
 */
typedef struct __CFInt 
{
	__CFObject 	obj;
	intmax_t 	value;
} __CFInt;

classD(CFInt);


/**
 * @brief Constructor function for CFInt objects.
 *
 * Initializes a CFInt object by extracting an intmax_t value from the provided
 * variable argument list and assigning it to the object's value field.
 *
 * @param ptr Pointer to the CFInt object to initialize.
 * @param args Variable argument list containing the intmax_t value.
 * @return true if initialization is successful.
 */
static bool ctor(void *ptr, va_list args)
{
	CFIntRef this = ptr;

	this->value = va_arg(args, intmax_t);

	return true;
}

/**
 * @brief Compares two CFInt objects for equality.
 *
 * This function checks if the second object is of type CFInt and,
 * if so, compares the integer values of both objects.
 *
 * @param ptr1 Pointer to the first CFInt object.
 * @param ptr2 Pointer to the second object to compare, expected to be a CFInt.
 * @return true if both objects are CFInt and their values are equal, false otherwise.
 */
static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFIntRef int1, int2;

	if (obj2->cls != CFInt)
		return false;

	int1 = ptr1;
	int2 = ptr2;

	return (int1->value == int2->value);
}

/**
 * @brief Computes a hash value for a CFIntRef object.
 *
 * This function takes a pointer to a CFIntRef structure and returns its
 * integer value cast to a uint32_t as the hash value.
 *
 * @param ptr Pointer to a CFIntRef object.
 * @return uint32_t Hash value derived from the integer's value.
 */
static uint32_t hash(void *ptr)
{
	CFIntRef this = ptr;

	return (uint32_t)this->value;
}

/**
 * @brief Creates a copy of the given pointer by increasing its reference count.
 *
 * This function calls CFRef on the provided pointer, which is assumed to
 * increment the reference count of the object pointed to by ptr. It returns
 * the same pointer after incrementing its reference count.
 *
 * @param ptr Pointer to the object whose reference count should be incremented.
 * @return The same pointer after its reference count has been incremented.
 */
static void* copy(void *ptr)
{
	return CFRef(ptr);
}

/**
 * @brief Retrieves the value stored in a CFIntRef integer object.
 *
 * This function returns the integer value contained within the given
 * CFIntRef structure.
 *
 * @param integer A pointer to a CFIntRef structure whose value is to be retrieved.
 * @return The integer value stored in the CFIntRef as an intmax_t.
 */
intmax_t CFIntValue(CFIntRef this)
{
	return this->value;
}

/**
 * @brief Creates a new CFInt object with the specified integer value.
 *
 * This function allocates and initializes a new CFInt object,
 * setting its value to the provided integer.
 *
 * @param value The integer value to assign to the new CFInt object.
 * @return A reference to the newly created CFInt object.
 */
proc CFIntRef NewInt(intmax_t value)
{
        return CFNew(CFInt, value);        
}
