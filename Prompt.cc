// SPDX-License-Identifier: GPL-3.0-or-later

#include <cctype>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <Prompt.hh>
#include <Render.hh>

extern "C"
{
#include <dirent.h>
}

Prompt  g_Prompt =
{
  .m_Cursor = -1
};

void  BeginPrompt(OWNS EString str)
{
  g_Prompt.m_Data.Free();
  g_Prompt.m_Data   = str;
  g_Prompt.m_Start  = str.m_Length;
  g_Prompt.m_Cursor = str.m_Length;
  g_Prompt.m_Status = PROMPT_NONE;
}

void  BeginPrompt(const char* str)
{
  BeginPrompt(EString{str});
}

void  EndPrompt()
{
  g_Prompt.m_Cursor = -1;
}

void  PromptWrite(EChar ch, u32 pos)
{
  g_Prompt.m_Data.Insert(ch, pos);
}

void  PromptWrite(const EString& str, u32 pos)
{
  g_Prompt.m_Data.Insert(str, pos);
}

void  PromptWrite(const char* str, u32 pos)
{
  g_Prompt.m_Data.Insert(str, pos);
}

void  PromptErase(u32 lowerBound, u32 upperBound)
{
  g_Prompt.m_Data.Erase(lowerBound, upperBound);
}

void  PromptErase(u32 pos)
{
  g_Prompt.m_Data.Erase(pos);
}

void  RenderPrompt()
{
  RenderBar(g_Prompt.m_Data.Copy());
}

void  CompletePromptPath()
{
  if ((u32)g_Prompt.m_Cursor < g_Prompt.m_Data.m_Length)
  {
    return;
  }
  
  char* promptData          = PromptDataCString();
  char  path[PATH_MAX + 1]  {};
  TruncateCString(promptData, PATH_MAX);
  strcpy(path, promptData);
  free(promptData);
  
  char  name[PATH_MAX + 1]  {};
  char  dir[PATH_MAX + 1]   {};
  
  strcpy(dir, path);
  char* firstSlash  = strchr(dir, '/');
  char* lastSlash   = strrchr(dir, '/');
  
  char* firstChar = dir;
  while (*firstChar && isspace((u8)*firstChar))
  {
    ++firstChar;
  }
  
  if (!lastSlash)
  {
    dir[0] = '.';
    dir[1] = 0;
    strcpy(name, path);
  }
  else if (firstChar && firstChar == firstSlash && firstSlash == lastSlash)
  {
    strcpy(name, &lastSlash[1]);
    lastSlash[1] = 0;
  }
  else
  {
    strcpy(name, &lastSlash[1]);
    lastSlash[0] = 0;
  }
  
  DIR*  dirPtr  = opendir(dir);
  if (!dirPtr)
  {
    return;
  }
  
  usize nameLength  = strlen(name);
  for (struct dirent* dirEnt = readdir(dirPtr); dirEnt; dirEnt = readdir(dirPtr))
  {
    if (strncasecmp(name, dirEnt->d_name, nameLength)
      || (dirEnt->d_type != DT_DIR && dirEnt->d_type != DT_REG)
      || !strcmp(".", dirEnt->d_name)
      || !strcmp("..", dirEnt->d_name))
    {
      continue;
    }
    
    char  newPath[PATH_MAX + 1] {};
    strcpy(newPath, dir);
    
    // only add '/' if not root
    if (firstChar && firstChar != lastSlash)
    {
      strcat(newPath, "/");
    }
    
    strcat(newPath, dirEnt->d_name);
    if (dirEnt->d_type == DT_DIR)
    {
      strcat(newPath, "/");
    }
    
    g_Prompt.m_Data.Erase(g_Prompt.m_Start, g_Prompt.m_Data.m_Length);
    g_Prompt.m_Data.Insert(newPath, g_Prompt.m_Start);
    g_Prompt.m_Cursor = g_Prompt.m_Data.m_Length;
    
    closedir(dirPtr);
    return;
  }
  
  closedir(dirPtr);
}

bool  WritableToPrompt(EChar ch)
{
  bool  typeable  = ch.IsPrint();
  bool  ignored   = ch.m_Codepoint == REPLACEMENT_CHAR;
  return (typeable && !ignored);
}

EString PromptData()
{
  EString data  = g_Prompt.m_Data.Copy();
  data.Erase(0, g_Prompt.m_Start);
  return (data);
}

char* PromptDataCString()
{
  EString dataEString = PromptData();
  char*   dataCString = dataEString.ToCString();
  dataEString.Free();
  return (dataCString);
}
