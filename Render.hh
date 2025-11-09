// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Options.hh>
#include <Util.hh>

i32   InitRender();
void  QuitRender(bool clearScreen);
void  RenderFill(EChar ch, u32 x, u32 y, u32 w, u32 h);
void  RenderFill(Color color, u32 x, u32 y, u32 w, u32 h);
void  RenderFill(EChar ch, Color color, u32 x, u32 y, u32 w, u32 h);
void  RenderPut(EChar ch, u32 x, u32 y);
void  RenderPut(Color color, u32 x, u32 y);
void  RenderPut(EChar ch, Color color, u32 x, u32 y);
void  RenderGet(OUT EChar& ch, u32 x, u32 y);
void  RenderGet(OUT Color& color, u32 x, u32 y);
void  RenderGet(OUT EChar& ch, OUT Color& color, u32 x, u32 y);
void  RenderPresent();
void  WindowSize(OUT u32& width, OUT u32& height);
void  RenderBar(OWNS EString str);
void  RenderBar(const char* str);
