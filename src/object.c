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
#include <assert.h>

#include "object.h"
#include "refpool.h"

/**
 * CFObject class
 */
static __CFClass class = {
	.name = "CFObject",
	.size = sizeof(__CFObject),
};
CFClassRef CFObject = &class;

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

void* CFRef(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return nullptr;

	obj->ref_cnt++;

	return obj;
}

void CFUnref(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return;

	if (--obj->ref_cnt == 0)
		CFFree(obj);
}

void CFFree(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return;

	if (obj->cls->dtor != nullptr)
		obj->cls->dtor(obj);

	free(obj);
}

CFClassRef CFClass(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return nullptr;

	return obj->cls;
}

bool CFIs(void *ptr, CFClassRef cls)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr || cls == nullptr)
		return false;

	return (obj->cls == cls);
}

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

uint32_t CFHash(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return 0;

	if (obj->cls->hash != nullptr)
		return obj->cls->hash(obj);

	return (uint32_t)(uintptr_t)ptr;
}

void* CFCopy(void *ptr)
{
	CFObjectRef obj = ptr;

	if (obj == nullptr)
		return nullptr;

	if (obj->cls->copy != nullptr)
		return obj->cls->copy(obj);

	return nullptr;
}

