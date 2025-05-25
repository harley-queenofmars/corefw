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
#include "CFDouble.h"

/**
 * @struct __CFDouble
 * @brief Represents a double-precision floating-point value wrapped in a Core Framework object.
 *
 * This structure encapsulates a double value along with a base object (__CFObject),
 * allowing it to be managed within the Core Framework's object system.
 *
 * @var __CFDouble::obj
 * The base object structure, providing common object functionality.
 *
 * @var __CFDouble::value
 * The double-precision floating-point value stored in this object.
 */
typedef struct __CFDouble 
{
	__CFObject 	obj;
	double 		value;
} __CFDouble;

classD(CFDouble);

/**
 * @brief Constructor function for CFDouble objects.
 *
 * Initializes a CFDouble object by extracting a double value from the provided
 * variable argument list and assigning it to the object's value field.
 *
 * @param ptr Pointer to the CFDouble object to initialize.
 * @param args Variable argument list containing the double value to assign.
 * @return true if initialization is successful.
 */
static bool ctor(void *ptr, va_list args)
{
	CFDoubleRef this = ptr;

	this->value = va_arg(args, double);

	return true;
}

/**
 * @brief Compares two CFDouble objects for equality.
 *
 * This function checks if the second object is of type CFDouble and,
 * if so, compares their double values for equality.
 *
 * @param ptr1 Pointer to the first CFDouble object.
 * @param ptr2 Pointer to the second CFDouble object.
 * @return true if both objects are CFDouble and their values are equal, false otherwise.
 */
static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFDoubleRef double1, double2;

	if (obj2->cls != CFDouble)
		return false;

	double1 = ptr1;
	double2 = ptr2;

	return (double1->value == double2->value);
}

/**
 * @brief Computes a hash value for a CFDoubleRef object.
 *
 * @param ptr Pointer to a CFDoubleRef object whose value will be hashed.
 * @return A 32-bit unsigned integer representing the hash of the object's value.
 *
 * @note The current implementation simply casts the double value to uint32_t.
 *       This is a placeholder and should be replaced with a proper hash function.
 */
static uint32_t hash(void *ptr)
{
	CFDoubleRef this = ptr;

	/* FIXME: Create a proper hash! */
	return (uint32_t)this->value;
}

/**
 * @brief Creates a copy of the given pointer by increasing its reference count.
 *
 * This function calls CFRef on the provided pointer, which is assumed to
 * increment the reference count of the object pointed to by ptr, effectively
 * creating a new reference to the same object.
 *
 * @param ptr Pointer to the object to be copied (reference incremented).
 * @return A pointer to the same object with an incremented reference count.
 */
static void* copy(void *ptr)
{
	return CFRef(ptr);
}

/**
 * @brief Creates a new CFDouble object with the specified value.
 *
 * This function allocates and initializes a new CFDouble object,
 * setting its value to the provided double.
 *
 * @param value The double value to initialize the CFDouble object with.
 * @return A reference to the newly created CFDouble object.
 */
proc CFDoubleRef NewDouble(double value)
{
	return CFNew(CFDouble, value);        
}

double CFDoubleValue(CFDoubleRef this)
{
	/**
	 * Returns the value stored in the CFDouble structure.
	 *
	 * @return The double-precision floating-point value contained in the structure.
	 */
	return this->value;
}

