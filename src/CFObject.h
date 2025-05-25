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
#include "CFClass.h"

typedef struct __CFObject* CFObjectRef;

/**
 * @struct __CFObject
 * @brief Base structure for CoreFW objects, providing class reference and reference counting.
 *
 * This structure is used as the base for all CoreFW objects. It contains a reference
 * to the object's class (`CFClassRef cls`) and an integer reference count (`int ref_cnt`)
 * for memory management and object lifecycle control.
 *
 * Members:
 *   CFClassRef cls   - Pointer to the class information for this object.
 *   int ref_cnt      - Reference count for managing the object's lifetime.
 */
typedef struct __CFObject 
{
	CFClassRef 	cls;
	int 		ref_cnt;
} __CFObject;

extern CFClassRef CFObject;
extern void* CFNew(CFClassRef, ...);
extern void* CFCreate(CFClassRef, ...);
extern void* CFRef(void*);
extern void CFUnref(void*);
extern void CFFree(void*);
extern CFClassRef CFClass(void*);
extern bool CFIs(void*, CFClassRef);
extern bool CFEqual(void*, void*);
extern uint32_t CFHash(void*);
extern void* CFCopy(void*);

static bool ctor(void *ptr, va_list args);
static void dtor(void *ptr);
static bool equal(void *ptr1, void *ptr2);
static uint32_t hash(void *ptr);
static void* copy(void *ptr);
