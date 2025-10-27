// SPDX-License-Identifier: GPL-3.0-or-later

#include <Args.hh>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <Options.hh>

extern "C"
{
#include <pwd.h>
}

struct NamedColor
{
  const char* m_Name;
  u8          m_Color;
};

DynamicOptions  g_Options;

static FILE*  OpenConfig(const char* configPath);
static i32    GetRaw(const char* path, FILE* file, const char* key, OUT char value[]);
static bool   GetRaw(const char* path, FILE* file, const char* key, OUT char value[], i32 n);
static i32    GetU32(const char* path, FILE* file, const char* key, OUT u32& value);
static bool   GetU32(const char* path, FILE* file, const char* key, OUT u32& value, i32 n);
static i32    GetColor(const char* path, FILE* file, const char* key, OUT Color& value);
static i32    GetBool(const char* path, FILE* file, const char* key, OUT bool& value);
static void   ReadLangConfig(FILE* file, const char* keywordKey, const char* primitiveKey, LangMode langMode);

static const NamedColor g_NamedColors[] =
{
  // terminal
  {"Terminal::DarkBG",          0},
  {"Terminal::DarkRed",         1},
  {"Terminal::DarkGreen",       2},
  {"Terminal::DarkYellow",      3},
  {"Terminal::DarkBlue",        4},
  {"Terminal::DarkPurple",      5},
  {"Terminal::DarkAqua",        6},
  {"Terminal::DarkFG",          7},
  {"Terminal::LightBG",         8},
  {"Terminal::LightRed",        9},
  {"Terminal::LightGreen",      10},
  {"Terminal::LightYellow",     11},
  {"Terminal::LightBlue",       12},
  {"Terminal::LightPurple",     13},
  {"Terminal::LightAqua",       14},
  {"Terminal::LightFG",         15},
  
  // gruvbox dark
  {"GruvboxDark::BG",           235},
  {"GruvboxDark::DarkRed",      124},
  {"GruvboxDark::DarkGreen",    106},
  {"GruvboxDark::DarkYellow",   172},
  {"GruvboxDark::DarkBlue",     66},
  {"GruvboxDark::DarkPurple",   132},
  {"GruvboxDark::DarkAqua",     72},
  {"GruvboxDark::LightGray",    246},
  {"GruvboxDark::DarkGray",     245},
  {"GruvboxDark::LightRed",     167},
  {"GruvboxDark::LightGreen",   142},
  {"GruvboxDark::LightYellow",  214},
  {"GruvboxDark::LightBlue",    109},
  {"GruvboxDark::LightPurple",  175},
  {"GruvboxDark::LightAqua",    108},
  {"GruvboxDark::FG",           223},
  {"GruvboxDark::BG0_H",        234},
  {"GruvboxDark::BG1",          237},
  {"GruvboxDark::BG2",          239},
  {"GruvboxDark::BG3",          241},
  {"GruvboxDark::BG4",          243},
  {"GruvboxDark::DarkOrange",   166},
  {"GruvboxDark::BG0_S",        236},
  {"GruvboxDark::FG4",          246},
  {"GruvboxDark::FG3",          248},
  {"GruvboxDark::FG2",          250},
  {"GruvboxDark::FG1",          223},
  {"GruvboxDark::FG0",          229},
  {"GruvboxDark::LightOrange",  208},
  
  // github dark
  {"GithubDark::Base0",         233},
  {"GithubDark::Base1",         235},
  {"GithubDark::Base2",         237},
  {"GithubDark::Base3",         243},
  {"GithubDark::Base4",         249},
  {"GithubDark::Base5",         252},
  {"GithubDark::LightRed",      210},
  {"GithubDark::DarkRed",       203},
  {"GithubDark::DarkYellow",    178},
  {"GithubDark::LightYellow",   221},
  {"GithubDark::LightBlue",     153},
  {"GithubDark::DarkBlue",      75},
  {"GithubDark::LightPurple",   183},
  {"GithubDark::DarkPurple",    135},
  {"GithubDark::LightGreen",    114},
  {"GithubDark::DarkGreen",     76},
  
  // vs dark
  {"VSDark::BG",                233},
  {"VSDark::FG0",               188},
  {"VSDark::FG1",               108},
  {"VSDark::Beige",             180},
  {"VSDark::Aqua0",             50},
  {"VSDark::Aqua1",             108},
  {"VSDark::Purple",            169},
  {"VSDark::Green",             64},
  {"VSDark::Blue0",             152},
  {"VSDark::Blue1",             30},
  {"VSDark::Blue2",             39},
  {"VSDark::Yellow",            230},
  
  // ridiculous light
  {"RidiculousLight::BGLight",  254},
  {"RidiculousLight::BGDark",   251},
  {"RidiculousLight::BGNight",  249},
  {"RidiculousLight::FGLight",  237},
  {"RidiculousLight::FGDark",   233},
  {"RidiculousLight::Blue",     21},
  {"RidiculousLight::Red",      124},
  {"RidiculousLight::Orange",   130},
  {"RidiculousLight::Aqua",     65},
  {"RidiculousLight::Purple",   126}
};

i32 ParseOptions()
{
  // TODO: implement
  return (1);
}

static FILE*  OpenConfig(const char* configPath)
{
  char  path[PATH_MAX + 1]  {};
  
  if (g_Args.m_ConfigDir)
  {
    usize length  = snprintf(path, PATH_MAX, "%s", g_Args.m_ConfigDir);
    
    char  fixed[PATH_MAX + 1] {};
    if (length && path[length - 1] != '/')
    {
      snprintf(fixed, PATH_MAX, "%s/", path);
    }
    else
    {
      snprintf(fixed, PATH_MAX, "%s", path);
    }
    
    snprintf(path, PATH_MAX, "%s%s", fixed, configPath);
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
    
    snprintf(path, PATH_MAX, "%s/%s/%s", home, FUNCTIONAL::CONFIG_DIR, configPath);
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
  // TODO: implement
}

static bool GetU32(const char* path, FILE* file, const char* key, OUT u32& value, i32 n)
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
