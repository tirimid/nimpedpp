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
};

static WordList completions[FUNCTIONAL::MAX_FILES];

void  GatherCompletions()
{
  for (usize i = 0; i < FUNCTIONAL::MAX_FILES; ++i)
  {
    if (g_Editor.m_Frames[i].m_Flags & FRAME_GATHER)
    {
      for (u32 j = 0; j < completions[i].m_Length; ++j)
      {
        completions[i].m_Words[j].Free();
      }
      completions[i].m_Length = 0;
    }
  }
  
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    Frame&  f = g_Editor.m_Frames[i];
    if (!(f.m_Flags & FRAME_GATHER))
    {
      continue;
    }
    
    WordList& words = completions[i];
    
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
      for (u32 j = 0; j < words.m_Length; ++j)
      {
        if (word.Equals(words.m_Words[j]))
        {
          word.Free();
          goto nextWord;
        }
      }
      
      if (words.m_Length >= words.m_Capacity)
      {
        words.m_Capacity = words.m_Words ? 2 * words.m_Capacity : 1;
        words.m_Words = (EString*)reallocarray(words.m_Words, words.m_Capacity, sizeof(EString));
      }
      words.m_Words[words.m_Length] = word;
      ++words.m_Length;
      
    nextWord:
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
