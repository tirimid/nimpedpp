// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdio>
#include <Util.hh>

constexpr u32 REPLACEMENT_CHAR  = 0xfffd;

struct EChar
{
  u32   m_Codepoint   {};
  char  m_Encoding[4] {};
  
  bool  IsPrint() const;
  bool  IsSpace() const;
  bool  IsAlpha() const;
  bool  IsDigit() const;
  bool  IsAlnum() const;
  usize EncodingLength() const;
  
  constexpr EChar()
  {
  }
  
  constexpr EChar(u32 codepoint)
    : m_Codepoint(codepoint)
  {
    if (codepoint < 0x80)
    {
      m_Encoding[0] = codepoint;
    }
    else if (codepoint < 0x800)
    {
      m_Encoding[0] = 0xc0 | (codepoint & 0x7c0) >> 6;
      m_Encoding[1] = 0x80 | (codepoint & 0x3f);
    }
    else if (codepoint < 0x10000)
    {
      m_Encoding[0] = 0xe0 | (codepoint & 0xf000) >> 12;
      m_Encoding[1] = 0x80 | (codepoint & 0xfc0) >> 6;
      m_Encoding[2] = 0x80 | (codepoint & 0x3f);
    }
    else if (codepoint < 0x11000)
    {
      m_Encoding[0] = 0xf0 | (codepoint & 0x1c0000) >> 18;
      m_Encoding[1] = 0x80 | (codepoint & 0x3f000) >> 12;
      m_Encoding[2] = 0x80 | (codepoint & 0xfc0) >> 6;
      m_Encoding[3] = 0x80 | (codepoint & 0x3f);
    }
    else
    {
      *this = EChar{REPLACEMENT_CHAR};
    }
  }
  
  constexpr EChar(const u8* ptr)
  {
    m_Encoding[0] = ptr[0];
    if (ptr[0] < 0x80)
    {
      m_Codepoint = ptr[0];
      return;
    }
    
    i32 nBytes  = 1;
    for (i32 i = 6; i >= 0; --i)
    {
      if (!(ptr[0] & 1 << i))
      {
        break;
      }
      ++nBytes;
    }
    
    switch (nBytes)
    {
    case (2):
      m_Encoding[1] = ptr[1];
      m_Codepoint |= (ptr[0] & 0x1f) << 6;
      m_Codepoint |= ptr[1] & 0x3f;
      break;
    case (3):
      m_Encoding[1] = ptr[1];
      m_Encoding[2] = ptr[2];
      m_Codepoint |= (ptr[0] & 0xf) << 12;
      m_Codepoint |= (ptr[1] & 0x3f) << 6;
      m_Codepoint |= ptr[2] & 0x3f;
      break;
    case (4):
      m_Encoding[1] = ptr[1];
      m_Encoding[2] = ptr[2];
      m_Encoding[3] = ptr[3];
      m_Codepoint |= (ptr[0] & 0x7) << 18;
      m_Codepoint |= (ptr[1] & 0x3f) << 12;
      m_Codepoint |= (ptr[2] & 0x3f) << 6;
      m_Codepoint |= ptr[3] & 0x3f;
      break;
    default:
      *this = EChar{REPLACEMENT_CHAR};
      break;
    }
  }
};

struct EString
{
  EChar*  m_Data      {};
  u32     m_Length    {};
  u32     m_Capacity  {};
  
  char*   ToCString() const;
  EString Copy() const;
  EChar*  CopyData() const;
  void    Free();
  void    IncreaseAllocation();
  void    Insert(EChar ch, u32 pos);
  void    Insert(const EString& str, u32 pos);
  void    Insert(const EChar* str, u32 length, u32 pos);
  void    Insert(const char* str, u32 pos);
  void    Erase(u32 lb, u32 ub);
  void    Erase(u32 pos);
  EString Substring(u32 lb, u32 ub) const;
  
  EString();
  EString(const char* cString);
};

EChar ReadEChar();
EChar ReadEChar(FILE* file);
i32   PrintEChar(EChar ch);
i32   PrintEChar(FILE* file, EChar ch);
