// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdarg>
#include <cstdio>
#include <Util.hh>

extern "C"
{
#include <sys/stat.h>
}

void  Info(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char  msg[512]  = "info: ";
  vsnprintf(&msg[5], sizeof(msg) - 6, fmt, args);
  
  // TODO: either display in editor or terminal
  
  va_end(args);
}

void  Error(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char  msg[512]  = "err: ";
  vsnprintf(&msg[5], sizeof(msg) - 5, fmt, args);
  
  // TODO: either display in editor or terminal
  
  va_end(args);
}

u64 FileID(const char* path, bool dereference)
{
  int (*statFunctions[])(const char*, struct stat*) =
  {
    lstat,
    stat
  };
  
  struct stat pathStat  {};
  if (statFunctions[dereference](path, &pathStat))
  {
    return (0);
  }
  
  return (pathStat.st_ino);
}

void  CreateFile(const char* path)
{
  // TODO: implement
}
