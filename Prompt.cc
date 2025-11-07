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
  g_Prompt.m_Data   = str;
  g_Prompt.m_Start  = str.m_Length;
  g_Prompt.m_Cursor = str.m_Length;
  g_Prompt.m_Status = PROMPT_NONE;
}

void  BeginPrompt(const char* str)
{
  BeginPrompt(EString{str});
}

void  EndPrompt()
{
  g_Prompt.m_Cursor = -1;
}

void  PromptWrite(EChar ch, u32 pos)
{
  g_Prompt.m_Data.Insert(ch, pos);
}

void  PromptWrite(const EString& str, u32 pos)
{
  g_Prompt.m_Data.Insert(str, pos);
}

void  PromptWrite(const char* str, u32 pos)
{
  g_Prompt.m_Data.Insert(str, pos);
}

void  PromptErase(u32 lowerBound, u32 upperBound)
{
  g_Prompt.m_Data.Erase(lowerBound, upperBound);
}

void  RenderPrompt()
{
  RenderBar(g_Prompt.m_Data.Copy());
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

EString PromptData()
{
  EString data  = g_Prompt.m_Data.Copy();
  data.Erase(0, g_Prompt.m_Start);
  return (data);
}
