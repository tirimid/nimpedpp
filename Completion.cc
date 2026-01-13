// SPDX-License-Identifier: GPL-3.0-or-later

#include <Completion.hh>
#include <cstdlib>
#include <Editor.hh>
#include <Options.hh>
#include <Render.hh>
#include <Util.hh>

struct WordList
{
  EString*  m_Words;
  u32       m_Length;
  u32       m_Capacity;
  
  void      IncreaseAllocation();
  void      Reset();
};

static WordList completions[FUNCTIONAL::MAX_FILES];

void  GatherCompletions()
{
  for (usize i = 0; i < FUNCTIONAL::MAX_FILES; ++i)
  {
    if (g_Editor.m_Frames[i].m_Flags & FRAME_GATHER)
    {
      completions[i].Reset();
    }
  }
  
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    Frame&  f = g_Editor.m_Frames[i];
    if (!(f.m_Flags & FRAME_GATHER))
    {
      continue;
    }
    
    // extract all the words in the given frame
    usize start = 0;
    usize end   = 0;
    
    for (;;)
    {
      while (start < f.m_Buffer.m_Length && !f.m_Buffer.m_Data[start].IsWord())
      {
        ++start;
      }
      
      if (start >= f.m_Buffer.m_Length)
      {
        break;
      }
      
      end = start + 1;
      while (end < f.m_Buffer.m_Length && f.m_Buffer.m_Data[end].IsWord())
      {
        ++end;
      }
      
      EString word  = f.m_Buffer.Substring(start, end);
      if (completions[i].m_Length >= completions[i].m_Capacity)
      {
        completions[i].IncreaseAllocation();
      }
      completions[i].m_Words[completions[i].m_Length] = word;
      ++completions[i].m_Length;
      
      start = end + 1;
    }
    f.m_Flags &= ~FRAME_GATHER;
  }
}

const EString*  CompleteWord(const EString& word)
{
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    for (usize j = 0; j < completions[i].m_Length; ++j)
    {
      if (completions[i].m_Words[j].m_Length <= word.m_Length)
      {
        continue;
      }
      
      if (word.Equals(completions[i].m_Words[j], word.m_Length))
      {
        return (&completions[i].m_Words[j]);
      }
    }
  }
  
  return (nullptr);
}

void  RenderCompletion(usize frame, const EString& word)
{
  const Frame&  f = g_Editor.m_Frames[frame];
  
  u32 start = f.m_Cursor;
  while (start > 0 && f.m_Buffer.m_Data[start - 1].IsWord())
  {
    --start;
  }
  
  u32 x {};
  u32 y {};
  u32 w {};
  u32 h {};
  ArrangeFrame(frame, x, y, w, h);
  
  u32 startX  {};
  u32 startY  {};
  if (f.VisualPosition(startX, startY, start, w))
  {
    u32 startSuggestX = startX + f.m_Cursor - start;
    startSuggestX = startSuggestX > w ? w : startSuggestX;
    
    u32 endX  = startX + word.m_Length;
    endX = endX > w ? w : endX;
    
    RenderFill(g_Options.m_CompleteCurrent, x + startX, y + startY, startSuggestX - startX, 1);
    RenderFill(g_Options.m_CompleteSuggestion, x + startSuggestX, y + startY, endX - startSuggestX, 1);
    for (u32 i = 0; i < word.m_Length && startX + i < w; ++i)
    {
      RenderPut(word.m_Data[i], x + startX + i, y + startY);
    }
  }
}

void  WordList::IncreaseAllocation()
{
  m_Capacity = m_Words ? 2 * m_Capacity : 1;
  m_Words = (EString*)reallocarray(m_Words, m_Capacity, sizeof(EString));
}

void  WordList::Reset()
{
  for (u32 i = 0; i < m_Length; ++i)
  {
    m_Words[i].Free();
  }
  m_Length = 0;
}
