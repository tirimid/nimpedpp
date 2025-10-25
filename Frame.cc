// SPDX-License-Identifier: GPL-3.0-or-later

#include <Frame.hh>

void  Frame::Free()
{
  // TODO: implement
}

void  Frame::Render(u32 x, u32 y, u32 w, u32 h, bool active)
{
  // TODO: implement
}

i32 Frame::Save()
{
  // TODO: implement
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
  // TODO: implement
}

Frame StringFrame()
{
  // TODO: implement
}

i32 FileFrame(OUT Frame& frame, const char* file)
{
  // TODO: implement
}
