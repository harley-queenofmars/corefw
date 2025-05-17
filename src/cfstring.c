/*
 * Copyright (c) 2012, Jonathan Schleifer <js@webkeks.org>
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
#include <stdint.h>
#include <string.h>

#include "object.h"
#include "cfstring.h"
#include "hash.h"

/**
 * CFString instance variables
 */
typedef struct __CFString 
{
	__CFObject 	obj;
	char*		data;
	size_t 		len;
} __CFString;

/**
 * CFString class
 */
 static __CFClass class = {
	.name = "CFString",
	.size = sizeof(__CFString),
	.ctor = ctor,
	.dtor = dtor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};
CFClassRef CFString = &class;


proc CFStringRef NewString(char* value)
{
        return CFNew(CFString, value);        
}

proc char* cstr(CFStringRef this)
{
        return CFStringC(this);
}

proc char* ToString(CFStringRef this)
{
        return CFStringC(this);
}


proc int Length(CFStringRef this)
{
        return (int)CFStringLength(this);
}



size_t CFStrnLen(const char *s, size_t max)
{
	size_t i = 0;

	for (i = 0; i < max && *s != '\0'; s++)
		i++;

	return i;
}

char* CFStrDup(const char *s)
{
	char *copy;
	size_t len;

	len = strlen(s);

	if ((copy = malloc(len + 1)) == nullptr)
		return nullptr;

	memcpy(copy, s, len + 1);

	return copy;
}

char* CFStrnDup(const char *s, size_t max)
{
	char *copy;
	size_t len;

	len = CFStrnLen(s, max);

	if ((copy = malloc(len + 1)) == nullptr)
		return nullptr;

	memcpy(copy, s, len);
	copy[len] = 0;

	return copy;
}

static bool ctor(void *ptr, va_list args)
{
	CFStringRef str = ptr;
	const char *cstr = va_arg(args, const char*);

	if (cstr != nullptr) {
		str->data = nullptr;
		if ((str->data = CFStrDup(cstr)) == nullptr)
			return false;

		str->len = strlen(cstr);
	} else {
		str->data = nullptr;
		str->len = 0;
	}

	return true;
}

static void dtor(void *ptr)
{
	CFStringRef str = ptr;

	if (str->data != nullptr)
		free(str->data);
}

static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFStringRef str1, str2;

	if (obj2->cls != CFString)
		return false;

	str1 = ptr1;
	str2 = ptr2;

	if (str1->len != str2->len)
		return false;

	return !memcmp(str1->data, str2->data, str1->len);
}

static uint32_t hash(void *ptr)
{
	CFStringRef str = ptr;
	size_t i;
	uint32_t hash;

	CF_HASH_INIT(hash);

	for (i = 0; i < str->len; i++)
		CF_HASH_ADD(hash, str->data[i]);

	CF_HASH_FINALIZE(hash);

	return hash;
}

static void* copy(void *ptr)
{
	CFStringRef str = ptr;
	CFStringRef new;

	if ((new = CFNew(CFString, (void*)nullptr)) == nullptr)
		return nullptr;

	if ((new->data = malloc(str->len + 1)) == nullptr) {
		CFUnref(new);
		return nullptr;
	}
	new->len = str->len;

	memcpy(new->data, str->data, str->len + 1);

	return new;
}

char* CFStringC(CFStringRef str)
{
	return str->data;
}

size_t CFStringLength(CFStringRef string)
{
	return string->len;
}

bool CFStringSet(CFStringRef str, const char *cstr)
{
	char *copy;
	size_t len;

	if (str != nullptr) {
		if ((copy = CFStrDup(cstr)) == nullptr)
			return false;

		len = strlen(copy);
	} else {
		copy = nullptr;
		len = 0;
	}

	if (str->data != nullptr)
		free(str->data);

	str->data = copy;
	str->len = len;

	return true;
}

void CFStringSetNoCopy(CFStringRef str, char *cstr, size_t len)
{
	if (str->data != nullptr)
		free(str->data);

	str->data = cstr;
	str->len = len;
}

bool CFStringAppend(CFStringRef str, CFStringRef append)
{
	char *new;

	if (append == nullptr)
		return true;

	if ((new = realloc(str->data, str->len + append->len + 1)) == nullptr)
		return false;

	memcpy(new + str->len, append->data, append->len);
	new[str->len + append->len] = 0;

	str->data = new;
	str->len += append->len;

	return true;
}

bool CFStringAppendC(CFStringRef str, const char *append)
{
	char *new;
	size_t append_len;

	if (append == nullptr)
		return true;

	append_len = strlen(append);

	if ((new = realloc(str->data, str->len + append_len + 1)) == nullptr)
		return false;

	memcpy(new + str->len, append, append_len);
	new[str->len + append_len] = 0;

	str->data = new;
	str->len += append_len;

	return true;
}

bool CFStringHasPrefix(CFStringRef str, CFStringRef prefix)
{
	if (prefix->len > str->len)
		return false;

	return !memcmp(str->data, prefix->data, prefix->len);
}

bool CFStringHasPrefixC(CFStringRef str, const char *prefix)
{
	size_t prefix_len = strlen(prefix);

	if (prefix_len > str->len)
		return false;

	return !memcmp(str->data, prefix, prefix_len);
}

bool CFStringHasSuffix(CFStringRef str, CFStringRef suffix)
{
	if (suffix->len > str->len)
		return false;

	return !memcmp(str->data, suffix->data, suffix->len);
}

bool CFStringHasSuffixC(CFStringRef str, const char *suffix)
{
	size_t suffix_len = strlen(suffix);

	if (suffix_len > str->len)
		return false;

	return !memcmp(str->data, suffix, suffix_len);
}

size_t CFStringFind(CFStringRef str, CFStringRef substr, CFRange_t range)
{
	size_t i;

	if (range.start > str->len)
		return SIZE_MAX;

	if (range.length == SIZE_MAX)
		range.length = str->len - range.start;

	if (range.start + range.length > str->len || substr->len > range.length)
		return SIZE_MAX;

	for (i = range.start; i <= range.start + range.length - substr->len;
	        i++)
		if (!memcmp(str->data + i, substr->data, substr->len))
			return i;

	return SIZE_MAX;
}

size_t CFStringFindC(CFStringRef str, const char *substr, CFRange_t range)
{
	size_t substr_len = strlen(substr);
	size_t i;

	if (range.start > str->len)
		return SIZE_MAX;

	if (range.length == SIZE_MAX)
		range.length = str->len - range.start;

	if (range.start + range.length > str->len || substr_len > range.length)
		return SIZE_MAX;

	for (i = range.start; i <= range.start + range.length - substr_len;
	        i++)
		if (!memcmp(str->data + i, substr, substr_len))
			return i;

	return SIZE_MAX;
}

/**
 * join strings
 * 
 * @param count of strings
 * @param ... list of char*'s
 * @returns all strings concantenated together.
 */
char* CFStringJoin(int count, ...)
{
        
        int size = 0;
        va_list args1;
        va_start(args1, count);
        va_list args2;
        va_copy(args2, args1);    

        /**
         * Caclulate length of the result string
         */
        for (int i = 0; i < count; ++i) {
                char* str = va_arg(args1, char*);
                size += strlen(str);
        }
        va_end(args1);
        char* result = (char*)calloc(((size_t)(size+1)),    sizeof(char));

        /**
         * Now build the result string
         */
        for (int i = 0; i < count; ++i) {
                char* str = va_arg(args2, char*);
                strcat(result, str);
        }
        va_end(args2);
        return result;
}

