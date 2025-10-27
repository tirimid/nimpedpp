// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include <cstdlib>
#include <Frame.hh>
#include <Render.hh>

void  Frame::Free()
{
  m_Buffer.Free();
  if (m_Source)
  {
    free(m_Source);
  }
  free(m_History);
}

void  Frame::Render(u32 x, u32 y, u32 w, u32 h, bool active) const
{
  // render window top bar
  Attributes  attrTop {active ? g_Options.m_CurrentWindow : g_Options.m_Window};
  RenderFill(' ', attrTop, x, y, w, 1);
  
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
  RenderFill(' ', g_Options.m_LineNumber, x, y + w, lineNumberLength + totalGutter, h - 1);
  RenderFill(' ', g_Options.m_Normal, x + lineNumberLength + totalGutter, y + 1, w - lineNumberLength - totalGutter, h - 1);
  
  u32 leftPad = lineNumberLength + totalGutter;
  
  // render margins
  for (usize i = 0; i < g_Options.m_NMargins; ++i)
  {
    RenderFill(' ', g_Options.m_Margin, x + leftPad + g_Options.m_Margins[i], y + 1, 1, h - 1);
  }
  
  // TODO: finish implementing
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
  // TODO: implement
}

void  Frame::Write(const EString& str, u32 pos)
{
  // TODO: implement
}

void  Frame::Erase(u32 lb, u32 ub)
{
  // TODO: implement
}

void  Frame::Undo()
{
  // TODO: implement
}

void  Frame::BreakHistory()
{
  // TODO: implement
}

void  Frame::SaveCursor()
{
  // TODO: implement
}

void  Frame::LoadCursor()
{
  // TODO: implement
}

void  Frame::ComputeBounds(u32 w, u32 h)
{
  // TODO: implement
}

u32 Frame::Tabulate(u32 at)
{
  // TODO: implement
}

Frame EmptyFrame()
{
  Frame frame =
  {
    .m_Buffer           = EString{},
    .m_Source           = nullptr,
    .m_Start            = 0,
    .m_Cursor           = 0,
    .m_SavedCursorX     = 0,
    .m_Flags            = 0,
    .m_History          = (History*)calloc(1, sizeof(History)),
    .m_HistoryLength    = 0,
    .m_HistoryCapacity  = 1
  };
  return (frame);
}

Frame StringFrame(const char* str)
{
  Frame frame =
  {
    .m_Buffer           = EString{str},
    .m_Source           = nullptr,
    .m_Start            = 0,
    .m_Cursor           = 0,
    .m_SavedCursorX     = 0,
    .m_Flags            = 0,
    .m_History          = (History*)calloc(1, sizeof(History)),
    .m_HistoryLength    = 0,
    .m_HistoryCapacity  = 1
  };
  return (frame);
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
    
    buffer.Append(ch);
  }
  
  fclose(file);
  
  frame = (Frame)
  {
    .m_Buffer           = buffer,
    .m_Source           = nullptr,
    .m_Start            = 0,
    .m_Cursor           = 0,
    .m_SavedCursorX     = 0,
    .m_Flags            = 0,
    .m_History          = (History*)calloc(1, sizeof(History)),
    .m_HistoryLength    = 0,
    .m_HistoryCapacity  = 1
  };
  return (0);
}
