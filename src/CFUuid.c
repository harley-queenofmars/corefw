/*******************************************************************
** This code is part of the Dark Framework.
**
MIT License

Copyright (c) 2018 Dark Overlord of Data
Copyright (c) 2025 harley davidson <harley.queenofmars@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the 0xSoftware0x), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 0xAS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************/
#include "CFRandom.h"
#include "CFUuid.h"
#include "CFString.h"
#include <string.h>

class(CFUuid);

/**
 * @brief Generates a new UUID and assigns it to the given CFUuidRef instance.
 *
 * This function creates a new UUID by generating four random unsigned long values,
 * copying them into the uuid array, and setting the appropriate version and variant bits
 * according to the UUID version 4 specification. The generated UUID is also copied into
 * the value field of the structure.
 *
 * @param this Pointer to a CFUuidRef structure where the new UUID will be stored.
 * @return Pointer to the same CFUuidRef structure with the newly generated UUID.
 *
 * @note The function does not currently check for duplicate UUIDs.
 */
proc void* New(CFUuidRef this)
{
    unsigned long d0 = NextLong();
    unsigned long d1 = NextLong();
    unsigned long d2 = NextLong();
    unsigned long d3 = NextLong();

    memcpy(&this->uuid[0], &d0, 8);
    memcpy(&this->uuid[4], &d1, 8);
    memcpy(&this->uuid[8], &d2, 8);
    memcpy(&this->uuid[12], &d3, 8);

    this->uuid[6] = (this->uuid[6] & 0x0f) | 0x40;
    this->uuid[8] = (this->uuid[8] & 0x3f) | 0x80;

    memcpy(&this->value, &this->uuid[0], 16);

    /**
     * 
     * todo: verify that its not a duplicate
     */
    return this;
}

/**
 * Returns the printf-style format string for representing a UUID in various formats.
 *
 * Supported formats:
 *   - 'N': 32 digits, no hyphens (e.g., 00000000000000000000000000000000)
 *   - 'D': 32 digits separated by hyphens (e.g., 00000000-0000-0000-0000-000000000000)
 *   - 'B': 32 digits separated by hyphens, enclosed in braces (e.g., {00000000-0000-0000-0000-000000000000})
 *   - 'P': 32 digits separated by hyphens, enclosed in parentheses (e.g., (00000000-0000-0000-0000-000000000000))
 *   - 'X': Hexadecimal format with braces and commas (e.g., {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}})
 *
 * If an unknown format is specified, the function defaults to the 'N' format.
 *
 * @param format The format specifier character ('N', 'D', 'B', 'P', or 'X').
 * @return       The corresponding printf-style format string for the specified UUID format.
 */
proc char* GetToStringFormat(char format)
{
    switch (format) {
    case 'N':
        return "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x";
    case 'D':
        return "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x";
    case 'B':
        return "{%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x}";
    case 'P':
        return "(%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x)";
    case 'X':
        return "{0x%02x%02x%02x%02x,0x%02x%02x,0x%02x%02x,{0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x}}";
    }
    return "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x";
}

/**
 * Converts a CFUuidRef object to its string representation.
 *
 * The resulting string is formatted as a standard UUID (8-4-4-4-12 hexadecimal digits),
 * for example: "123e4567-e89b-12d3-a456-426614174000".
 *
 * @param this   Pointer to the CFUuidRef object to convert.
 * @param format Format specifier (currently unused).
 * @return       A newly created CFString containing the UUID string representation.
 */
proc char* ToString(CFUuidRef this, char format)
{
    (void)format;
    static char hex[256][3] = { // hex identity values 0-255
        "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f",
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f",
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f",
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f",
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4a", "4b", "4c", "4d", "4e", "4f",
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5a", "5b", "5c", "5d", "5e", "5f",
        "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b", "6c", "6d", "6e", "6f",
        "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d", "7e", "7f",
        "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
        "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f",
        "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af",
        "b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf",
        "c0", "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf",
        "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8", "d9", "da", "db", "dc", "dd", "de", "df",
        "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "ea", "eb", "ec", "ed", "ee", "ef",
        "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb", "fc", "fd", "fe", "ff"
    };

    char r[37];

    memcpy(&r[0],    hex[this->value.byte0], 2);
    memcpy(&r[2],    hex[this->value.byte1], 2);
    memcpy(&r[4],    hex[this->value.byte2], 2);
    memcpy(&r[6],    hex[this->value.byte3], 2);
    r[8] = '-';
    memcpy(&r[9],    hex[this->value.byte4], 2);
    memcpy(&r[11], hex[this->value.byte5], 2);
    r[13] = '-';
    memcpy(&r[14], hex[this->value.byte6], 2);
    memcpy(&r[16], hex[this->value.byte7], 2);
    r[18] = '-';
    memcpy(&r[19], hex[this->value.byte8], 2);
    memcpy(&r[21], hex[this->value.byte9], 2);
    r[23] = '-';
    memcpy(&r[24], hex[this->value.byte10], 2);
    memcpy(&r[26], hex[this->value.byte11], 2);
    memcpy(&r[28], hex[this->value.byte12], 2);
    memcpy(&r[30], hex[this->value.byte13], 2);
    memcpy(&r[32], hex[this->value.byte14], 2);
    memcpy(&r[34], hex[this->value.byte15], 2);
    r[36] = 0;

    return CFCreate(CFString, r);
}

/**
 * Converts the given CFUuidRef object to its string representation.
 *
 * @param this A reference to the CFUuid object to be converted.
 * @return A pointer to a string representing the UUID.
 *
 * @note This function calls the overloaded ToString function with a default format specifier 'D'.
 */
proc char* ToString(CFUuidRef this)
{
    return ToString(this, 'D');
}