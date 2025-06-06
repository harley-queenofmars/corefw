/* 
     A C-program for MT19937, with initialization improved 2002/1/26.
     Coded by Takuji Nishimura and Makoto Matsumoto.

     Before using, initialize the state by using init_genrand(seed)    
     or init_by_array(init_key, key_length).

     Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
     All rights reserved.                                                    

     Redistribution and use in source and binary forms, with or without
     modification, are permitted provided that the following conditions
     are met:

         1. Redistributions of source code must retain the above copyright
                notice, this list of conditions and the following disclaimer.

         2. Redistributions in binary form must reproduce the above copyright
                notice, this list of conditions and the following disclaimer in the
                documentation and/or other materials provided with the distribution.

         3. The names of its contributors may not be used to endorse or promote 
                products derived from this software without specific prior written 
                permission.

     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
     A PARTICULAR PURPOSE ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
     EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
     PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
     PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
     LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


     Any feedback is very welcome.
     http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
     email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/
#pragma once
#include "CFClass.h"
#include "CFObject.h"

extern CFClassRef CFRandom;
typedef struct __CFRandom* CFRandomRef;

/* Period parameters */    
#define MT19937_N 624
#define MT19937_M 397
#define MT19937_MATRIX_A 0x9908b0dfUL     /* constant vector a */
#define MT19937_UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define MT19937_LOWER_MASK 0x7fffffffUL /* least significant r bits */

/**
 * @struct __CFRandom
 * @brief Represents a Mersenne Twister (MT19937) random number generator state.
 *
 * This structure holds the internal state required for generating random numbers
 * using the MT19937 algorithm. It includes an object header, the current index
 * into the state array, the state array itself, and the initial seed value.
 *
 * @var __CFObject obj
 *      Base object structure for reference counting or type identification.
 * @var unsigned long mti
 *      Current index into the state array `mt`.
 * @var unsigned long mt[MT19937_N]
 *      State vector array for the MT19937 algorithm.
 * @var unsigned long seed
 *      Initial seed value used to initialize the generator.
 */
typedef struct __CFRandom 
{
     __CFObject      obj;
     unsigned long   mti;
     unsigned long   mt[MT19937_N];
     unsigned long   seed;
} __CFRandom;

extern proc void* Ctor(CFRandomRef);
extern proc void* Ctor(CFRandomRef, unsigned long);
extern proc void* Ctor(CFRandomRef, unsigned long [], int );

extern proc unsigned long NextLong(void);
extern proc double NextDouble(void);

/**
 * @brief Creates and initializes a new CFRandom instance with the specified seed.
 *
 * This function allocates a new CFRandom object and initializes it using the provided seed value.
 *
 * @param seed The seed value to initialize the random number generator.
 * @return CFRandomRef A reference to the newly created and seeded CFRandom instance.
 */
static inline CFRandomRef NewRandom(unsigned long seed)
{
        return Ctor((CFRandomRef)CFCreate(CFRandom), seed);
}