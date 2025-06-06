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
#include <unistd.h>

#include "CFClass.h"
#include "CFObject.h"
#include "CFString.h"

typedef struct __CFStream* CFStreamRef;

struct CFStreamOps {
	ssize_t (*read)(void*, void*, size_t);
	bool (*write)(void*, const void*, size_t);
	bool (*at_end)(void*);
	void (*close)(void*);
};

typedef struct __CFStream {
	__CFObject obj;
	struct CFStreamOps *ops;
	char *cache;
	size_t cache_len;
} __CFStream;

extern CFClassRef CFStream;
extern ssize_t CFStreamRead(void*, void*, size_t);
extern CFStringRef CFStreamReadLine(void*);
extern bool CFStreamWrite(void*, const void*, size_t);
extern bool CFStreamWriteString(void*, const char*);
extern bool CFStreamWriteLine(void*, const char*);
extern bool CFStreamAtEnd(void*);
extern void CFStreamClose(void*);

