// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <Args.hh>
#include <getopt.h>

Args  g_Args;

static void Usage(const char* name);

i32 ParseArgs(i32 argc, char* argv[])
{
  i32 ch  {};
  while (ch = getopt(argc, (char* const*)argv, "cho:"), ch != -1)
  {
    switch (ch)
    {
    case ('c'):
      g_Args.m_CreateFiles = true;
      break;
    case ('h'):
      Usage(argv[0]);
      exit(0);
    case ('o'):
      g_Args.m_ConfigDir = optarg;
      break;
    default:
      return (1);
    }
  }
  
  for (; optind < argc; ++optind)
  {
    if (g_Args.m_NFiles >= FUNCTIONAL::MAX_FILES)
    {
      Error("Args: Cannot open more than %u files!", FUNCTIONAL::MAX_FILES);
      return (1);
    }
    
    g_Args.m_Files[g_Args.m_NFiles++] = argv[optind];
  }
  
  for (usize i = 0; i < g_Args.m_NFiles; ++i)
  {
    for (usize j = i + 1; j < g_Args.m_NFiles; ++j)
    {
      u64 firstID   = FileID(g_Args.m_Files[i], true);
      u64 secondID  = FileID(g_Args.m_Files[j], true);
      if (firstID == secondID)
      {
        Error("Args: Cannot open the same file twice: %s!", g_Args.m_Files[i]);
        return (1);
      }
    }
  }
  
  return (0);
}

static void Usage(const char* name)
{
  fprintf(
    stderr,
    "nimped++\n"
    "Referenced as: %s\n"
    "\n"
    "Usage:\n"
    "  %s [options] files\n"
    "\n"
    "Options:\n"
    "  -c           Create files if they don't exist\n"
    "  -h           Display this help information\n"
    "  -o dir       Use a different config directory\n"
    "\n"
    "Additional resources:\n"
    "  Source code  https://git.tirimid.net/nimpedpp\n",
    name,
    name
  );
}
