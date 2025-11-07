// SPDX-License-Identifier: GPL-3.0-or-later

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <Encoding.hh>

bool  EChar::IsPrint() const
{
  if (m_Codepoint <= 0x1f
    || (m_Codepoint >= 0x7f && m_Codepoint <= 0x9f)
    || m_Codepoint == 0x61c
    || (m_Codepoint >= 0x200e && m_Codepoint <= 0x200f)
    || (m_Codepoint >= 0x2028 && m_Codepoint <= 0x2029)
    || (m_Codepoint >= 0x202a && m_Codepoint <= 0x202e)
    || (m_Codepoint >= 0x2066 && m_Codepoint <= 0x2069)
    || (m_Codepoint >= 0xfff9 && m_Codepoint <= 0xfffb)
    || (m_Codepoint >= 0xe0000 && m_Codepoint <= 0xe007f))
  {
    return (false);
  }
  else
  {
    return (true);
  }
}

bool  EChar::IsSpace() const
{
  if ((m_Codepoint >= 0x9 && m_Codepoint <= 0xd)
    || m_Codepoint == 0x20
    || m_Codepoint == 0x85
    || m_Codepoint == 0xa0
    || m_Codepoint == 0x1680
    || (m_Codepoint >= 0x2000 && m_Codepoint <= 0x200a)
    || (m_Codepoint >= 0x2028 && m_Codepoint <= 0x2029)
    || m_Codepoint == 0x202f
    || m_Codepoint == 0x205f
    || m_Codepoint == 0x3000)
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

bool  EChar::IsAlpha() const
{
  // some writing systems are not included
  if ((m_Codepoint >= 0x41 && m_Codepoint <= 0x5a)
    || (m_Codepoint >= 0x61 && m_Codepoint <= 0x7a)
    || (m_Codepoint >= 0xc0 && m_Codepoint <= 0xd6)
    || (m_Codepoint >= 0xd8 && m_Codepoint <= 0xf6)
    || (m_Codepoint >= 0xf8 && m_Codepoint <= 0x2af)
    || (m_Codepoint >= 0x370 && m_Codepoint <= 0x4ff)
    || (m_Codepoint >= 0x530 && m_Codepoint <= 0x74f)
    || (m_Codepoint >= 0x780 && m_Codepoint <= 0x7bf)
    || (m_Codepoint >= 0x900 && m_Codepoint <= 0xdff)
    || (m_Codepoint >= 0x10a0 && m_Codepoint <= 0x137f)
    || (m_Codepoint >= 0x1400 && m_Codepoint <= 0x167f)
    || (m_Codepoint >= 0x1800 && m_Codepoint <= 0x18af)
    || (m_Codepoint >= 0x1e00 && m_Codepoint <= 0x1fff)
    || (m_Codepoint >= 0x3040 && m_Codepoint <= 0x33ff)
    || (m_Codepoint >= 0xfb00 && m_Codepoint <= 0xfb4f)
    || (m_Codepoint >= 0xfe30 && m_Codepoint <= 0xfe4f)
    || (m_Codepoint >= 0x10450 && m_Codepoint <= 0x1047f))
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

bool  EChar::IsDigit() const
{
  if ((m_Codepoint >= 0x30 && m_Codepoint <= 0x39)
    || (m_Codepoint >= 0x2160 && m_Codepoint <= 0x2188)
    || m_Codepoint == 0x3007
    || (m_Codepoint >= 0x3021 && m_Codepoint <= 0x3029)
    || m_Codepoint == 0x5341
    || (m_Codepoint >= 0x5344 && m_Codepoint <= 0x5345)
    || (m_Codepoint >= 0x10140 && m_Codepoint <= 0x1018f)
    || (m_Codepoint >= 0x1d360 && m_Codepoint <= 0x1d37f))
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

bool  EChar::IsAlnum() const
{
  return (IsAlpha() || IsDigit());
}

usize EChar::EncodingLength() const
{
  usize length  = !!m_Encoding[0] + !!m_Encoding[1] + !!m_Encoding[2] + !!m_Encoding[3];
  return (length);
}

char* EString::ToCString() const
{
  // first allocate too much memory, then reallocate to the appropriate amount; calloc() used so allocated memory is zeroed
  char* cString = (char*)calloc(m_Length + 1, sizeof(EChar));
  
  usize cStringLength = 0;
  for (usize i = 0; i < m_Length; ++i)
  {
    for (usize j = 0; j < 4; ++j)
    {
      if (!m_Data[i].m_Encoding[j])
      {
        break;
      }
      
      cString[cStringLength++] = m_Data[i].m_Encoding[j];
    }
  }
  
  cString = (char*)realloc(cString, cStringLength + 1);
  return (cString);
}

EString EString::Copy() const
{
  if (!m_Data)
  {
    return (EString{});
  }
  
  EString newEString  {};
  
  newEString.m_Data     = (EChar*)calloc(m_Capacity, sizeof(EChar));
  newEString.m_Length   = m_Length;
  newEString.m_Capacity = m_Capacity;
  memcpy(newEString.m_Data, m_Data, m_Capacity * sizeof(EChar));
  
  return (newEString);
}

void  EString::Free()
{
  if (m_Data)
  {
    free(m_Data);
  }
  
  m_Data      = nullptr;
  m_Length    = 0;
  m_Capacity  = 0;
}

void  EString::IncreaseAllocation()
{
  m_Capacity = m_Data ? 2 * m_Capacity : 1;
  m_Data = (EChar*)reallocarray(m_Data, m_Capacity, sizeof(EChar));
}

void  EString::Append(EChar ch)
{
  if (m_Length >= m_Capacity)
  {
    IncreaseAllocation();
  }
  m_Data[m_Length++] = ch;
}

void  EString::Insert(EChar ch, u32 pos)
{
  if (m_Length >= m_Capacity)
  {
    m_Capacity *= 2;
    m_Data = (EChar*)reallocarray(m_Data, m_Capacity, sizeof(EChar));
  }
  
  memmove(&m_Data[pos + 1], &m_Data[pos], sizeof(EChar) * (m_Length - pos));
  m_Data[pos] = ch;
  ++m_Length;
}

void  EString::Insert(const EString& str, u32 pos)
{
  u32 newCapacity = m_Capacity;
  for (usize i = 1; i <= str.m_Length; ++i)
  {
    if (m_Length + i > newCapacity)
    {
      newCapacity *= 2;
    }
  }
  
  if (newCapacity != m_Capacity)
  {
    m_Capacity = newCapacity;
    m_Data = (EChar*)reallocarray(m_Data, m_Capacity, sizeof(EChar));
  }
  
  memmove(&m_Data[pos + str.m_Length], &m_Data[pos], sizeof(EChar) * (m_Length - pos));
  memcpy(&m_Data[pos], str.m_Data, sizeof(EChar) * str.m_Length);
  m_Length += str.m_Length;
}

void  EString::Insert(const EChar* str, u32 length, u32 pos)
{
  u32 newCapacity = m_Capacity;
  for (usize i = 1; i <= length; ++i)
  {
    if (m_Length + i > newCapacity)
    {
      newCapacity *= 2;
    }
  }
  
  if (newCapacity != m_Capacity)
  {
    m_Capacity = newCapacity;
    m_Data = (EChar*)reallocarray(m_Data, m_Capacity, sizeof(EChar));
  }
  
  memmove(&m_Data[pos + length], &m_Data[pos], sizeof(EChar) * (m_Length - pos));
  memcpy(&m_Data[pos], str, sizeof(EChar) * length);
  m_Length += length;
}

void  EString::Insert(const char* str, u32 pos)
{
  u32 length      = strlen(str);
  u32 newCapacity = m_Capacity;
  for (usize i = 1; i <= length; ++i)
  {
    if (m_Length + i > newCapacity)
    {
      newCapacity *= 2;
    }
  }
  
  if (newCapacity != m_Capacity)
  {
    m_Capacity = newCapacity;
    m_Data = (EChar*)reallocarray(m_Data, m_Capacity, sizeof(EChar));
  }
  
  memmove(&m_Data[pos + length], &m_Data[pos], sizeof(EChar) * (m_Length - pos));
  for (usize i = 0; i < length; ++i)
  {
    m_Data[pos + i] = str[i];
  }
  m_Length += length;
}

void  EString::Erase(u32 lb, u32 ub)
{
  memmove(&m_Data[lb], &m_Data[ub], sizeof(EChar) * (m_Length - ub));
  m_Length -= ub - lb;
}

EString::EString()
{
}

EString::EString(const char* cString)
{
  usize cStringLength = strlen(cString);
  
  m_Capacity  = cStringLength;
  m_Data      = (EChar*)calloc(cStringLength, sizeof(EChar));
  
  for (usize i = 0; i < cStringLength;)
  {
    EChar ch  {(const u8*)&cString[i]};
    m_Data[m_Length++] = ch;
    i += ch.m_Codepoint != REPLACEMENT_CHAR ? ch.EncodingLength() : 1;
  }
  
  // resize allocated memory to minimum requirement
  m_Data = (EChar*)reallocarray(m_Data, m_Length, sizeof(EChar));
}

EChar ReadEChar()
{
  return (ReadEChar(stdin));
}

EChar ReadEChar(FILE* file)
{
  i32 firstByte = fgetc(file);
  if (firstByte == EOF)
  {
    return (REPLACEMENT_CHAR);
  }
  
  if (firstByte < 0x80)
  {
    EChar ch  {(u32)firstByte};
    return (ch);
  }
  
  i32 nBytes = 1;
  for (i32 i = 6; i>= 0; --i)
  {
    if (!(firstByte & 1 << i))
    {
      break;
    }
    ++nBytes;
  }
  
  u32 codepoint = 0;
  switch (nBytes)
  {
  case (2):
  {
    i32 secondByte  = fgetc(file);
    if (secondByte == EOF)
    {
      return (REPLACEMENT_CHAR);
    }
    
    codepoint |= (firstByte & 0x1f) << 6;
    codepoint |= secondByte & 0x3f;
    
    break;
  }
  case (3):
  {
    i32 secondByte  = fgetc(file);
    i32 thirdByte   = fgetc(file);
    if (secondByte == EOF || thirdByte == EOF)
    {
      return (REPLACEMENT_CHAR);
    }
    
    codepoint |= (firstByte & 0xf) << 12;
    codepoint |= (secondByte & 0x3f) << 6;
    codepoint |= thirdByte & 0x3f;
    
    break;
  }
  case (4):
  {
    i32 secondByte  = fgetc(file);
    i32 thirdByte   = fgetc(file);
    i32 fourthByte  = fgetc(file);
    if (secondByte == EOF || thirdByte == EOF || fourthByte == EOF)
    {
      return (REPLACEMENT_CHAR);
    }
    
    codepoint |= (firstByte & 0x7) << 18;
    codepoint |= (secondByte & 0x3f) << 12;
    codepoint |= (thirdByte & 0x3f) << 6;
    codepoint |= fourthByte & 0x3f;
    
    break;
  }
  default:
    return (REPLACEMENT_CHAR);
  }
  
  return (codepoint);
}

i32 PrintEChar(EChar ch)
{
  return (PrintEChar(stdout, ch));
}

i32 PrintEChar(FILE* file, EChar ch)
{
  for (usize i = 0; i < 4 && ch.m_Encoding[i]; ++i)
  {
    if (fputc(ch.m_Encoding[i], file) == EOF)
    {
      return (1);
    }
  }
  return (0);
}
