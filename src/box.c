/*
 * Copyright (c) 2012, Jonathan Schleifer <js@webkeks.org>
 * Copyright (c) 2018 Dark Overlord of Data <darkoverlordofdata@gmail.com>
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
#include "object.h"
#include "box.h"

/**
 * CFBox instance variables
 */
typedef struct __CFBox 
{
	__CFObject 	obj;
	void*		ptr;
	uint32_t 	type;
	bool 		free;
} __CFBox;

/**
 * CFBox class
 */
 static __CFClass class = {
	.name = "CFBox",
	.size = sizeof(__CFBox),
	.ctor = ctor,
	.dtor = dtor
};
CFClassRef CFBox = &class;

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

