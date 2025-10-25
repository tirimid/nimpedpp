// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>

enum FrameFlag : u64
{
  FRAME_UNSAVED = 0x1
};

enum HistoryType : u8
{
  HISTORY_WRITE = 0,
  HISTORY_ERASE,
  HISTORY_BREAK
};

union History
{
  struct
  {
    HistoryType m_Type;
    u32         m_LowerBound;
    u32         m_UpperBound;
  }             m_Write;
  
  struct
  {
    HistoryType m_Type;
    u32         m_LowerBound;
    u32         m_UpperBound;
    EChar*      m_Data;
  }             m_Erase;
  
  HistoryType   m_Type;
};

struct Frame
{
  EString   m_Buffer;
  char*     m_Source;
  u32       m_Start;
  u32       m_Cursor;
  u32       m_SavedCursorX;
  u64       m_Flags;
  History*  m_History;
  u32       m_HistoryLength;
  u32       m_HistoryCapacity;
  
  void  Free();
  void  Render(u32 x, u32 y, u32 w, u32 h, bool active);
  i32   Save();
  void  Write(EChar ch, u32 pos);
  void  Write(const EString& str, u32 pos);
  void  Erase(u32 lb, u32 ub);
  void  Undo();
  void  BreakHistory();
  void  SaveCursor();
  void  LoadCursor();
  void  ComputeBounds(u32 w, u32 h);
  u32   Tabulate(u32 at);
};

Frame EmptyFrame();
Frame StringFrame(const char* str);
i32   FileFrame(OUT Frame& frame, const char* file);
