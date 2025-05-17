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
#pragma once
#include "class.h"

extern CFClassRef CFArray;
typedef struct __CFArray* CFArrayRef;

extern size_t CFArraySize(CFArrayRef);
extern void* CFArrayGet(CFArrayRef, size_t);
extern bool CFArraySet(CFArrayRef, size_t, void*);
extern bool CFArrayPush(CFArrayRef, void*);
extern void* CFArrayLast(CFArrayRef);
extern bool CFArrayPop(CFArrayRef);
extern bool CFArrayContains(CFArrayRef, void*);
extern bool CFArrayContainsPtr(CFArrayRef, void*);
extern size_t CFArrayFind(CFArrayRef, void*);
extern size_t CFArrayFindPtr(CFArrayRef, void*);

extern proc void Clear(CFArrayRef);
extern proc void* Get(CFArrayRef, int);
extern proc void Add(CFArrayRef, void*);
extern proc void Put(CFArrayRef, int, void*);
extern proc size_t Length(CFArrayRef);

