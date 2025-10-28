// SPDX-License-Identifier: GPL-3.0-or-later

#include <Args.hh>
#include <Binds.hh>
#include <cstring>
#include <Editor.hh>
#include <Input.hh>
#include <Render.hh>

Editor  g_Editor;

i32 InitEditor()
{
  for (usize i = 0; i < g_Args.m_NFiles; ++i)
  {
    if (g_Args.m_CreateFiles)
    {
      CreateFile(g_Args.m_Files[i]);
    }
    
    if (FileFrame(g_Editor.m_Frames[i], g_Args.m_Files[i]))
    {
      return (1);
    }
    
    ++g_Editor.m_NFrames;
  }
  
  if (g_Editor.m_NFrames)
  {
    StringFrame(g_Editor.m_Frames[g_Editor.m_NFrames++], FRAME::GREETER_TEXT);
  }
  
  InstallBaseBinds();
  
  return (0);
}

void  EditorLoop()
{
  g_Editor.m_Running = true;
  while (g_Editor.m_Running)
  {
    RenderEditor();
    RenderPresent();
    
    EChar input = ReadKey();
    if (g_Editor.m_WriteInput && IsWritable(input))
    {
      // TODO: echo input characters into buffer when m_WriteInput
    }
  }
}

void  ArrangeFrame(usize idx, OUT u32& x, OUT u32& y, OUT u32& w, OUT u32& h)
{
  u32 renderWidth   {};
  u32 renderHeight  {};
  WindowSize(renderWidth, renderHeight);
  
  if (g_Editor.m_NFrames == 1)
  {
    x = 0;
    y = 0;
    w = renderWidth;
    h = renderHeight;
    return;
  }
  
  if (!idx)
  {
    x = 0;
    y = 0;
    w = g_Options.m_MasterNumer * renderWidth / g_Options.m_MasterDenom;
    h = renderHeight;
  }
  else
  {
    h = renderHeight / (g_Editor.m_NFrames - 1);
    y = (idx - 1) * h;
    x = g_Options.m_MasterNumer * renderWidth / g_Options.m_MasterDenom;
    w = renderWidth - x;
    
    if (x + y > renderHeight || idx == g_Editor.m_NFrames - 1)
    {
      h = renderHeight - y;
    }
  }
}

void  RenderEditor()
{
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    u32 x {};
    u32 y {};
    u32 w {};
    u32 h {};
    ArrangeFrame(i, x, y, w, h);
    
    g_Editor.m_Frames[i].ComputeBounds(w, h);
    g_Editor.m_Frames[i].Render(x, y, w, h, i == g_Editor.m_CurFrame);
  }
}

bool  IsWritable(EChar ch)
{
  // input returns replacement unicode character on non-writing key, and it is rare that a user wants to input a replacement
  // character, so they are all just considered non-writable.
  bool  typeable  = ch.m_Codepoint == '\t' || ch.IsPrint();
  bool  ignored   = ch.m_Codepoint == REPLACEMENT_CHAR;
  return (typeable && !ignored);
}

void  DestroyFrame(usize idx)
{
  g_Editor.m_Frames[idx].Free();
  
  if (g_Editor.m_NFrames == 1)
  {
    EmptyFrame(g_Editor.m_Frames[0]);
    return;
  }
  
  memmove(&g_Editor.m_Frames[idx], &g_Editor.m_Frames[idx + 1], sizeof(Frame) * (g_Editor.m_NFrames - idx));
  --g_Editor.m_NFrames;
  
  if (g_Editor.m_CurFrame && g_Editor.m_CurFrame >= idx)
  {
    --g_Editor.m_CurFrame;
  }
}
