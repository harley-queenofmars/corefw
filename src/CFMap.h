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

/**
 * @brief A reference to a Core Foundation class object representing a map data structure.
 *
 * CFClassRef is typically used to refer to Core Foundation class types. In this context,
 * CFMap likely represents the class reference for a map (key-value store) implementation.
 * This reference can be used for type identification, object creation, and other
 * class-level operations within the Core Foundation framework.
 */
extern CFClassRef CFMap;
/**
 * @typedef CFMapRef
 * @brief Opaque reference to a CFMap structure.
 *
 * CFMapRef is a pointer to an internal CFMap structure, providing an abstract handle
 * for map (dictionary) operations. The actual structure definition is hidden to enforce
 * encapsulation and prevent direct access to internal members.
 */
typedef struct __CFMap* CFMapRef;

/**
 * @brief Iterator structure for traversing a CFMap.
 *
 * CFMapIter_t is used to iterate over the entries in a CFMap.
 * It maintains the current position and references to the key, object,
 * and the map being iterated.
 *
 * @var key
 *   Pointer to the current key in the map.
 * @var obj
 *   Pointer to the current object/value associated with the key.
 * @var _map
 *   Reference to the CFMap being iterated.
 * @var _pos
 *   Current position or index within the map.
 */
typedef struct CFMapIter_t 
{
	void*		key; 
	void*		obj;
	CFMapRef 	_map;
	uint32_t 	_pos;
} CFMapIter_t;

extern size_t CFMapSize(CFMapRef);
extern void* CFMapGet(CFMapRef, void*);
extern void* CFMapGetC(CFMapRef, const char*);
extern bool CFMapSet(CFMapRef, void*, void*);
extern bool CFMapSetC(CFMapRef, const char*, void*);
extern void CFMapIter(CFMapRef, CFMapIter_t*);
extern void CFMapIterNext(CFMapIter_t*);

extern proc void* Get(CFMapRef, char*);
extern proc bool Remove(CFMapRef, char*);
extern proc void Put(CFMapRef, char*, void*);
extern proc void ForEach(CFMapRef, void(*func)(void*, void*));

