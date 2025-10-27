// SPDX-License-Identifier: GPL-3.0-or-later

#include <Options.hh>

DynamicOptions  g_Options;

static FILE*  OpenConfig(const char* path);
static i32    GetRaw(const char* path, FILE* file, const char* key, OUT char buffer[]);
static i32    GetRaw(const char* path, FILE* file, const char* key, OUT char buffer[], i32 n);
static i32    GetU32(const char* path, FILE* file, const char* key, OUT u32& value);
static i32    GetColor(const char* path, FILE* file, const char* key, OUT Color& value);
static i32    GetBool(const char* path, FILE* file, const char* key, OUT bool& value);
static void   ReadLangConfig(FILE* file, const char* keywordKey, const char* primitiveKey, LangMode langMode);

i32 ParseOptions()
{
  // TODO: implement
  return (1);
}

static FILE*  OpenConfig(const char* path)
{
  // TODO: implement
}

static i32  GetRaw(const char* path, FILE* file, const char* key, OUT char buffer[])
{
  // TODO: implement
}

static i32  GetRaw(const char* path, FILE* file, const char* key, OUT char buffer[], i32 n)
{
  // TODO: implement
}

static i32  GetU32(const char* path, FILE* file, const char* key, OUT u32& value)
{
  // TODO: implement
}

static i32  GetColor(const char* path, FILE* file, const char* key, OUT Color& value)
{
  // TODO: implement
}

static i32  GetBool(const char* path, FILE* file, const char* key, OUT bool& value)
{
  // TODO: implement
}

static void ReadLangConfig(FILE* file, const char* keywordKey, const char* primitiveKey, LangMode langMode)
{
  // TODO: implement
}
