// SPDX-License-Identifier: GPL-3.0-or-later

#include <Highlight.hh>

Region  FindHighlight(const Frame& frame, u32 from)
{
  if (!frame.m_Source)
  {
    Region  region  =
    {
      .m_LowerBound = frame.m_Buffer.m_Length,
      .m_UpperBound = frame.m_Buffer.m_Length,
      .m_FG         = 0,
      .m_BG         = 0
    };
    return (region);
  }
  
  if (false)
  {
    // TODO: implement file extension highlight
  }
  else
  {
    Region  region  =
    {
      .m_LowerBound = frame.m_Buffer.m_Length,
      .m_UpperBound = frame.m_Buffer.m_Length,
      .m_FG         = 0,
      .m_BG         = 0
    };
    return (region);
  }
}
