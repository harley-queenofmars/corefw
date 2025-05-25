/*******************************************************************
** This code is part of the Dark Framework.
**
MIT License

Copyright (c) 2018 Dark Overlord of Data
Copyright (c) 2025 harley davidson <harley.queenofmars@gmail.com>

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
#include "CFBitVector.h"
#include "stdlib.h"

/**
 * @struct __CFBitVector
 * @brief Represents a bit vector structure for efficient storage and manipulation of bits.
 *
 * This structure is used to manage a dynamic array of bits, allowing for compact storage and
 * bitwise operations. It contains a base object, the length of the bit vector, and a pointer
 * to the underlying storage of bits as an array of unsigned integers.
 *
 * @var __CFBitVector::obj
 *      Base object for Core Foundation object management.
 * @var __CFBitVector::length
 *      The number of bits stored in the bit vector.
 * @var __CFBitVector::words
 *      Pointer to the array of unsigned integers that store the bits.
 */
typedef struct __CFBitVector 
{
    __CFObject      obj;
    int         length;
    unsigned int*   words;
} __CFBitVector;
 
class(CFBitVector);

/*
 * CFBitVectors are packed into arrays of "words."    Currently a word
 * consists of 32 bits, requiring 5 address bits.
 */
const int ADDRESS_BITS_PER_WORD = 5;
const int64_t BITS_PER_WORD = 1 << ADDRESS_BITS_PER_WORD; // 32
const int64_t WORD_MASK = 0xffffffff;

/**
 * @brief Returns the number of trailing zero bits in the binary representation of an unsigned integer.
 *
 * This function counts the number of consecutive zero bits starting from the least significant bit (LSB)
 * up to the first set bit (1) in the given unsigned integer. If the input is zero, the function returns 32,
 * as there are 32 bits in an unsigned int.
 * @see http://stackoverflow.com/questions/6506356/java-implementation-of-long-numberoftrailingzeros
 *
 * @param i The unsigned integer to examine.
 * @return The number of trailing zeros in the binary representation of @p i, or 32 if @p i is zero.
 */
unsigned int numberOfTrailingZeros(unsigned int i)
{
    if (i == 0) return 32;
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
 * @brief Initializes a CFBitVector object with the specified number of bits.
 *
 * Allocates memory for the internal word array to store the bit vector,
 * initializes all bits to zero, and sets the length accordingly.
 *
 * @param this Pointer to the CFBitVectorRef structure to initialize.
 * @param nbits The number of bits to allocate in the bit vector.
 * @return Pointer to the initialized CFBitVectorRef structure.
 */
proc void* New(CFBitVectorRef this, int nbits)
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

/**
 * @brief Creates a new CFBitVector instance with a default initial size.
 *
 * This function allocates and initializes a new CFBitVector object using
 * the provided reference and a default size of 16 bits.
 *
 * @param this A reference to the CFBitVector to be created.
 * @return A pointer to the newly created CFBitVector instance.
 */
proc void* New(CFBitVectorRef this)
{
    return New(this, 16);
}

/**
 * Finds the index of the next set bit (bit with value 1) in the bit vector, starting from the specified index.
 *
 * @param this      Pointer to the CFBitVector structure.
 * @param fromIndex The index to start searching from (inclusive).
 * @return The index of the next set bit, or -1 if no such bit exists after fromIndex.
 */
proc int NextSetBit(CFBitVectorRef this, int fromIndex)
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

/**
 * @brief Checks if two CFBitVector objects have any bits in common.
 *
 * This function determines whether there is at least one bit set in both
 * the current bit vector (`this`) and the provided bit vector (`set`).
 * It iterates through the words of both vectors up to the minimum length
 * of the two, performing a bitwise AND operation on corresponding words.
 * If any result is non-zero, the function returns true, indicating that
 * the two bit vectors intersect (i.e., share at least one set bit).
 *
 * @param this Pointer to the first CFBitVectorRef object.
 * @param set Pointer to the second CFBitVectorRef object.
 * @return true if there is at least one common set bit; false otherwise.
 */
proc bool Intersects(CFBitVectorRef this, CFBitVectorRef set)
{
    int wordsInUse = this->length;

    for (int i = Min(wordsInUse, set->length) - 1; i >= 0; i--)
        if ((this->words[i] & set->words[i]) != 0)
            return true;
    return false;
}

/**
 * @brief Checks if the CFBitVector is empty.
 *
 * This function returns true if the bit vector contains no elements (i.e., its length is zero).
 *
 * @param this Pointer to the CFBitVector instance.
 * @return true if the bit vector is empty, false otherwise.
 */
proc bool IsEmpty(CFBitVectorRef this)
{
    return this->length == 0;
}

/**
 * @brief Returns the size of the bit vector in bits.
 *
 * Calculates the total number of bits represented by the CFBitVectorRef instance.
 * The size is determined by shifting the 'length' member by ADDRESS_BITS_PER_WORD,
 * which typically represents the number of bits in a word (e.g., 5 for 32 bits, 6 for 64 bits).
 *
 * @param this Pointer to the CFBitVectorRef instance.
 * @return The size of the bit vector in bits.
 */
proc int Size(CFBitVectorRef this)
{
    return (this->length << ADDRESS_BITS_PER_WORD);
}

/**
 * @brief Sets or clears a specific bit in the CFBitVector.
 *
 * This function sets or clears the bit at the specified index in the bit vector.
 * If the internal storage is not large enough to accommodate the bit at bitIndex,
 * it reallocates memory to expand the storage. The function ensures that the
 * storage is resized efficiently, potentially doubling its size or expanding to
 * fit the required index. The bit is then set or cleared according to the value
 * parameter.
 *
 * @param this      Pointer to the CFBitVector object.
 * @param bitIndex  The index of the bit to set or clear.
 * @param value     Boolean value indicating whether to set (true) or clear (false) the bit.
 */
proc void Set(CFBitVectorRef this, int bitIndex, bool value)
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

/**
 * @brief Retrieves the value of a specific bit in the bit vector.
 *
 * This function checks whether the bit at the specified index is set (1) or not (0)
 * in the given CFBitVectorRef instance.
 *
 * @param this Pointer to the CFBitVectorRef structure.
 * @param bitIndex The index of the bit to retrieve.
 * @return true if the bit at bitIndex is set, false otherwise.
 */
proc bool Get(CFBitVectorRef this, int bitIndex)
{
    int wordIndex = bitIndex >> ADDRESS_BITS_PER_WORD;
    int wordsInUse = this->length;

    return (wordIndex < wordsInUse) && ((this->words[wordIndex] & (1 << bitIndex)) != 0);
}

/**
 * @brief Clears a specific bit or all bits in the CFBitVector.
 *
 * If bitIndex is -1, all bits in the bit vector are cleared (set to 0).
 * Otherwise, clears the bit at the specified bitIndex.
 * If the internal storage is not large enough to accommodate the bitIndex,
 * the storage is reallocated to fit the required size.
 *
 * @param this Pointer to the CFBitVectorRef structure.
 * @param bitIndex Index of the bit to clear, or -1 to clear all bits.
 */
proc void proc Clear(CFBitVectorRef this, int bitIndex)
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

/**
 * @brief Clears all bits in the given CFBitVector.
 *
 * This function resets all bits in the specified CFBitVectorRef to 0.
 *
 * @param this Pointer to the CFBitVector to be cleared.
 */
proc void proc Clear(CFBitVectorRef this)
{
    Clear(this, -1);
}

/**
 * Returns the string value of this CFBitVector
 */
// proc char* ToString(CFBitVectorRef this)
// {
//     (CFBitVectorRef)this;
    
//     return "CFBitVector";
//     //     string[] s = new string[words.length];
//     //     for (var i=0; i<words.length; i++)
//     //     {
//     //         s[i] = "0x%08x".printf(words[i]);
//     //     }
//     //     return string.joinv("|", s);
//     // }
// }
