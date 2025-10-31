// SPDX-License-Identifier: GPL-3.0-or-later

#include <Prompt.hh>
#include <Render.hh>

Prompt  g_Prompt =
{
  .m_Cursor = -1
};

void  BeginPrompt(OWNS EString str)
{
  g_Prompt.m_Data.Free();
  g_Prompt.m_Data = str;
  g_Prompt.m_Cursor = str.m_Length;
  g_Prompt.m_Status = PROMPT_NONE;
}

void  BeginPrompt(const char* str)
{
  EString eString = str;
  BeginPrompt(eString);
}

void  EndPrompt()
{
  g_Prompt.m_Cursor = -1;
}

void  PromptWrite(EChar ch, u32 pos)
{
  // TODO: implement
}

void  PromptWrite(const EString& str, u32 pos)
{
  // TODO: implement
}

void  PromptErase(u32 lowerBound, u32 upperBound)
{
  // TODO: implement
}

void  PromptRender()
{
  RenderBar(g_Prompt.m_Data.Copy());
}

EString PromptData()
{
  EString copy  = g_Prompt.m_Data.Copy();
  return (copy);
}

void  CompletePromptPath()
{
  // TODO: implement
}

bool  WritableToPrompt(EChar ch)
{
  bool  typeable  = ch.IsPrint();
  bool  ignored   = ch.m_Codepoint == REPLACEMENT_CHAR;
  return (typeable && !ignored);
}
