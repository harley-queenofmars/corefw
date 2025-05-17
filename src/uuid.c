/*******************************************************************
** This code is part of the Dark Framework.
**
MIT License

Copyright (c) 2018 Dark Overlord of Data

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
#include "random.h"
#include "uuid.h"
#include "cfstring.h"
#include <string.h>

/**
 * CFUuid class
 */
static __CFClass class = {
    .name = "CFUuid",
    .size = sizeof(__CFUuid),
};
const CFClassRef CFUuid = &class;

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

proc char* ToString(CFUuidRef this)
{
    return ToString(this, 'D');
}