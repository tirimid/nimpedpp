// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>

struct Attributes
{
  u8  m_FG;
  u8  m_BG;
};

i32   InitRender();
void  QuitRender(bool clearScreen);
void  RenderFill(EChar ch, u32 x, u32 y, u32 w, u32 h);
void  RenderFill(Attributes a, u32 x, u32 y, u32 w, u32 h);
void  RenderFill(EChar ch, Attributes a, u32 x, u32 y, u32 w, u32 h);
void  RenderPut(EChar ch, u32 x, u32 y);
void  RenderPut(Attributes a, u32 x, u32 y);
void  RenderPut(EChar ch, Attributes a, u32 x, u32 y);
void  RenderGet(OUT EChar& ch, u32 x, u32 y);
void  RenderGet(OUT Attributes& a, u32 x, u32 y);
void  RenderGet(OUT EChar& ch, OUT Attributes& a, u32 x, u32 y);
void  RenderPresent();
void  WindowSize(OUT u32& width, OUT u32& height);
void  RenderBar(EString str);
void  RenderBar(const char* str);
