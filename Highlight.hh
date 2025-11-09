// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Frame.hh>
#include <Options.hh>
#include <Util.hh>

struct Region
{
  u32   m_LowerBound;
  u32   m_UpperBound;
  Color m_Color;
};

Region  FindHighlight(const Frame& frame, u32 from);
