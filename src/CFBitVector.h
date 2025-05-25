/*******************************************************************
** This code is part of the Dark Framework.
**
MIT License

Copyright (c) 2018 Dark Overlord of Data <darkoverlordofdata@gmail.com>
Copyright (c) 2025 harley davidson <harley.queenofmars@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software");, to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************/
#pragma once
#include <assert.h>
#include "CFClass.h"

extern CFClassRef CFBitVector;

typedef struct __CFBitVector* CFBitVectorRef;

extern proc void* New(CFBitVectorRef);
extern proc void* New(CFBitVectorRef, int);
extern proc int NextSetBit(CFBitVectorRef, int); 
extern proc bool Intersects(CFBitVectorRef, CFBitVectorRef); 
extern proc bool IsEmpty(CFBitVectorRef); 
extern proc void Set(CFBitVectorRef, int, bool); 
extern proc bool Get(CFBitVectorRef, int); 
extern proc void Clear(CFBitVectorRef);
extern proc void Clear(CFBitVectorRef, int);    
extern proc int Size(CFBitVectorRef);
// extern proc char* ToString(CFBitVectorRef);



/**
 *    MACRO Min
 *            cache results of calculation in pocket scope 
 */
 #define Min(a, b)                        \
 ({                                             \
         __auto_type _a = a;    \
         __auto_type _b = b;    \
         (_a < _b) ? _a : _b; \
 })

/**
*    MACRO Max
*            cache results of calculation in pocket scope 
*/
#define Max(a, b)                        \
 ({                                             \
         __auto_type _a = a;    \
         __auto_type _b = b;    \
         (_a > _b) ? _a : _b; \
 })

