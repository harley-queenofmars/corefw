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
#include "CFBool.h"

/**
 * @struct __CFBool
 * @brief Represents a boolean value within the Core Framework.
 *
 * This structure encapsulates a boolean value along with a base object,
 * allowing it to be used in contexts where Core Framework objects are required.
 *
 * @var __CFBool::obj
 *      The base object structure, providing common object functionality.
 * @var __CFBool::value
 *      The boolean value represented by this structure.
 */
typedef struct __CFBool 
{
	__CFObject 	obj;
	bool 		value;
} __CFBool;

classD(CFBool);


/**
 * @brief Constructor function for CFBool objects.
 *
 * Initializes a CFBool object by setting its value from the provided argument list.
 *
 * @param ptr Pointer to the CFBool object to initialize.
 * @param args Variable argument list containing the value to assign.
 * @return true if initialization was successful.
 */
static bool ctor(void *ptr, va_list args)
{
	CFBoolRef boolean = ptr;

	boolean->value = va_arg(args, int);

	return true;
}

/**
 * @brief Compares two CFBool objects for equality.
 *
 * This static function checks if the second object is of type CFBool,
 * and if so, compares their boolean values for equality.
 *
 * @param ptr1 Pointer to the first CFBool object.
 * @param ptr2 Pointer to the second object, expected to be a CFBool.
 * @return true if both objects are CFBool and their values are equal, false otherwise.
 */
static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFBoolRef boolean1, boolean2;

	if (obj2->cls != CFBool)
		return false;

	boolean1 = ptr1;
	boolean2 = ptr2;

	return (boolean1->value == boolean2->value);
}

/**
 * @brief Computes a hash value for a CFBoolRef object.
 *
 * This function takes a pointer to a CFBoolRef structure and returns
 * a 32-bit unsigned integer hash based on the boolean value stored in the structure.
 *
 * @param ptr Pointer to a CFBoolRef object.
 * @return uint32_t Hash value derived from the boolean's value.
 */
static uint32_t hash(void *ptr)
{
	CFBoolRef boolean = ptr;

	return (uint32_t)boolean->value;
}

/**
 * @brief Creates a copy of the given pointer by increasing its reference count.
 *
 * This function calls CFRef on the provided pointer, which is assumed to
 * increment the reference count of the object pointed to by ptr. It returns
 * the same pointer after incrementing the reference count.
 *
 * @param ptr Pointer to the object to be copied (reference counted).
 * @return The same pointer after its reference count has been incremented.
 */
static void* copy(void *ptr)
{
	return CFRef(ptr);
}

/**
 * @brief Returns the boolean value stored in a CFBoolRef object.
 *
 * @param boolean A pointer to a CFBoolRef structure.
 * @return The boolean value contained in the CFBoolRef.
 */
bool CFBoolValue(CFBoolRef boolean)
{
	return boolean->value;
}

