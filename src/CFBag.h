/*
 * Copyright (c) 2012, Jonathan Schleifer <js@webkeks.org>
 * Copyright (c) 2025, darkoverlordofdata
 * Copyright (c) 2025 harley davidson <harley.queenofmars@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
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

typedef struct __CFBag* CFBagRef;
extern CFClassRef CFBag;
extern size_t CFBagSize(CFBagRef);
extern void* CFBagGet(CFBagRef, size_t);
extern void* CFBagSafeGet(CFBagRef, size_t);
extern bool CFBagIsEmpty(CFBagRef);
extern bool CFBagIsIndexWithinBounds(CFBagRef, size_t);
extern bool CFBagContains(CFBagRef, void*);
extern void* CFBagRemoveAt(CFBagRef, size_t);
extern bool CFBagRemove(CFBagRef, void*);
extern void* CFBagRemoveLast(CFBagRef);
extern bool CFBagRemoveAll(CFBagRef, CFBagRef);
extern size_t CFBagGetCapacity(CFBagRef);
extern void CFBagSet(CFBagRef, size_t, void*);
extern void CFBagAdd(CFBagRef, void*);
extern void CFBagGrow(CFBagRef, size_t);
extern void CFBagEnsureCapacity(CFBagRef, size_t);
extern void CFBagClear(CFBagRef);
extern void CFBagAddAll(CFBagRef, CFBagRef);



