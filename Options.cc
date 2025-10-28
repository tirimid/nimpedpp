// SPDX-License-Identifier: GPL-3.0-or-later

#include <Args.hh>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <Options.hh>

extern "C"
{
#include <pwd.h>
}

DynamicOptions  g_Options;

static FILE*  OpenConfig(const char* configPath);
static i32    GetRaw(const char* path, FILE* file, const char* key, OUT char value[]);
static bool   GetRaw(const char* path, FILE* file, const char* key, OUT char value[], i32 n);
static i32    GetU32(const char* path, FILE* file, const char* key, OUT u32& value);
static bool   GetU32(const char* path, FILE* file, const char* key, OUT u32& value, i32 n);
static i32    GetColor(const char* path, FILE* file, const char* key, OUT u8& value);
static i32    GetColorPair(const char* path, FILE* file, const char* key, OUT Color& value);
static i32    GetBool(const char* path, FILE* file, const char* key, OUT bool& value);
static void   ReadLangConfig(FILE* file, const char* keywordKey, const char* primitiveKey, LangMode langMode);

i32 ParseOptions()
{
  FILE* file  = OpenConfig(FUNCTIONAL::EDITOR_CONF);
  if (!file)
  {
    return (1);
  }
  
  auto  getEditorU32  = [&](const char* key, OUT u32& value)
  {
    i32 error = GetU32(FUNCTIONAL::EDITOR_CONF, file, key, value);
    return (error);
  };
  
  // layout options
  if (getEditorU32("MasterNumer", g_Options.m_MasterNumer)
    || getEditorU32("MasterDenom", g_Options.m_MasterDenom)
    || getEditorU32("LeftGutter", g_Options.m_LeftGutter)
    || getEditorU32("RightGutter", g_Options.m_RightGutter)
    || getEditorU32("TabSize", g_Options.m_TabSize))
  {
    fclose(file);
    return (1);
  }
  
  u32 margin  {};
  for (i32 i = 0; GetU32(FUNCTIONAL::EDITOR_CONF, file, "Margin", margin, i); ++i)
  {
    ++g_Options.m_NMargins;
    g_Options.m_Margins = (u32*)reallocarray(g_Options.m_Margins, g_Options.m_NMargins, sizeof(u32));
    g_Options.m_Margins[g_Options.m_NMargins - 1] = margin;
  }
  
  // editing options
  if (GetBool(FUNCTIONAL::EDITOR_CONF, file, "TabSpaces", g_Options.m_TabSpaces))
  {
    fclose(file);
    return (1);
  }
  
  fclose(file);
  file = OpenConfig(FUNCTIONAL::COLOR_CONF);
  if (!file)
  {
    return (1);
  }
  
  auto  getColorColor = [&](const char* key, OUT u8& value)
  {
    i32 error = GetColor(FUNCTIONAL::COLOR_CONF, file, key, value);
    return (error);
  };
  
  auto  getColorColorPair = [&](const char* key, OUT Color& value)
  {
    i32 error = GetColorPair(FUNCTIONAL::COLOR_CONF, file, key, value);
    return (error);
  };
  
  // theme options
  if (getColorColorPair("Global", g_Options.m_Global)
    || getColorColorPair("GlobalCursor", g_Options.m_GlobalCursor)
    || getColorColorPair("Window", g_Options.m_Window)
    || getColorColorPair("CurrentWindow", g_Options.m_CurrentWindow)
    || getColorColorPair("Normal", g_Options.m_Normal)
    || getColorColorPair("LineNumber", g_Options.m_LineNumber)
    || getColorColorPair("LineNumberHighlight", g_Options.m_LineNumberHighlight)
    || getColorColorPair("Margin", g_Options.m_Margin)
    || getColorColorPair("Cursor", g_Options.m_Cursor)
    || getColorColor("CursorHighlightBG", g_Options.m_CursorHighlightBG)
    || getColorColorPair("Comment", g_Options.m_Comment)
    || getColorColorPair("Macro", g_Options.m_Macro)
    || getColorColorPair("Special", g_Options.m_Special)
    || getColorColorPair("Keyword", g_Options.m_Keyword)
    || getColorColorPair("Primitive", g_Options.m_Primitive)
    || getColorColorPair("Type", g_Options.m_Type)
    || getColorColorPair("Emphasis", g_Options.m_Emphasis)
    || getColorColorPair("String", g_Options.m_String)
    || getColorColorPair("Number", g_Options.m_Number))
  {
    fclose(file);
    return (1);
  }
  
  fclose(file);
  file = OpenConfig(FUNCTIONAL::LANG_CONF);
  if (!file)
  {
    return (1);
  }
  
  // language mode options
  ReadLangConfig(file, "CKeyword", "CPrimitive", LANG_MODE_C);
  ReadLangConfig(file, "ShKeyword", "ShPrimitive", LANG_MODE_SH);
  ReadLangConfig(file, "JSKeyword", "JSPrimitive", LANG_MODE_JS);
  ReadLangConfig(file, "CCKeyword", "CCPrimitive", LANG_MODE_CC);
  
  fclose(file);
  return (0);
}

static FILE*  OpenConfig(const char* configPath)
{
  char  path[PATH_MAX]  {};
  
  if (g_Args.m_ConfigDir)
  {
    usize length  = snprintf(path, PATH_MAX, "%s", g_Args.m_ConfigDir);
    
    char  fixed[PATH_MAX] {};
    if (length && path[length - 1] != '/')
    {
      // compiler really needs to shut up about this crap
      i32 nbytes  = snprintf(fixed, sizeof(fixed), "%s/", path);
      (void)nbytes;
    }
    else
    {
      snprintf(fixed, sizeof(fixed), "%s", path);
    }
    
    snprintf(path, sizeof(path), "%s%s", fixed, configPath);
  }
  else
  {
    const char* home  = getenv("HOME");
    if (!home)
    {
      struct passwd*  userPasswd  = getpwuid(getuid());
      if (!userPasswd)
      {
        Error("Options: Failed on getpwuid() getting home directory!");
        return (nullptr);
      }
      
      home = userPasswd->pw_dir;
    }
    
    snprintf(path, sizeof(path), "%s/%s/%s", home, FUNCTIONAL::CONFIG_DIR, configPath);
  }
  
  FILE* file  = fopen(path, "rb");
  if (!file)
  {
    Error("Options: Failed to open config file: %s!", path);
    return (nullptr);
  }
  
  return (file);
}

static i32  GetRaw(const char* path, FILE* file, const char* key, OUT char value[])
{
  fseek(file, 0, SEEK_SET);
  
  for (usize line = 0; !feof(file) && !ferror(file); ++line)
  {
    i32 ch  {};
    while (ch = fgetc(file), ch != EOF && isspace(ch))
    {
    }
    
    if (ch == '#')
    {
      while (ch = fgetc(file), ch != EOF && ch != '\n')
      {
      }
    }
    
    if (ch == '\n' || feof(file))
    {
      continue;
    }
    
    fseek(file, -1, SEEK_CUR);
    char  buffer[INTERNAL::CONFIG_KEY_LENGTH] {};
    if (fscanf(file, INTERNAL::CONFIG_SCAN, buffer, value) != 2)
    {
      Error("Options: Error on line %zu of %s!", line, path);
      return (1);
    }
    
    if (!strcmp(value, "NONE"))
    {
      value[0] = 0;
    }
    
    if (!strcmp(buffer, key))
    {
      return (0);
    }
  }
  
  Error("Options: Didn't find %s in %s!", key, path);
  return (1);
}

static bool GetRaw(const char* path, FILE* file, const char* key, OUT char value[], i32 n)
{
  fseek(file, 0, SEEK_SET);
  
  for (usize line = 0; !feof(file) && !ferror(file); ++line)
  {
    i32 ch  {};
    while (ch = fgetc(file), ch != EOF && isspace(ch))
    {
    }
    
    if (ch == '#')
    {
      while (ch = fgetc(file), ch != EOF && ch != '\n')
      {
      }
    }
    
    if (ch == '\n' || feof(file))
    {
      continue;
    }
    
    fseek(file, -1, SEEK_CUR);
    char  buffer[INTERNAL::CONFIG_KEY_LENGTH] {};
    if (fscanf(file, INTERNAL::CONFIG_SCAN, buffer, value) != 2)
    {
      Error("Options: Error on line %zu of %s!", line, path);
      return (false);
    }
    
    if (!strcmp(value, "NONE"))
    {
      value[0] = 0;
    }
    
    if (!strcmp(buffer, key))
    {
      if (n > 0)
      {
        --n;
        continue;
      }
      
      return (true);
    }
  }
  
  return (false);
}

static i32  GetU32(const char* path, FILE* file, const char* key, OUT u32& value)
{
  char  buffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  if (GetRaw(path, file, key, buffer))
  {
    return (1);
  }
  
  errno = 0;
  unsigned long long  ull = strtoull(buffer, nullptr, 0);
  if (errno || ull > UINT32_MAX)
  {
    Error("Options: Invalid U32 value for %s in %s!", key, path);
    return (1);
  }
  
  value = ull;
  return (0);
}

static bool GetU32(const char* path, FILE* file, const char* key, OUT u32& value, i32 n)
{
  char  buffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  if (!GetRaw(path, file, key, buffer, n))
  {
    return (false);
  }
  
  errno = 0;
  unsigned long long  ull = strtoull(buffer, nullptr, 0);
  if (errno || ull > UINT32_MAX)
  {
    Error("Options: Invalid U32 value for %s in %s!", key, path);
    return (false);
  }
  
  value = ull;
  return (true);
}

static i32  GetColor(const char* path, FILE* file, const char* key, OUT u8& value)
{
  char  buffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  if (GetRaw(path, file, key, buffer))
  {
    return (1);
  }
  
  // check for named colors
  for (usize i = 0; i < ARRAY_SIZE(NAMED_COLORS); ++i)
  {
    if (!strcmp(buffer, NAMED_COLORS[i].m_Name))
    {
      value = NAMED_COLORS[i].m_Color;
      return (0);
    }
  }
  
  errno = 0;
  unsigned long long  ull = strtoull(buffer, nullptr, 0);
  if (errno || ull > UINT8_MAX)
  {
    Error("Options: Invalid color value for %s in %s!", key, path);
    return (1);
  }
  
  value = ull;
  return (0);
}

static i32  GetColorPair(const char* path, FILE* file, const char* key, OUT Color& value)
{
  char  buffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  if (GetRaw(path, file, key, buffer))
  {
    return (1);
  }
  
  char  fgBuffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  char  bgBuffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  if (sscanf(buffer, INTERNAL::CONFIG_COLOR_SCAN, fgBuffer, bgBuffer) != 2)
  {
    Error("Options: Wrongly formatted color pair value for %s in %s!", key, path);
    return (1);
  }
  
  // invalid color value used as default so that non-assignment can be detected
  constexpr u16 INVALID_COLOR = 256;
  u16           fg            = INVALID_COLOR;
  u16           bg            = INVALID_COLOR;
  
  // check for named colors
  for (usize i = 0; i < ARRAY_SIZE(NAMED_COLORS); ++i)
  {
    if (!strcmp(fgBuffer, NAMED_COLORS[i].m_Name))
    {
      fg = NAMED_COLORS[i].m_Color;
    }
    
    if (!strcmp(bgBuffer, NAMED_COLORS[i].m_Name))
    {
      bg = NAMED_COLORS[i].m_Color;
    }
  }
  
  if (fg == INVALID_COLOR)
  {
    errno = 0;
    unsigned long long  ull = strtoull(fgBuffer, nullptr, 0);
    if (errno || ull > UINT8_MAX)
    {
      Error("Options: Invalid FG color value for %s in %s!", key, path);
      return (1);
    }
    fg = ull;
  }
  
  if (bg == INVALID_COLOR)
  {
    errno = 0;
    unsigned long long  ull = strtoull(bgBuffer, nullptr, 0);
    if (errno || ull > UINT8_MAX)
    {
      Error("Options: Invalid BG color value for %s in %s!", key, path);
      return (1);
    }
    bg = ull;
  }
  
  value.m_FG = fg;
  value.m_BG = bg;
  return (0);
}

static i32  GetBool(const char* path, FILE* file, const char* key, OUT bool& value)
{
  char  buffer[INTERNAL::CONFIG_VALUE_LENGTH] {};
  if (GetRaw(path, file, key, buffer))
  {
    return (1);
  }
  
  if (!strcmp(buffer, "true"))
  {
    value = true;
    return (0);
  }
  else if (!strcmp(buffer, "false"))
  {
    value = false;
    return (0);
  }
  else
  {
    Error("Options: Invalid boolean value for %s in %s!", key, path);
    return (1);
  }
}

static void ReadLangConfig(FILE* file, const char* keywordKey, const char* primitiveKey, LangMode langMode)
{
  char  value[INTERNAL::CONFIG_VALUE_LENGTH]  {};
  
  for (i32 i = 0; GetRaw(FUNCTIONAL::LANG_CONF, file, keywordKey, value, i); ++i)
  {
    EString** keywords  = &g_Options.m_Lang[langMode].m_Keywords;
    usize*    nKeywords = &g_Options.m_Lang[langMode].m_NKeywords;
    
    ++*nKeywords;
    *keywords = (EString*)reallocarray(*keywords, *nKeywords, sizeof(EString));
    (*keywords)[*nKeywords - 1] = EString{value};
  }
  
  for (i32 i = 0; GetRaw(FUNCTIONAL::LANG_CONF, file, primitiveKey, value, i); ++i)
  {
    EString** primitives  = &g_Options.m_Lang[langMode].m_Primitives;
    usize*    nPrimitives = &g_Options.m_Lang[langMode].m_NPrimitives;
    
    ++*nPrimitives;
    *primitives = (EString*)reallocarray(*primitives, *nPrimitives, sizeof(EString));
  }
}
