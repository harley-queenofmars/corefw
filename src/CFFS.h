#pragma once
#include "CFString.h"

struct CFFS {
    CFStringRef (*getPath)(const char* path);
    char* const (*getRoot)();
    CFStringRef (*getPathRelativeRoot)(const char* path);
    CFStringRef (*getPathRelativeBinary)(const char* path);
    CFStringRef (*readTextFile)(char* path);
};

extern struct CFFS CFFS;