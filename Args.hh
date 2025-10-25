// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Options.hh>
#include <Util.hh>

struct Args
{
  const char* m_ConfigDir;
  const char* m_Files[FUNCTIONAL.MAX_FILES];
  usize       m_NFiles;
  bool        m_CreateFiles;
};

extern Args g_Args;

i32 ParseArgs(i32 argc, char* argv[]);
