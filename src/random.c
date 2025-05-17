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

#include "corefw.h" // IWYU pragma: keep
#include "random.h"
#include <string.h>

/* Period parameters */
#define MT19937_N 624
#define MT19937_M 397
#define MT19937_MATRIX_A 0x9908b0dfUL /* constant vector a */
#define MT19937_UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define MT19937_LOWER_MASK 0x7fffffffUL /* least significant r bits */

// static unsigned long mt[MT19937_N]; /* the array for the state vector    */
// static int mti=MT19937_N+1; /* mti==MT19937_N+1 means mt[MT19937_N] is not initialized */

static CFRandomRef CFRandomInstance = nullptr;

static inline void init_genrand(CFRandomRef this, unsigned long s);
static inline void init_by_array(CFRandomRef this, unsigned long init_key[], unsigned long key_length);
static inline unsigned long genrand_int32(CFRandomRef this);
static inline double genrand_real1(CFRandomRef this);

/**
 * CFRandom class
 */
static __CFClass class = {
    .name = "CFRandom",
    .size = sizeof(__CFRandom),
};
const CFClassRef CFRandom = &class;

unsigned long frameCounter = 999;

method unsigned long NextLong(void)
{
        if (CFRandomInstance == nullptr) {
                // unsigned long seed = (unsigned long)time(nullptr);
                unsigned long seed = frameCounter;
                CFRandomInstance = NewRandom(seed);
        }

        return genrand_int32(CFRandomInstance);
}

method double NextDouble(void)
{
        if (CFRandomInstance == nullptr) {
                // unsigned long seed = (unsigned long)time(nullptr);
                unsigned long seed = frameCounter;
                CFRandomInstance = NewRandom(seed);
        }
        return genrand_real1(CFRandomInstance);
}

method void* Ctor(CFRandomRef this)
{
        // (CFRandomRef)this;
        // return New(this, (unsigned long)time(nullptr));
        return NewRandom(frameCounter);
}

method void* Ctor(CFRandomRef this, unsigned long seed)
{
        memset(this->mt, 0, (MT19937_N * sizeof(unsigned long)));
        this->mti = MT19937_N + 1;
        init_genrand(this, seed);
        return this;
}

method void* Ctor(CFRandomRef this, unsigned long seed[], int length)
{
        memset(this->mt, 0, (MT19937_N * sizeof(unsigned long)));
        this->mti = MT19937_N + 1;
        init_by_array(this, seed, (unsigned long)length);
        return this;
}

/* initializes mt[MT19937_N] with a seed */
static inline void init_genrand(CFRandomRef this, unsigned long s)
{
        this->mt[0] = s & 0xffffffffUL;
        for (this->mti = 1; this->mti < MT19937_N; this->mti++) {
                this->mt[this->mti] = (1812433253UL * (this->mt[this->mti - 1] ^ (this->mt[this->mti - 1] >> 30)) + this->mti);
                /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
                /* In the previous versions, MSBs of the seed affect     */
                /* only MSBs of the array this->mt[].                                                */
                /* 2002/01/09 modified by Makoto Matsumoto                         */
                this->mt[this->mti] &= 0xffffffffUL;
                /* for >32 bit machines */
        }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
static inline void init_by_array(CFRandomRef this, unsigned long init_key[], unsigned long key_length)
{
        unsigned long i, j, k;
        init_genrand(this, 19650218UL);
        i = 1;
        j = 0;
        k = (MT19937_N > key_length ? MT19937_N : key_length);
        for (; k; k--) {
                this->mt[i] = (this->mt[i] ^ ((this->mt[i - 1] ^ (this->mt[i - 1] >> 30)) * 1664525UL))
                        + init_key[j] + j; /* non linear */
                this->mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
                i++;
                j++;
                if (i >= MT19937_N) {
                        this->mt[0] = this->mt[MT19937_N - 1];
                        i = 1;
                }
                if (j >= key_length)
                        j = 0;
        }
        for (k = MT19937_N - 1; k; k--) {
                this->mt[i] = (this->mt[i] ^ ((this->mt[i - 1] ^ (this->mt[i - 1] >> 30)) * 1566083941UL))
                        - i; /* non linear */
                this->mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
                i++;
                if (i >= MT19937_N) {
                        this->mt[0] = this->mt[MT19937_N - 1];
                        i = 1;
                }
        }

        this->mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

/* generates a random number on [0,0xffffffff]-interval */
static inline unsigned long genrand_int32(CFRandomRef this)
{
        unsigned long y;
        static unsigned long mag01[2] = { 0x0UL, MT19937_MATRIX_A };
        /* mag01[x] = x * MT19937_MATRIX_A    for x=0,1 */

        if (this->mti >= MT19937_N) { /* generate MT19937_N words at one time */
                int kk;

                if (this->mti == MT19937_N + 1) /* if init_genrand() has not been called, */
                        init_genrand(this, 5489UL); /* a default initial seed is used */

                for (kk = 0; kk < MT19937_N - MT19937_M; kk++) {
                        y = (this->mt[kk] & MT19937_UPPER_MASK) | (this->mt[kk + 1] & MT19937_LOWER_MASK);
                        this->mt[kk] = this->mt[kk + MT19937_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }
                for (; kk < MT19937_N - 1; kk++) {
                        y = (this->mt[kk] & MT19937_UPPER_MASK) | (this->mt[kk + 1] & MT19937_LOWER_MASK);
                        this->mt[kk] = this->mt[kk + (MT19937_M - MT19937_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
                }
                y = (this->mt[MT19937_N - 1] & MT19937_UPPER_MASK) | (this->mt[0] & MT19937_LOWER_MASK);
                this->mt[MT19937_N - 1] = this->mt[MT19937_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

                this->mti = 0;
        }

        y = this->mt[this->mti++];

        /* Tempering */
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680UL;
        y ^= (y << 15) & 0xefc60000UL;
        y ^= (y >> 18);

        return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
static inline long genrand_int31(CFRandomRef this)
{
        return (long)(genrand_int32(this) >> 1);
}

/* generates a random number on [0,1]-real-interval */
static inline double genrand_real1(CFRandomRef this)
{
        return (double)genrand_int32(this) * (1.0 / 4294967295.0);
        /* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
static inline double genrand_real2(CFRandomRef this)
{
        return (double)genrand_int32(this) * (1.0 / 4294967296.0);
        /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
static inline double genrand_real3(CFRandomRef this)
{
        return (((double)genrand_int32(this)) + 0.5) * (1.0 / 4294967296.0);
        /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
static inline double genrand_res53(CFRandomRef this)
{
        unsigned long a = genrand_int32(this) >> 5, b = genrand_int32(this) >> 6;
        return ((double)a * 67108864.0 + (double)b) * (1.0 / 9007199254740992.0);
}
/* These real versions are due to Isaku Wada, 2002/01/09 added */
