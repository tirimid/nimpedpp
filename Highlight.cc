// SPDX-License-Identifier: GPL-3.0-or-later

#include <cctype>
#include <cstring>
#include <Highlight.hh>

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
constexpr const char* CC_WORD       = C_WORD;

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
static bool   TryKeyword(OUT Region& region, const Frame& f, u32 from, u32 end, LangMode lang);
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
      .m_Color      {}
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
      .m_Color      {}
    };
    return (region);
  }
}

static Region FindC(const Frame& frame, u32 from)
{
  for (u32 i = from; i < frame.m_Buffer.m_Length; ++i)
  {
    if (CompareString(frame, "//", i))
    {
      Region  region  = LineComment(frame, i);
      return (region);
    }
    else if (CompareString(frame, "/*", i))
    {
      Region  region  = CComment(frame, i);
      return (region);
    }
    else if (CompareAny(frame, NUMBER_INIT, i))
    {
      Region  region  = Number(frame, i);
      return (region);
    }
    else if (CompareString(frame, "#", i))
    {
      Region  region  = CPreproc(frame, i);
      return (region);
    }
    else if (CompareAny(frame, C_SPECIAL, i))
    {
      Region  region  = Special(frame, i, C_SPECIAL);
      return (region);
    }
    else if (CompareAny(frame, C_WORD_INIT, i))
    {
      Region  region  = CWord(frame, i);
      return (region);
    }
    else if (CompareAny(frame, "\"'", i))
    {
      Region  region  = String(frame, i, true, false);
      return (region);
    }
  }
  
  Region  region  =
  {
    .m_LowerBound = frame.m_Buffer.m_Length,
    .m_UpperBound = frame.m_Buffer.m_Length,
    .m_Color      {}
  };
  return (region);
}

static Region FindCC(const Frame& frame, u32 from)
{
  for (u32 i = from; i < frame.m_Buffer.m_Length; ++i)
  {
    if (CompareString(frame, "//", i))
    {
      Region  region  = LineComment(frame, i);
      return (region);
    }
    else if (CompareString(frame, "/*", i))
    {
      Region  region  = CComment(frame, i);
      return (region);
    }
    else if (CompareAny(frame, NUMBER_INIT, i))
    {
      Region  region  = Number(frame, i);
      return (region);
    }
    else if (CompareString(frame, "#", i))
    {
      Region  region  = CPreproc(frame, i);
      return (region);
    }
    else if (CompareAny(frame, CC_SPECIAL, i))
    {
      Region  region  = Special(frame, i, CC_SPECIAL);
      return (region);
    }
    else if (CompareAny(frame, CC_WORD_INIT, i))
    {
      Region  region  = CCWord(frame, i);
      return (region);
    }
    else if (CompareAny(frame, "\"'", i))
    {
      Region  region  = String(frame, i, true, false);
      return (region);
    }
  }
  
  Region  region  =
  {
    .m_LowerBound = frame.m_Buffer.m_Length,
    .m_UpperBound = frame.m_Buffer.m_Length,
    .m_Color      {}
  };
  return (region);
}

static Region FindSh(const Frame& frame, u32 from)
{
  for (u32 i = from; i < frame.m_Buffer.m_Length; ++i)
  {
    if (CompareString(frame, "#", i))
    {
      Region  region  = LineComment(frame, i);
      return (region);
    }
    else if (CompareAny(frame, NUMBER_INIT, i))
    {
      Region  region  = Number(frame, i);
      return (region);
    }
    else if (CompareString(frame, "'", i))
    {
      Region  region  = String(frame, i, false, true);
      return (region);
    }
    else if (CompareString(frame, "\"", i))
    {
      Region  region  = String(frame, i, true, true);
      return (region);
    }
    else if (CompareAny(frame, SH_SPECIAL, i))
    {
      Region  region  = Special(frame, i, SH_SPECIAL);
      return (region);
    }
    else if (CompareAny(frame, SH_WORD_INIT, i))
    {
      Region  region  = ShWord(frame, i);
      return (region);
    }
  }
  
  Region  region  =
  {
    .m_LowerBound = frame.m_Buffer.m_Length,
    .m_UpperBound = frame.m_Buffer.m_Length,
    .m_Color      {}
  };
  return (region);
}

static Region FindJS(const Frame& frame, u32 from)
{
  for (u32 i = from; i < frame.m_Buffer.m_Length; ++i)
  {
    // regex highlight will not be implemented
    
    if (CompareString(frame, "//", i) || CompareString(frame, "#!", i))
    {
      Region  region  = LineComment(frame, i);
      return (region);
    }
    else if (CompareString(frame, "/*", i))
    {
      Region  region  = CComment(frame, i);
      return (region);
    }
    else if (CompareAny(frame, NUMBER_INIT, i))
    {
      Region  region  = Number(frame, i);
      return (region);
    }
    else if (CompareAny(frame, JS_WORD_INIT, i))
    {
      Region  region  = JSWord(frame, i);
      return (region);
    }
    else if (CompareAny(frame, "\"'`", i))
    {
      Region  region  = String(frame, i, true, false);
      return (region);
    }
    else if (CompareAny(frame, JS_SPECIAL, i))
    {
      Region  region  = Special(frame, i, JS_SPECIAL);
      return (region);
    }
  }
  
  Region  region  =
  {
    .m_LowerBound = frame.m_Buffer.m_Length,
    .m_UpperBound = frame.m_Buffer.m_Length,
    .m_Color      {}
  };
  return (region);
}

static bool CompareString(const Frame& f, const char* cmp, u32 at)
{
  for (; *cmp && at < f.m_Buffer.m_Length; ++at, ++cmp)
  {
    if ((u32)*cmp != f.m_Buffer.m_Data[at].m_Codepoint)
    {
      return (false);
    }
  }
  return (!*cmp);
}

static bool CompareAny(const Frame& f, const char* cmp, u32 at)
{
  for (; *cmp; ++cmp)
  {
    if ((u32)*cmp == f.m_Buffer.m_Data[at].m_Codepoint)
    {
      return (true);
    }
  }
  return (false);
}

static Region LineComment(const Frame& f, u32 from)
{
  u32 end = from;
  while (end < f.m_Buffer.m_Length && f.m_Buffer.m_Data[end].m_Codepoint != '\n')
  {
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Comment
  };
  return (region);
}

static Region Number(const Frame& f, u32 from)
{
  u32 end = from;
  while (end < f.m_Buffer.m_Length && strchr(NUMBER, f.m_Buffer.m_Data[end].m_Codepoint))
  {
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Number
  };
  return (region);
}

static Region String(const Frame& f, u32 from, bool escape, bool newline)
{
  u32 quote = f.m_Buffer.m_Data[from].m_Codepoint;
  u32 end   = from + 1;
  while (end < f.m_Buffer.m_Length)
  {
    if (!newline && f.m_Buffer.m_Data[end].m_Codepoint == '\n')
    {
      break;
    }
    
    if (f.m_Buffer.m_Data[end].m_Codepoint == quote)
    {
      ++end;
      break;
    }
    
    end += (escape && f.m_Buffer.m_Data[end].m_Codepoint == '\\') + 1;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_String
  };
  return (region);
}

static Region Special(const Frame& f, u32 from, const char* special)
{
  u32 end = from;
  while (end < f.m_Buffer.m_Length && strchr(special, f.m_Buffer.m_Data[end].m_Codepoint))
  {
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Special
  };
  return (region);
}

static bool TryKeyword(OUT Region& region, const Frame& f, u32 from, u32 end, LangMode lang)
{
  for (usize i = 0; i < g_Options.m_Lang[lang].m_NKeywords; ++i)
  {
    const EChar*  keyword = g_Options.m_Lang[lang].m_Keywords[i].m_Data;
    u32           length  = g_Options.m_Lang[lang].m_Keywords[i].m_Length;
    
    if (end - from != length)
    {
      continue;
    }
    
    if (!memcmp(keyword, &f.m_Buffer.m_Data[from], sizeof(EChar) * length))
    {
      region.m_Color = g_Options.m_Keyword;
      return (true);
    }
  }
  
  for (usize i = 0; i < g_Options.m_Lang[lang].m_NPrimitives; ++i)
  {
    const EChar*  primitive = g_Options.m_Lang[lang].m_Primitives[i].m_Data;
    u32           length    = g_Options.m_Lang[lang].m_Primitives[i].m_Length;
    
    if (end - from != length)
    {
      continue;
    }
    
    if (!memcmp(primitive, &f.m_Buffer.m_Data[from], sizeof(EChar) * length))
    {
      region.m_Color = g_Options.m_Primitive;
      return (true);
    }
  }
  
  return (false);
}

static Region CPreproc(const Frame& f, u32 from)
{
  u32   end       = from;
  bool  continue_ = false;
  while (end < f.m_Buffer.m_Length && (continue_ || f.m_Buffer.m_Data[end].m_Codepoint != '\n'))
  {
    if (!f.m_Buffer.m_Data[end].IsSpace())
    {
      continue_ = f.m_Buffer.m_Data[end].m_Codepoint == '\\';
    }
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Macro
  };
  return (region);
}

static Region CComment(const Frame& f, u32 from)
{
  u32 end = from + 2;
  while (end < f.m_Buffer.m_Length && !CompareString(f, "*/", end))
  {
    ++end;
  }
  end += 2 * (end < f.m_Buffer.m_Length);
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Comment
  };
  return (region);
}

static Region CWord(const Frame& f, u32 from)
{
  i32 nLower  = 0;
  u32 end     = from;
  while (end < f.m_Buffer.m_Length && strchr(C_WORD, f.m_Buffer.m_Data[end].m_Codepoint))
  {
    nLower += islower(f.m_Buffer.m_Data[end].m_Codepoint);
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Normal
  };
  
  if (TryKeyword(region, f, from, end, LANG_MODE_C))
  {
    return (region);
  }
  
  if (end - from >= 2 && f.m_Buffer.m_Data[end - 1].m_Codepoint == 't' && f.m_Buffer.m_Data[end - 2].m_Codepoint == '_')
  {
    region.m_Color = g_Options.m_Type;
    return (region);
  }
  
  if (nLower == 0)
  {
    region.m_Color = g_Options.m_Macro;
    return (region);
  }
  
  u32 next  = end;
  while (next < f.m_Buffer.m_Length && f.m_Buffer.m_Data[next].IsSpace())
  {
    ++next;
  }
  
  if (next < f.m_Buffer.m_Length && f.m_Buffer.m_Data[next].m_Codepoint == '(')
  {
    region.m_Color = g_Options.m_Emphasis;
    return (region);
  }
  
  if (isupper(f.m_Buffer.m_Data[from].m_Codepoint))
  {
    region.m_Color = g_Options.m_Type;
    return (region);
  }
  
  return (region);
}

static Region ShWord(const Frame& f, u32 from)
{
  i32 nLower  = 0;
  u32 end     = from;
  while (end < f.m_Buffer.m_Length && strchr(SH_WORD, f.m_Buffer.m_Data[end].m_Codepoint))
  {
    nLower += islower(f.m_Buffer.m_Data[end].m_Codepoint);
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Normal
  };
  
  if (TryKeyword(region, f, from, end, LANG_MODE_SH))
  {
    return (region);
  }
  
  if (nLower == 0)
  {
    region.m_Color = g_Options.m_Emphasis;
    return (region);
  }
  
  return (region);
}

static Region JSWord(const Frame& f, u32 from)
{
  u32 end = from;
  while (end < f.m_Buffer.m_Length && strchr(JS_WORD, f.m_Buffer.m_Data[end].m_Codepoint))
  {
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Normal
  };
  
  if (TryKeyword(region, f, from, end, LANG_MODE_JS))
  {
    return (region);
  }
  
  if (isupper(f.m_Buffer.m_Data[from].m_Codepoint))
  {
    region.m_Color = g_Options.m_Emphasis;
    return (region);
  }
  
  return (region);
}

static Region CCWord(const Frame& f, u32 from)
{
  i32 nLower  = 0;
  u32 end     = from;
  while (end < f.m_Buffer.m_Length && strchr(CC_WORD, f.m_Buffer.m_Data[end].m_Codepoint))
  {
    nLower += islower(f.m_Buffer.m_Data[end].m_Codepoint);
    ++end;
  }
  
  Region  region  =
  {
    .m_LowerBound = from,
    .m_UpperBound = end,
    .m_Color      = g_Options.m_Normal
  };
  
  if (TryKeyword(region, f, from, end, LANG_MODE_CC))
  {
    return (region);
  }
  
  if (end - from >= 2 && f.m_Buffer.m_Data[end - 1].m_Codepoint == 't' && f.m_Buffer.m_Data[end - 2].m_Codepoint == '_')
  {
    region.m_Color = g_Options.m_Type;
    return (region);
  }
  
  if (nLower == 0)
  {
    region.m_Color = g_Options.m_Macro;
    return (region);
  }
  
  u32 next  = end;
  while (next < f.m_Buffer.m_Length && f.m_Buffer.m_Data[next].IsSpace())
  {
    ++next;
  }
  
  // very dumb way of diong checks for template parameters; only works for "well behaved" code, but honestly good enough
  if (next < f.m_Buffer.m_Length && f.m_Buffer.m_Data[next].m_Codepoint == '<')
  {
    ++next;
    for (u32 nOpen = 1; next < f.m_Buffer.m_Length && nOpen > 0; ++next)
    {
      nOpen += f.m_Buffer.m_Data[next].m_Codepoint == '<';
      nOpen -= f.m_Buffer.m_Data[next].m_Codepoint == '>';
    }
  }
  
  while (next < f.m_Buffer.m_Length && f.m_Buffer.m_Data[next].IsSpace())
  {
    ++next;
  }
  
  if (next < f.m_Buffer.m_Length && f.m_Buffer.m_Data[next].m_Codepoint == '(')
  {
    region.m_Color = g_Options.m_Emphasis;
    return (region);
  }
  
  if (isupper(f.m_Buffer.m_Data[from].m_Codepoint))
  {
    region.m_Color = g_Options.m_Type;
    return (region);
  }
  
  return (region);
}
