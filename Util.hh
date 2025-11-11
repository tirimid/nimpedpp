// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstddef>
#include <cstdint>

extern "C"
{
#include <unistd.h>
}

#define OUT
#define IN_OUT
#define OWNS

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

using i8    = int8_t;
using i16   = int16_t;
using i32   = int32_t;
using i64   = int64_t;
using isize = ssize_t;
using u8    = uint8_t;
using u16   = uint16_t;
using u32   = uint32_t;
using u64   = uint64_t;
using usize = size_t;
using f32   = float;
using f64   = double;

void        Info(const char* fmt, ...);
void        Error(const char* fmt, ...);
u64         FileID(const char* path, bool dereference);
i32         RecursiveCreateDir(const char* path);
i32         CreateFile(const char* path);
const char* FileExtension(const char *path);
void        AppendCString(char* dst, usize dstSize, const char* src);
void        TruncateCString(char* str, usize maxLength);
