#pragma once
#include <stddef.h>
#include <stdint.h>

typedef uint8_t UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef size_t Counter;


typedef unsigned int uint;
typedef unsigned long ulong;


#include "class.h"       // IWYU pragma: keep
#include "object.h"      // IWYU pragma: keep
#include "array.h"       // IWYU pragma: keep
#include "cfstring.h"    // IWYU pragma: keep
#include "bag.h"         // IWYU pragma: keep
#include "bool.h"        // IWYU pragma: keep
#include "box.h"         // IWYU pragma: keep
#include "double.h"      // IWYU pragma: keep
#include "hash.h"        // IWYU pragma: keep
#include "int.h"         // IWYU pragma: keep
#include "map.h"         // IWYU pragma: keep
#include "range.h"       // IWYU pragma: keep
#include "refpool.h"     // IWYU pragma: keep
#include "bitvector.h"   // IWYU pragma: keep
#include "random.h"      // IWYU pragma: keep
#include "uuid.h"        // IWYU pragma: keep
#include "file.h"        // IWYU pragma: keep
#include "stream.h"      // IWYU pragma: keep
#include "cfwfs.h"       // IWYU pragma: keep
