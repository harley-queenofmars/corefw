/*******************************************************************
** This code is part of the Dark Framework.
**
MIT License

Copyright (c) 2018 Dark Overlord of Data

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
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
#include "corefw.h"     // IWYU pragma: keep
#include "bitvector.h"
#include "stdlib.h"

/**
 * CFBitVector instance variables
 */
typedef struct __CFBitVector 
{
    __CFObject      obj;
    int             length;
    unsigned int*   words;
} __CFBitVector;
 
/**
 * CFBitVector class
 */
static __CFClass class = {
    .name = "CFBitVector",
    .size = sizeof(__CFBitVector),
};
const CFClassRef CFBitVector = &class;


/*
 * CFBitVectors are packed into arrays of "words."    Currently a word
 * consists of 32 bits, requiring 5 address bits.
 */
const int ADDRESS_BITS_PER_WORD = 5;
const int64_t BITS_PER_WORD = 1 << ADDRESS_BITS_PER_WORD; // 32
const int64_t WORD_MASK = 0xffffffff;
/**
 * @see http://stackoverflow.com/questions/6506356/java-implementation-of-long-numberoftrailingzeros
 */
unsigned int numberOfTrailingZeros(unsigned int i)
{
        if (i == 0)
                return 32;
        unsigned int x = i;
        unsigned int y;
        unsigned int n = 31;
        y = x << 16;
        if (y != 0) {
                n -= 16;
                x = y;
        }
        y = x << 8;
        if (y != 0) {
                n -= 8;
                x = y;
        }
        y = x << 4;
        if (y != 0) {
                n -= 4;
                x = y;
        }
        y = x << 2;
        if (y != 0) {
                n -= 2;
                x = y;
        }
        return (n - ((x << 1) >> 31));
}

/**
 * Constructor
 * create a new CFBitVector
 * 
 * @param value of bool
 * 
 */
method void* New(CFBitVectorRef this, int nbits)
{
        this->length = 0;
        if (nbits > 0) {
                int size = (((nbits - 1) >> ADDRESS_BITS_PER_WORD) + 1);
                this->words = calloc((size_t)size, sizeof(int*));
                for (int i = 0; i < size; i++)
                        this->words[i] = 0;
                this->length = size;
        }
        return this;
}

method void* New(CFBitVectorRef this)
{
        return New(this, 16);
}

method int NextSetBit(CFBitVectorRef this, int fromIndex)
{
        int u = fromIndex >> ADDRESS_BITS_PER_WORD;
        int wordsInUse = this->length;

        unsigned int word = this->words[u] & (WORD_MASK << fromIndex);
        while (true) {
                if (word != 0)
                        return (int)((u * BITS_PER_WORD) + numberOfTrailingZeros(word));
                if (++u == wordsInUse)
                        return -1;
                word = this->words[u];
        }
}

method bool Intersects(CFBitVectorRef this, CFBitVectorRef set)
{
        int wordsInUse = this->length;

        for (int i = Min(wordsInUse, set->length) - 1; i >= 0; i--)
                if ((this->words[i] & set->words[i]) != 0)
                        return true;
        return false;
}

method bool IsEmpty(CFBitVectorRef this)
{
        return this->length == 0;
}

method int Size(CFBitVectorRef this)
{
        return (this->length << ADDRESS_BITS_PER_WORD);
}

method void Set(CFBitVectorRef this, int bitIndex, bool value)
{
        int wordIndex = bitIndex >> ADDRESS_BITS_PER_WORD;
        int wordsInUse = this->length;
        int wordsRequired = wordIndex + 1;

        if (wordIndex >= this->length) {
                this->words = realloc(this->words, sizeof(unsigned int) * (size_t)(wordIndex + 1));
                // this = realloc(this, sizeof(CFObject) + sizeof(int) + sizeof(unsigned int) * wordIndex + 1);
        }
        if (wordsInUse < wordsRequired) {

                int mm = Max(2 * wordsInUse, wordsRequired);
                // words.resize(int.max(2 * wordsInUse, wordsRequired));
                // this = realloc(this, sizeof(CFObject) + sizeof(int) + sizeof(unsigned int) * Max(2 * wordsInUse, wordsRequired));
                this->words = realloc(this->words, sizeof(unsigned int) * (size_t)mm);
                for (int i = wordsInUse, l = this->length; i < l; i++) {
                        this->words[i] = 0;
                }
        }

        if (value) {
                this->words[wordIndex] |= (1 << bitIndex);
        } else {
                this->words[wordIndex] &= ~(1 << bitIndex);
        }
}

method bool Get(CFBitVectorRef this, int bitIndex)
{
        int wordIndex = bitIndex >> ADDRESS_BITS_PER_WORD;
        int wordsInUse = this->length;

        return (wordIndex < wordsInUse) && ((this->words[wordIndex] & (1 << bitIndex)) != 0);
}

method void method Clear(CFBitVectorRef this, int bitIndex)
{
        if (bitIndex == -1) {
                int wordsInUse = this->length;
                while (wordsInUse > 0) {
                        this->words[--wordsInUse] = 0;
                }
                return;
        }

        int wordIndex = bitIndex >> ADDRESS_BITS_PER_WORD;
        if (this->length <= wordIndex) {
                this->words = realloc(this->words, sizeof(unsigned int) * (size_t)(wordIndex + 1));
                // this = realloc(this, sizeof(CFObject) + sizeof(int) + sizeof(unsigned int) * wordIndex + 1);
        }
        this->words[wordIndex] &= ~(1 << bitIndex);
}

method void method Clear(CFBitVectorRef this)
{
        Clear(this, -1);
}

/**
 * Returns the string value of this CFBitVector
 */
// method char* ToString(CFBitVectorRef this)
// {
//         (CFBitVectorRef)this;
        
//         return "CFBitVector";
//         //         string[] s = new string[words.length];
//         //         for (var i=0; i<words.length; i++)
//         //         {
//         //                 s[i] = "0x%08x".printf(words[i]);
//         //         }
//         //         return string.joinv("|", s);
//         // }
// }
