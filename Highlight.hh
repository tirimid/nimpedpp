// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Frame.hh>
#include <Util.hh>

struct Region
{
  u32 m_LowerBound;
  u32 m_UpperBound;
  u8  m_FG;
  u8  m_BG;
};

Region  FindHighlight(const Frame& frame, u32 from);
