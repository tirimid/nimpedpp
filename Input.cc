// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdlib>
#include <cstring>
#include <Input.hh>
#include <Options.hh>

enum MacroMode : u8
{
  NO_MACRO  = 0,
  RECORDING_MACRO,
  EXECUTING_MACRO
};

struct BindData
{
  const EChar*  m_Bind;
  usize         m_Length;
  void          (*m_Function)();
};

static int  CompareBinds(const void* lhs, const void* rhs);

static BindData   g_Binds[FUNCTIONAL::MAX_BINDS];
static usize      g_NBinds;
static EChar      g_CurBind[FUNCTIONAL::MAX_BIND_LENGTH];
static usize      g_CurBindLength;
static MacroMode  g_MacroMode;
static EString    g_Macro;
static usize      g_CurMacroInstruction;

void  Unbind()
{
  g_NBinds = 0;
  g_CurBindLength = 0;
}

i32 Bind(const EChar* bind, void (*function)())
{
  if (g_NBinds >= FUNCTIONAL::MAX_BINDS)
  {
    Error("Input: Cannot register more than %u keybinds!", FUNCTIONAL::MAX_BINDS);
    return (1);
  }
  
  usize length  = 0;
  while (bind[length].m_Codepoint)
  {
    ++length;
  }
  
  for (usize i = 0; i < g_NBinds; ++i)
  {
    if (g_Binds[i].m_Length == length && !memcmp(bind, g_Binds[i].m_Bind, sizeof(EChar) * length))
    {
      g_Binds[i].m_Function = function;
      return (0);
    }
  }
  
  g_Binds[g_NBinds].m_Bind      = bind;
  g_Binds[g_NBinds].m_Length    = length;
  g_Binds[g_NBinds].m_Function  = function;
  ++g_NBinds;
  
  return (0);
}

void  OrganizeInputs()
{
  qsort(g_Binds, g_NBinds, sizeof(BindData), CompareBinds);
}

EChar ReadRawKey()
{
  if (g_MacroMode == EXECUTING_MACRO)
  {
    // the +1 handles the last key of the macro, which would just infinitely repeat the macro if executed
    if (g_CurMacroInstruction + 1 < g_Macro.m_Length)
    {
      EChar ch  = g_Macro.m_Data[g_CurMacroInstruction++];
      return (ch);
    }
    else
    {
      g_MacroMode = NO_MACRO;
      g_CurBindLength = 0;
      EChar ch  = ReadEChar();
      return (ch);
    }
  }
  
  EChar ch  = ReadEChar();
  
  if (g_MacroMode == RECORDING_MACRO)
  {
    g_Macro.Append(ch);
  }
  
  return (ch);
}

EChar ReadKey()
{
  EChar ch  = ReadRawKey();
  if (!g_NBinds)
  {
    return (ch);
  }
  
  if (g_CurBindLength < FUNCTIONAL::MAX_BIND_LENGTH)
  {
    g_CurBind[g_CurBindLength++] = ch;
  }
  else
  {
    g_CurBindLength = 0;
  }
  
  isize low   = 0;
  isize high  = g_NBinds - 1;
  isize mid   {};
  while (low <= high)
  {
    mid = (low + high) / 2;
    
    BindData  otherBind =
    {
      .m_Bind     = g_CurBind,
      .m_Length   = g_CurBindLength,
      .m_Function = nullptr
    };
    
    switch (CompareBinds(&otherBind, &g_Binds[mid]))
    {
    case (-1):
      high = mid - 1;
      break;
    case (0):
      goto found;
    case (1):
      low = mid + 1;
      break;
    }
  }
  
  mid = -1;
found:
  if (mid != -1)
  {
    const BindData* bind  = &g_Binds[mid];
    if (bind->m_Length == g_CurBindLength)
    {
      bind->m_Function();
      g_CurBindLength = 0;
    }
    
    return (REPLACEMENT_CHAR);
  }
  
  g_CurBindLength = 0;
  return (ch);
}

void  RecordMacro()
{
  g_Macro.Free();
  g_MacroMode = RECORDING_MACRO;
}

void  StopRecordingMacro()
{
  g_MacroMode = NO_MACRO;
}

bool  IsRecordingMacro()
{
  return (g_MacroMode == RECORDING_MACRO);
}

void  ExecuteMacro()
{
  g_MacroMode = EXECUTING_MACRO;
  g_CurMacroInstruction = 0;
}

static int  CompareBinds(const void* lhs, const void* rhs)
{
  const BindData* lhsBind = (BindData*)lhs;
  const BindData* rhsBind = (BindData*)rhs;
  
  for (usize i = 0; i < lhsBind->m_Length && i < rhsBind->m_Length; ++i)
  {
    if (lhsBind->m_Bind[i].m_Codepoint > rhsBind->m_Bind[i].m_Codepoint)
    {
      return (1);
    }
    else if (lhsBind->m_Bind[i].m_Codepoint < rhsBind->m_Bind[i].m_Codepoint)
    {
      return (-1);
    }
  }
  
  return (0);
}
