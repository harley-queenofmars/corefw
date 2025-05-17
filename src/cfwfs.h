#pragma once
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "object.h"
#include "cfstring.h"
// #include "file.h"
// #include "stream.h"

struct CFFS {
    CFStringRef (*getPath)(const char* path);
    char* const (*getRoot)();
    CFStringRef (*getPathRelativeRoot)(const char* path);
    CFStringRef (*getPathRelativeBinary)(const char* path);
    CFStringRef (*readTextFile)(char* path);
};

extern struct CFFS CFFS;