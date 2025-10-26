// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Util.hh>

extern "C"
{
#include <sys/stat.h>
}

void  Info(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char  msg[512]  = "Info: ";
  vsnprintf(&msg[5], sizeof(msg) - 6, fmt, args);
  
  // TODO: either display in editor or terminal
  
  va_end(args);
}

void  Error(const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  
  char  msg[512]  = "Error: ";
  vsnprintf(&msg[7], sizeof(msg) - 7, fmt, args);
  
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

i32 RecursiveCreateDir(const char* path)
{
  char* workingDir        = strdup(path);
  usize workingDirLength  = strlen(workingDir);
  if (!workingDirLength)
  {
    free(workingDir);
    return (0); // not doing anything in this case isn't really an error
  }
  
  for (usize i = 0, length = 0; i < workingDirLength; ++i)
  {
    if (workingDir[i] != '/')
    {
      ++length;
      continue;
    }
    
    if (!length)
    {
      continue;
    }
    
    workingDir[i] = 0;
    struct stat workingDirStat  {};
    if (!stat(workingDir, &workingDirStat) && S_ISDIR(workingDirStat.st_mode))
    {
      workingDir[i] = '/';
      continue;
    }
    
    mode_t  mode    = umask(0);
    i32     status  = mkdir(workingDir, 0755);
    umask(mode);
    
    if (status)
    {
      free(workingDir);
      return (1);
    }
    
    workingDir[i] = '/';
    length = 0;
  }
  
  free(workingDir);
  return (0);
}

i32 CreateFile(const char* path)
{
  struct stat pathStat  {};
  if (!stat(path, &pathStat))
  {
    return (!S_ISREG(pathStat.st_mode));
  }
  
  char* dir = strdup(path);
  if (strrchr(dir, '/'))
  {
    strrchr(dir, '/')[1] = 0;
  }
  
  // only need to create directory if it is specified in path
  if (strcmp(dir, path))
  {
    i32 dirStatus = RecursiveCreateDir(dir);
    free(dir);
    if (dirStatus)
    {
      return (1);
    }
  }
  
  free(dir);
  
  FILE* file  = fopen(path, "wb");
  if (!file)
  {
    return (1);
  }
  
  fclose(file);
  return (0);
}
