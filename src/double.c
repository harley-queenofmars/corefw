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
#include "object.h"
#include "double.h"

/**
 * CFDouble instance variables
 */
typedef struct __CFDouble 
{
	__CFObject 	obj;
	double 		value;
} __CFDouble;

/**
 * CFDouble class
 */
 static __CFClass class = {
	.name = "CFDouble",
	.size = sizeof(__CFDouble),
	.ctor = ctor,
	.equal = equal,
	.hash = hash,
	.copy = copy
};
CFClassRef CFDouble = &class;

static bool ctor(void *ptr, va_list args)
{
	CFDoubleRef double_ = ptr;

	double_->value = va_arg(args, double);

	return true;
}

static bool equal(void *ptr1, void *ptr2)
{
	CFObjectRef obj2 = ptr2;
	CFDoubleRef double1, double2;

	if (obj2->cls != CFDouble)
		return false;

	double1 = ptr1;
	double2 = ptr2;

	return (double1->value == double2->value);
}

static uint32_t hash(void *ptr)
{
	CFDoubleRef double_ = ptr;

	/* FIXME: Create a proper hash! */
	return (uint32_t)double_->value;
}

static void* copy(void *ptr)
{
	return CFRef(ptr);
}

extern proc CFDoubleRef NewDouble(double value)
{
	return CFNew(CFDouble, value);        
}

double CFDoubleValue(CFDoubleRef double_)
{
	return double_->value;
}

