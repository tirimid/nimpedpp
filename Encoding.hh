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
  
  EChar();
  EChar(u32 codepoint);
  EChar(const u8* ptr);
};

struct EString
{
  EChar*  m_Data      {};
  u32     m_Length    {};
  u32     m_Capacity  {};
  
  char* ToCString() const;
  void  Free();
  void  IncreaseAllocation();
  void  Append(EChar ch);
  
  EString();
  EString(const char* cString);
};

EChar ReadEChar();
EChar ReadEChar(FILE* file);
i32   PrintEChar(EChar ch);
i32   PrintEChar(FILE* file, EChar ch);
