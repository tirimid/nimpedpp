// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>

enum PromptStatus
{
  PROMPT_NONE = 0,
  PROMPT_FAIL,
  PROMPT_SUCCESS
};

struct Prompt
{
  EString       m_Data    {};
  u32           m_Start   {};
  i32           m_Cursor  {};
  PromptStatus  m_Status  {};
  
  Prompt();
};

extern Prompt g_Prompt;
