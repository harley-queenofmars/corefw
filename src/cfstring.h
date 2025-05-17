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
#include "range.h"

extern CFClassRef CFString;
typedef struct __CFString *CFStringRef;

extern size_t CFStrnLen(const char *, size_t);
extern char *CFStrDup(const char *);
extern char *CFStrnDup(const char *, size_t);
extern char *CFStringC(CFStringRef);
extern size_t CFStringLength(CFStringRef);
extern bool CFStringSet(CFStringRef, const char *);
extern void CFStringSetNoCopy(CFStringRef, char *, size_t);
extern bool CFStringAppend(CFStringRef, CFStringRef);
extern bool CFStringAppendC(CFStringRef, const char *);
extern bool CFStringHasPrefix(CFStringRef, CFStringRef);
extern bool CFStringHasPrefixC(CFStringRef, const char *);
extern bool CFStringHasSuffix(CFStringRef, CFStringRef);
extern bool CFStringHasSuffixC(CFStringRef, const char *);
extern size_t CFStringFind(CFStringRef, CFStringRef, CFRange_t);
extern size_t CFStringFindC(CFStringRef, const char *, CFRange_t);
extern char *CFStringJoin(int count, ...);

// extern method CFStringRef NewString();
extern method CFStringRef NewString(char *);
extern method char *cstr(CFStringRef);
extern method char *ToString(CFStringRef);
extern method int Length(CFStringRef);
