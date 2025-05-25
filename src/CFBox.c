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
#include "CFBox.h"

/**
 * @struct __CFBox
 * @brief A generic container structure for holding a pointer and associated metadata.
 *
 * This structure encapsulates an object pointer along with its type information and
 * a flag indicating whether the memory should be freed. It is typically used for
 * managing dynamically allocated resources with type safety and ownership semantics.
 *
 * @var __CFObject obj
 *      The base object for inheritance or reference counting.
 * @var void* ptr
 *      Pointer to the contained data or resource.
 * @var uint32_t type
 *      Type identifier for the contained data, used for type safety.
 * @var bool free
 *      Indicates whether the memory pointed to by 'ptr' should be freed when the box is destroyed.
 */
typedef struct __CFBox 
{
	__CFObject 	obj;
	void*		ptr;
	uint32_t 	type;
	bool 		free;
} __CFBox;

class3(CFBox);

static bool ctor(void *ptr, va_list args)
{
	CFBoxRef box = ptr;

	box->ptr = va_arg(args, void*);
	box->type = va_arg(args, uint32_t);
	box->free = va_arg(args, int);

	return true;
}

static void dtor(void *ptr)
{
	CFBoxRef box = ptr;

	if (box->free)
		free(box->ptr);
}

void* CFBoxPtr(CFBoxRef box)
{
	return box->ptr;
}

uint32_t CFBoxType(CFBoxRef box)
{
	return box->type;
}

