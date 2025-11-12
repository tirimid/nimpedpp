// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Frame.hh>
#include <Highlight.hh>
#include <Render.hh>

void  Frame::Free()
{
  m_Buffer.Free();
  
  if (m_Source)
  {
    free(m_Source);
  }
  
  for (u32 i = 0; i < m_HistoryLength; ++i)
  {
    if (m_History[i].m_Data)
    {
      free(m_History[i].m_Data);
    }
  }
  free(m_History);
}

void  Frame::Render(u32 x, u32 y, u32 w, u32 h, bool active) const
{
  // render window top bar
  Color topColor  = active ? g_Options.m_CurrentWindow : g_Options.m_Window;
  RenderFill(' ', topColor, x, y, w, 1);
  
  EString name  {m_Source ? m_Source : VISUAL::SCRATCH_NAME};
  
  {
    u32 i {};
    for (i = 0; i < name.m_Length && i < w; ++i)
    {
      RenderPut(name.m_Data[i], x + i, y);
    }
    
    if (m_Flags & FRAME_UNSAVED)
    {
      i -= i >= w;
      RenderPut('*', x + i, y);
    }
  }
  
  name.Free();
  
  // fill frame and gutter
  u32 startLine = 1;
  for (u32 i = 0; i < m_Start; ++i)
  {
    startLine += m_Buffer.m_Data[i].m_Codepoint == '\n';
  }
  
  u32 lastLine  = 1;
  for (u32 i = 0; i < m_Buffer.m_Length; ++i)
  {
    lastLine += m_Buffer.m_Data[i].m_Codepoint == '\n';
  }
  
  u32 lineNumberLength  = 0;
  while (lastLine)
  {
    ++lineNumberLength;
    lastLine /= 10;
  }
  
  u32 totalGutter = g_Options.m_LeftGutter + g_Options.m_RightGutter;
  RenderFill(' ', g_Options.m_LineNumber, x, y + 1, lineNumberLength + totalGutter, h - 1);
  RenderFill(' ', g_Options.m_Normal, x + lineNumberLength + totalGutter, y + 1, w - lineNumberLength - totalGutter, h - 1);
  
  u32 leftPad = lineNumberLength + totalGutter;
  
  // render margins
  for (usize i = 0; i < g_Options.m_NMargins; ++i)
  {
    RenderFill(VISUAL::MARGIN_CHAR, g_Options.m_Margin, x + leftPad + g_Options.m_Margins[i], y + 1, 1, h - 1);
  }
  
  // render frame contents and find cursor position
  u32 cx      = 0;
  u32 cy      = 0;
  u32 cursorX = -1;
  u32 cursorY = -1;
  u32 curLine = startLine;
  
  Region  highlight = FindHighlight(*this, 0);
  while (highlight.m_UpperBound < m_Start)
  {
    highlight = FindHighlight(*this, highlight.m_UpperBound);
  }
  
  for (u32 i = m_Start; i < m_Buffer.m_Length; ++i)
  {
    if (i >= highlight.m_UpperBound)
    {
      highlight = FindHighlight(*this, highlight.m_UpperBound);
    }
    
    if (!cx)
    {
      char  lineNumber[32]  {};
      snprintf(lineNumber, sizeof(lineNumber), "%u", curLine);
      
      u32 pad = g_Options.m_LeftGutter + lineNumberLength - strlen(lineNumber);
      for (u32 j = 0; lineNumber[j]; ++j)
      {
        RenderPut((u32)lineNumber[j], x + pad + j, y + cy + 1);
      }
      
      ++curLine;
    }
    
    if (leftPad + cx >= w)
    {
      cx = 0;
      ++cy;
    }
    
    if (cy + 1 >= h)
    {
      break;
    }
    
    if (i == m_Cursor)
    {
      cursorX = cx;
      cursorY = cy;
    }
    
    u32 cw  {};
    switch (m_Buffer.m_Data[i].m_Codepoint)
    {
    case ('\n'):
      cx = 0;
      ++cy;
      continue;
    case ('\t'):
      cw = g_Options.m_TabSize - cx % g_Options.m_TabSize;
      break;
    default:
      cw = 1;
      if (i >= highlight.m_LowerBound && i < highlight.m_UpperBound)
      {
        RenderPut(highlight.m_Color, x + leftPad + cx, y + cy + 1);
      }
      else
      {
        RenderPut(g_Options.m_Normal, x + leftPad + cx, y + cy + 1);
      }
      RenderPut(m_Buffer.m_Data[i].IsPrint() ? m_Buffer.m_Data[i] : REPLACEMENT_CHAR, x + leftPad + cx, y + cy + 1);
      break;
    }
    
    cx += cw;
  }
  
  cursorX = cursorX == (u32)-1 ? cx : cursorX;
  cursorY = cursorY == (u32)-1 ? cy : cursorY;
  
  // render cursor and row highlights
  RenderFill(g_Options.m_LineNumberHighlight, x, y + cursorY + 1, leftPad, 1);
  for (u32 i = 0; leftPad + i < w; ++i)
  {
    Color color {};
    RenderGet(color, x + leftPad + i, y + cursorY + 1);
    color.m_BG = color.m_BG == g_Options.m_Normal.m_BG ? g_Options.m_CursorHighlightBG : color.m_BG;
    RenderPut(color, x + leftPad + i, y + cursorY + 1);
  }
  RenderPut(g_Options.m_Cursor, x + leftPad + cursorX, y + cursorY + 1);
}

i32 Frame::Save()
{
  if (!m_Source)
  {
    Error("Frame: Cannot save frame with no source!");
    return (1);
  }
  
  FILE* file  = fopen(m_Source, "wb");
  if (!file)
  {
    Error("Frame: Failed to open file source for writing: %s!", m_Source);
    return (1);
  }
  
  for (usize i = 0; i < m_Buffer.m_Length; ++i)
  {
    if (PrintEChar(file, m_Buffer.m_Data[i]))
    {
      Error("Frame: Failed to write file, take care not to lose data: %s!", m_Source);
      fclose(file);
      return (1);
    }
  }
  
  fclose(file);
  m_Flags &= ~FRAME_UNSAVED;
  
  return (0);
}

void  Frame::Write(EChar ch, u32 pos)
{
  EString str {};
  str.m_Data    = &ch;
  str.m_Length  = 1;
  Write(str, pos);
}

void  Frame::Write(const EString& str, u32 pos)
{
  // modify buffer
  m_Buffer.Insert(str, pos);
  m_Flags |= FRAME_UNSAVED;
  
  // push history entry
  TruncateHistory();
  History*  history = m_HistoryLength ? &m_History[m_HistoryLength - 1] : nullptr;
  if (history && history->m_Type == HISTORY_WRITE && history->m_UpperBound == pos)
  {
    u32 newUpperBound = pos + str.m_Length;
    history->m_Data = (EChar*)reallocarray(history->m_Data, newUpperBound - history->m_LowerBound, sizeof(EChar));
    memcpy(&history->m_Data[history->m_UpperBound - history->m_LowerBound], str.m_Data, sizeof(EChar) * str.m_Length);
    history->m_UpperBound = newUpperBound;
  }
  else
  {
    if (m_HistoryLength >= m_HistoryCapacity)
    {
      m_HistoryCapacity *= 2;
      m_History = (History*)reallocarray(m_History, m_HistoryCapacity, sizeof(History));
    }
    
    m_History[m_HistoryLength] = (History)
    {
      .m_Data       = str.CopyData(),
      .m_LowerBound = pos,
      .m_UpperBound = pos + str.m_Length,
      .m_Type       = HISTORY_WRITE
    };
    ++m_HistoryLength;
    ++m_CurHistory;
  }
}

void  Frame::Write(const char* str, u32 pos)
{
  EString eString {str};
  Write(eString, pos);
  eString.Free();
}

void  Frame::Erase(u32 lb, u32 ub)
{
  // push history entry
  History*  history = m_HistoryLength ? &m_History[m_HistoryLength - 1] : nullptr;
  if (history && history->m_Type == HISTORY_ERASE && history->m_LowerBound == ub)
  {
    history->m_Data = (EChar*)reallocarray(history->m_Data, history->m_UpperBound - lb, sizeof(EChar));
    memmove(&history->m_Data[ub - lb], history->m_Data, sizeof(EChar) * (history->m_UpperBound - history->m_LowerBound));
    memcpy(history->m_Data, &m_Buffer.m_Data[lb], sizeof(EChar) * (ub - lb));
    history->m_LowerBound = lb;
  }
  else
  {
    if (m_HistoryLength >= m_HistoryCapacity)
    {
      m_HistoryCapacity *= 2;
      m_History = (History*)reallocarray(m_History, m_HistoryCapacity, sizeof(History));
    }
    
    EChar*  data  = (EChar*)calloc(ub - lb, sizeof(EChar));
    memcpy(data, &m_Buffer.m_Data[lb], sizeof(EChar) * (ub - lb));
    m_History[m_HistoryLength] = (History)
    {
      .m_Data       = data,
      .m_LowerBound = lb,
      .m_UpperBound = ub,
      .m_Type       = HISTORY_ERASE
    };
    ++m_HistoryLength;
    ++m_CurHistory;
  }
  
  // modify buffer
  m_Buffer.Erase(lb, ub);
  m_Flags |= FRAME_UNSAVED;
}

void  Frame::Erase(u32 pos)
{
  Erase(pos, pos + 1);
}

void  Frame::Undo()
{
  if (m_HistoryLength == 0)
  {
    return;
  }
  
  while (m_CurHistory > 0 && m_History[m_CurHistory - 1].m_Type == HISTORY_BREAK)
  {
    --m_CurHistory;
  }
  
  if (m_CurHistory == 0)
  {
    return;
  }
  
  const History*  history = &m_History[m_CurHistory - 1];
  switch (history->m_Type)
  {
  case (HISTORY_WRITE):
    m_Buffer.Erase(history->m_LowerBound, history->m_UpperBound);
    m_Cursor = history->m_LowerBound;
    m_Flags |= FRAME_UNSAVED;
    break;
  case (HISTORY_ERASE):
    m_Buffer.Insert(history->m_Data, history->m_UpperBound - history->m_LowerBound, history->m_LowerBound);
    m_Cursor = history->m_UpperBound;
    m_Flags |= FRAME_UNSAVED;
    break;
  default:
    break;
  }
  
  --m_CurHistory;
}

void  Frame::Redo()
{
  while (m_CurHistory < m_HistoryLength && m_History[m_CurHistory].m_Type == HISTORY_BREAK)
  {
    ++m_CurHistory;
  }
  
  if (m_CurHistory == m_HistoryLength)
  {
    return;
  }
  
  const History*  history = &m_History[m_CurHistory];
  switch (history->m_Type)
  {
  case (HISTORY_ERASE):
    m_Buffer.Erase(history->m_LowerBound, history->m_UpperBound);
    m_Cursor = history->m_LowerBound;
    m_Flags |= FRAME_UNSAVED;
    break;
  case (HISTORY_WRITE):
    m_Buffer.Insert(history->m_Data, history->m_UpperBound - history->m_LowerBound, history->m_LowerBound);
    m_Cursor = history->m_UpperBound;
    m_Flags |= FRAME_UNSAVED;
    break;
  default:
    break;
  }
  
  ++m_CurHistory;
}

void  Frame::BreakHistory()
{
  if (m_HistoryLength >= m_HistoryCapacity)
  {
    m_HistoryCapacity *= 2;
    m_History = (History*)reallocarray(m_History, m_HistoryCapacity, sizeof(History));
  }
  
  TruncateHistory();
  m_History[m_HistoryLength] = (History)
  {
    .m_Data       = nullptr,
    .m_LowerBound = 0,
    .m_UpperBound = 0,
    .m_Type       = HISTORY_BREAK
  };
  ++m_HistoryLength;
  ++m_CurHistory;
}

void  Frame::TruncateHistory()
{
  for (usize i = m_CurHistory; i < m_HistoryLength; ++i)
  {
    if (m_History[i].m_Data)
    {
      free(m_History[i].m_Data);
    }
  }
  m_HistoryLength = m_CurHistory;
}

void  Frame::SaveCursor()
{
  u32 lineBegin = m_Cursor;
  while (lineBegin > 0 && m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
  {
    --lineBegin;
  }
  
  u32 cx = 0;
  for (u32 i = lineBegin; i < m_Cursor; ++i)
  {
    switch (m_Buffer.m_Data[i].m_Codepoint)
    {
    case ('\t'):
      cx += g_Options.m_TabSize - cx % g_Options.m_TabSize;
      break;
    default:
      ++cx;
      break;
    }
  }
  
  m_SavedCursorX = cx;
}

void  Frame::LoadCursor()
{
  u32 lineBegin = m_Cursor;
  while (lineBegin > 0 && m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
  {
    --lineBegin;
  }
  
  u32 i = lineBegin;
  for (u32 cx = 0; i < m_Buffer.m_Length && m_Buffer.m_Data[i].m_Codepoint != '\n' && cx < m_SavedCursorX; ++i)
  {
    switch (m_Buffer.m_Data[i].m_Codepoint)
    {
    case ('\t'):
      cx += g_Options.m_TabSize - cx % g_Options.m_TabSize;
      break;
    default:
      ++cx;
      break;
    }
  }
  
  m_Cursor = i;
}

void  Frame::ComputeBounds(u32 w, u32 h)
{
  if (m_Cursor < m_Start)
  {
    m_Start = m_Cursor;
    while (m_Start > 0 && m_Buffer.m_Data[m_Start - 1].m_Codepoint != '\n')
    {
      --m_Start;
    }
    return;
  }
  
  u32 lastLine  = 1;
  for (u32 i = 0; i < m_Buffer.m_Length; ++i)
  {
    lastLine += m_Buffer.m_Data[i].m_Codepoint == '\n';
  }
  
  u32 lineNumberLength  = 0;
  while (lastLine)
  {
    ++lineNumberLength;
    lastLine /= 10;
  }
  
  u32 leftPad = g_Options.m_LeftGutter + g_Options.m_RightGutter + lineNumberLength;
  
  u32 cx  = 0;
  u32 cy  = 0;
  for (usize i = m_Start; i < m_Cursor; ++i)
  {
    if (leftPad + cx >= w)
    {
      cx = 0;
      ++cy;
    }
    
    u32 cw  {};
    switch (m_Buffer.m_Data[i].m_Codepoint)
    {
    case ('\n'):
      cx = 0;
      ++cy;
      continue;
    case ('\t'):
      cw = g_Options.m_TabSize - cx % g_Options.m_TabSize;
      break;
    default:
      cw = 1;
      break;
    }
    
    cx += cw;
  }
  
  for (cx = 0; cy + 1 >= h; ++m_Start)
  {
    if (leftPad + cx >= w)
    {
      cx = 0;
      --cy;
    }
    
    u32 cw  {};
    switch (m_Buffer.m_Data[m_Start].m_Codepoint)
    {
    case ('\n'):
      cx = 0;
      --cy;
      continue;
    case ('\t'):
      cw = g_Options.m_TabSize - cx % g_Options.m_TabSize;
      break;
    default:
      cw = 1;
      break;
    }
    
    cx += cw;
  }
}

u32 Frame::Tabulate(u32 at)
{
  if (g_Options.m_TabSpaces)
  {
    u32 lineBegin = at;
    while (lineBegin && m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
    {
      --lineBegin;
    }
    
    u32 linePos = at - lineBegin;
    u32 nSpaces = g_Options.m_TabSize - linePos % g_Options.m_TabSize;
    while (nSpaces)
    {
      Write(' ', at);
      ++at;
      --nSpaces;
    }
    
    return (at);
  }
  else
  {
    Write('\t', at);
    return (at + 1);
  }
}

void  EmptyFrame(OUT Frame& frame)
{
  frame = (Frame)
  {
    .m_Buffer           = EString{},
    .m_Source           = nullptr,
    .m_Start            = 0,
    .m_Cursor           = 0,
    .m_SavedCursorX     = 0,
    .m_Flags            = 0,
    .m_History          = (History*)calloc(1, sizeof(History)),
    .m_HistoryLength    = 0,
    .m_HistoryCapacity  = 1,
    .m_CurHistory       = 0
  };
}

void  StringFrame(OUT Frame& frame, const char* str)
{
  frame = (Frame)
  {
    .m_Buffer           = EString{str},
    .m_Source           = nullptr,
    .m_Start            = 0,
    .m_Cursor           = 0,
    .m_SavedCursorX     = 0,
    .m_Flags            = 0,
    .m_History          = (History*)calloc(1, sizeof(History)),
    .m_HistoryLength    = 0,
    .m_HistoryCapacity  = 1,
    .m_CurHistory       = 0
  };
}

i32 FileFrame(OUT Frame& frame, const char* path)
{
  FILE* file  = fopen(path, "rb");
  if (!file)
  {
    Error("Frame: Failed to open file to read: %s!", path);
    return (1);
  }
  
  EString buffer  {};
  for (;;)
  {
    EChar ch  = ReadEChar(file);
    
    if (feof(file))
    {
      break;
    }
    
    if (ferror(file))
    {
      Error("Frame: Experienced a read failure for file: %s!", path);
      fclose(file);
      buffer.Free();
      return (1);
    }
    
    buffer.Insert(ch, buffer.m_Length);
  }
  
  fclose(file);
  
  frame = (Frame)
  {
    .m_Buffer           = buffer,
    .m_Source           = strdup(path),
    .m_Start            = 0,
    .m_Cursor           = 0,
    .m_SavedCursorX     = 0,
    .m_Flags            = 0,
    .m_History          = (History*)calloc(1, sizeof(History)),
    .m_HistoryLength    = 0,
    .m_HistoryCapacity  = 1,
    .m_CurHistory       = 0
  };
  return (0);
}
