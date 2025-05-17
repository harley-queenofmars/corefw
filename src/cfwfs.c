#include "cfwfs.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "file.h"
#include "stream.h"

typedef CFStringRef (*Builder)(const char* path);
static Builder getPathBuilder();
static CFStringRef getPath(const char* path);
static char* const getRoot();
static CFStringRef getPathRelativeRoot(const char* path);
static CFStringRef getPathRelativeBinary(const char* path);

//#include <file-private.h>
struct __CFFile {
	__CFStream stream;
	int fd;
	bool at_end;
};

/**
 * getPath
 * 
 * @param path
 * @returns the path
 */
static CFStringRef getPath(const char* path)
{
    Builder pathBuilder = getPathBuilder();
    return (*pathBuilder)(path);
}

/**
 * getRoot
 * 
 * @returns this path root
 */
static char* const getRoot()
{
    static char buf[2048];
#ifdef __EMSCRIPTEN__
    return strcpy(buf, "./");
#else
    return getcwd(buf, sizeof(buf));
#endif
}

/**
 * getPathBuilder
 * 
 * @returns path builder function
 */
static Builder getPathBuilder()
{
    if (strcmp(getRoot(), "") != 0)
        return getPathRelativeRoot;
    else
        return getPathRelativeBinary;
}

/**
 * getPathRelativeRoot
 * 
 * @param path
 */
static CFStringRef getPathRelativeRoot(const char* path)
{
    CFStringRef res = CFCreate(CFString, getRoot());
    CFStringAppendC(res, "/");
    CFStringAppendC(res, path);
    return res;
}

/**
 * getPathRelativeBinary
 * 
 * @param path
 */
static CFStringRef getPathRelativeBinary(const char* path)
{
    CFStringRef res = CFCreate(CFString, "../../../");
    CFStringAppendC(res, path);
    return res;
}

/**
 * ReadTextFile
 * 
 * @param path path to file
 * @returns string with file contents
 * 
 */
CFStringRef readTextFile(char* path)
{
    struct stat statbuf;

    CFFileRef  handle = CFNew(CFFile, path, "r");
    if (!handle) {
        printf("Unable to open %s\n", path);
        return CFCreate(CFString, "");
    }

    if (fstat(handle->fd, &statbuf) == -1) {
        printf("Unable to stat %s\n", path);
        return CFCreate(CFString, "");
    }
    
    long len = statbuf.st_size;
    char* content = (char*)calloc(1, len + 1);
    CFStreamRead(handle, content, len);
    CFStreamClose(handle);
    CFStringRef result = CFCreate(CFString, content);
    free(content);
    return result;
}

/**
 * CFFS object
 */
struct CFFS CFFS = {
    .getPath = getPath,
    .getRoot = getRoot,
    .getPathRelativeRoot = getPathRelativeRoot,
    .getPathRelativeBinary = getPathRelativeBinary,
    .readTextFile = readTextFile
};
