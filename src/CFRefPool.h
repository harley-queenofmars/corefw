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
#include "CFObject.h"

/**
 * @brief An external reference to a Core Foundation class representing a reference pool.
 *
 * CFClassRef CFRefPool is typically used to manage a pool of references,
 * allowing for efficient allocation and deallocation of objects within the pool.
 * This external declaration provides access to the reference pool class
 * for use in memory management and object lifecycle control.
 *
 * @see CFClassRef
 */
extern CFClassRef CFRefPool;
/**
 * @typedef CFRefPoolRef
 * @brief Opaque pointer to a reference pool structure.
 *
 * This typedef defines CFRefPoolRef as a pointer to an incomplete type (__CFRefPool),
 * providing encapsulation for reference pool management. The actual structure
 * definition is hidden from users to enforce abstraction and prevent direct access.
 */
typedef struct __CFRefPool* CFRefPoolRef;

extern bool CFRefPoolAdd(void*);
CFRefPoolRef proc Ctor(CFRefPoolRef);

/**
 * @brief Creates and initializes a new CFRefPool instance.
 *
 * This function allocates memory for a new CFRefPool object and calls its constructor.
 *
 * @return A reference to the newly created CFRefPool instance.
 */
static inline CFRefPoolRef NewCFRefPool()
{
        return Ctor((CFRefPoolRef)CFCreate(CFRefPool));
}
