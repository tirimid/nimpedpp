// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Frame.hh>
#include <Options.hh>
#include <Util.hh>

struct Editor
{
  Frame   m_Frames[FUNCTIONAL::MAX_FILES];
  usize   m_CurFrame;
  usize   m_NFrames;
  EString m_Clipboard;
  bool    m_Running;
  bool    m_WriteInput;
};

extern Editor g_Editor;

i32     InitEditor();
void    EditorLoop();
void    ArrangeFrame(usize idx, OUT u32& x, OUT u32& y, OUT u32& w, OUT u32& h);
void    RenderEditor();
bool    IsWritable(EChar ch);
void    DestroyFrame(usize idx);
Frame&  CurrentFrame();
