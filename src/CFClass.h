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
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#define class(x) static __CFClass class = {.name = #x,.size = sizeof(__##x)};CFClassRef x = &class;
#define class2(x) static __CFClass class = {.name = #x,.size = sizeof(__##x),.dtor=&dtor};CFClassRef x = &class;
#define class3(x) static __CFClass class = {.name = #x,.size = sizeof(__##x),.ctor=ctor,.dtor=&dtor};CFClassRef x = &class;
#define classD(x) static __CFClass class = {.name = #x,.size = sizeof(__##x),.ctor = ctor,.equal = equal,.hash = hash,.copy = copy };CFClassRef x = &class;
#define classF(x) static __CFClass class = {.name = #x,.size = sizeof(__##x),.ctor = ctor,.dtor = dtor,.equal = equal,.hash = hash,.copy = copy };CFClassRef x = &class;


#define proc __attribute__((overloadable))

/**
 * @typedef CFClassRef
 * @brief Opaque reference to a CFClass structure.
 *
 * This typedef defines CFClassRef as a pointer to an incomplete type (__CFClass),
 * providing encapsulation and abstraction for CFClass objects.
 * The actual structure definition is hidden from the user to enforce usage
 * through the provided API functions.
 */
typedef struct __CFClass* CFClassRef;

/**
 * @struct __CFClass
 * @brief Represents a class descriptor for a custom object system.
 *
 * This structure defines the metadata and function pointers required to manage
 * objects in a custom class framework, including construction, destruction,
 * comparison, hashing, and copying.
 *
 * @var __CFClass::name
 *   The name of the class.
 * @var __CFClass::size
 *   The size (in bytes) of the class instance.
 * @var __CFClass::ctor
 *   Pointer to the constructor function. Should initialize an instance with a variable argument list.
 * @var __CFClass::dtor
 *   Pointer to the destructor function. Should clean up an instance.
 * @var __CFClass::equal
 *   Pointer to a function that compares two instances for equality.
 * @var __CFClass::hash
 *   Pointer to a function that computes a hash value for an instance.
 * @var __CFClass::copy
 *   Pointer to a function that creates a copy of an instance.
 */
typedef struct __CFClass 
{
	const char	*name;
	size_t 		size;
	bool 		(*ctor)(void*, va_list);
	void 		(*dtor)(void*);
	bool 		(*equal)(void*, void*);
	uint32_t 	(*hash)(void*);
	void* 		(*copy)(void*);
} __CFClass;

extern const char* CFClassName(CFClassRef);

