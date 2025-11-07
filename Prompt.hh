// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>

enum PromptStatus : u8
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
};

extern Prompt g_Prompt;

void    BeginPrompt(OWNS EString str);
void    BeginPrompt(const char* str);
void    EndPrompt();
void    PromptWrite(EChar ch, u32 pos);
void    PromptWrite(const EString& str, u32 pos);
void    PromptWrite(const char* str, u32 pos);
void    PromptErase(u32 lowerBound, u32 upperBound);
void    RenderPrompt();
void    CompletePromptPath();
bool    WritableToPrompt(EChar ch);
EString PromptData();
