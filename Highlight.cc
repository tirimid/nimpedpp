// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstring>
#include <Highlight.hh>
#include <Options.hh>

constexpr const char* NUMBER_INIT   = "0123456789";
constexpr const char* NUMBER        = "xob+-.0123456789aAbBcCdDeEfF_";
constexpr const char* C_SPECIAL     = "!%&()*+,-./:;<=>?[\\]^{|}~";
constexpr const char* C_WORD_INIT   = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
constexpr const char* C_WORD        = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
constexpr const char* SH_SPECIAL    = "$\\=[]!><|;{}()*?~&`:.";
constexpr const char* SH_WORD_INIT  = C_WORD_INIT;
constexpr const char* SH_WORD       = C_WORD;
constexpr const char* JS_SPECIAL    = "()[]{};=+-*/%<>&^|?!~:,.";
constexpr const char* JS_WORD_INIT  = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$";
constexpr const char* JS_WORD       = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$0123456789";
constexpr const char* CC_SPECIAL    = C_SPECIAL;
constexpr const char* CC_WORD_INIT  = C_WORD_INIT;
constexpr const char* CC_WORD       = C_WORD_INIT;

static Region FindC(const Frame& frame, u32 from);
static Region FindCC(const Frame& frame, u32 from);
static Region FindSh(const Frame& frame, u32 from);
static Region FindJS(const Frame& frame, u32 from);
static bool   CompareString(const Frame& f, const char* cmp, u32 at);
static bool   CompareAny(const Frame& f, const char* cmp, u32 at);
static Region LineComment(const Frame& f, u32 from);
static Region Number(const Frame& f, u32 from);
static Region String(const Frame& f, u32 from, bool escape, bool newline);
static Region Special(const Frame& f, u32 from, const char* special);
static bool   TryKeyword(OUT Region& region, const Frame& f, u32 from, u32 end, LangMode langMode);
static Region CPreproc(const Frame& f, u32 from);
static Region CComment(const Frame& f, u32 from);
static Region CWord(const Frame& f, u32 from);
static Region ShWord(const Frame& f, u32 from);
static Region JSWord(const Frame& f, u32 from);
static Region CCWord(const Frame& f, u32 from);

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
  
  const char* extension = FileExtension(frame.m_Source);
  if (!strcmp(extension, "c") || !strcmp(extension, "h"))
  {
    Region  region  = FindC(frame, from);
    return (region);
  }
  else if (!strcmp(extension, "cc") || !strcmp(extension, "hh"))
  {
    Region  region  = FindCC(frame, from);
    return (region);
  }
  else if (!strcmp(extension, "sh"))
  {
    Region  region  = FindSh(frame, from);
    return (region);
  }
  else if (!strcmp(extension, "js"))
  {
    Region  region  = FindJS(frame, from);
    return (region);
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

static Region FindC(const Frame& frame, u32 from)
{
  // TODO: implement
  return (Region{});
}

static Region FindCC(const Frame& frame, u32 from)
{
  // TODO: implement
  return (Region{});
}

static Region FindSh(const Frame& frame, u32 from)
{
  // TODO: implement
  return (Region{});
}

static Region FindJS(const Frame& frame, u32 from)
{
  // TODO: implement
  return (Region{});
}

static bool CompareString(const Frame& f, const char* cmp, u32 at)
{
  // TODO: implement
}

static bool CompareAny(const Frame& f, const char* cmp, u32 at)
{
  // TODO: implement
}

static Region LineComment(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region Number(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region String(const Frame& f, u32 from, bool escape, bool newline)
{
  // TODO: implement
}

static Region Special(const Frame& f, u32 from, const char* special)
{
  // TODO: implement
}

static bool TryKeyword(OUT Region& region, const Frame& f, u32 from, u32 end, LangMode langMode)
{
  // TODO: implement
}

static Region CPreproc(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region CComment(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region CWord(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region ShWord(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region JSWord(const Frame& f, u32 from)
{
  // TODO: implement
}

static Region CCWord(const Frame& f, u32 from)
{
  // TODO: implement
}
