// SPDX-License-Identifier: GPL-3.0-or-later

#include <Binds.hh>
#include <cstdlib>
#include <Editor.hh>
#include <Input.hh>
#include <Options.hh>
#include <Prompt.hh>
#include <Render.hh>

namespace Binds
{

static void FrameMoveLeft();
static void FrameMoveRight();
static void FrameMoveUp();
static void FrameMoveDown();
static void FrameMoveStart();
static void FrameMoveEnd();
static void FrameMoveWordLeft();
static void FrameMoveWordRight();
static void PromptMoveLeft();
static void PromptMoveRight();
static void PromptMoveStart();
static void PromptMoveEnd();
static void PromptMoveWordLeft();
static void PromptMoveWordRight();
static void Quit();
static void Exit();
static void QuitPromptFail();
static void QuitPromptSuccess();
static void Next();
static void Previous();
static void WriteMode();
static void FrameDeleteFront();
static void FrameDeleteBack();
static void FrameDeleteWord();
static void PromptDeleteFront();
static void PromptDeleteBack();
static void PromptDeleteWord();
static void Newline();
static void Undo();
static void NewFrame();
static void KillFrame();
static void Save();
static void Focus();
static void OpenFile();
static void Search();
static void ReverseSearch();
static void FrameLeftParen();
static void FrameLeftBracket();
static void FrameLeftBrace();
static void FrameDoubleQuote();
static void PromptLeftParen();
static void PromptLeftBracket();
static void PromptLeftBrace();
static void PromptDoubleQuote();
static void Paste();
static void CopyLine();
static void CutLine();
static void CopyLines();
static void CutLines();
static void CopyUntilLine();
static void CutUntilLine();
static void Zoom();
static void Goto();
static void RecordMacro();
static void ExecuteMacro();
static void Help();
static void Tab();
static void Complete();

}

void  InstallBaseBinds()
{
  Unbind();
  Bind(KEYBIND::FRAME_MOVE_LEFT,        Binds::FrameMoveLeft);
  Bind(KEYBIND::FRAME_MOVE_RIGHT,       Binds::FrameMoveRight);
  Bind(KEYBIND::FRAME_MOVE_UP,          Binds::FrameMoveUp);
  Bind(KEYBIND::FRAME_MOVE_DOWN,        Binds::FrameMoveDown);
  Bind(KEYBIND::FRAME_MOVE_START,       Binds::FrameMoveStart);
  Bind(KEYBIND::FRAME_MOVE_END,         Binds::FrameMoveEnd);
  Bind(KEYBIND::FRAME_MOVE_WORD_LEFT,   Binds::FrameMoveWordLeft);
  Bind(KEYBIND::FRAME_MOVE_WORD_RIGHT,  Binds::FrameMoveWordRight);
  Bind(KEYBIND::QUIT,                   Binds::Quit);
  Bind(KEYBIND::NEXT,                   Binds::Next);
  Bind(KEYBIND::PREVIOUS,               Binds::Previous);
  Bind(KEYBIND::WRITE_MODE,             Binds::WriteMode);
  Bind(KEYBIND::UNDO,                   Binds::Undo);
  Bind(KEYBIND::NEW_FRAME,              Binds::NewFrame);
  Bind(KEYBIND::KILL_FRAME,             Binds::KillFrame);
  Bind(KEYBIND::SAVE,                   Binds::Save);
  Bind(KEYBIND::FOCUS,                  Binds::Focus);
  Bind(KEYBIND::OPEN_FILE,              Binds::OpenFile);
  Bind(KEYBIND::SEARCH,                 Binds::Search);
  Bind(KEYBIND::REVERSE_SEARCH,         Binds::ReverseSearch);
  Bind(KEYBIND::PASTE,                  Binds::Paste);
  Bind(KEYBIND::COPY_LINE,              Binds::CopyLine);
  Bind(KEYBIND::CUT_LINE,               Binds::CutLine);
  Bind(KEYBIND::COPY_LINES,             Binds::CopyLines);
  Bind(KEYBIND::CUT_LINES,              Binds::CutLines);
  Bind(KEYBIND::COPY_UNTIL_LINE,        Binds::CopyUntilLine);
  Bind(KEYBIND::CUT_UNTIL_LINE,         Binds::CutUntilLine);
  Bind(KEYBIND::ZOOM,                   Binds::Zoom);
  Bind(KEYBIND::GOTO,                   Binds::Goto);
  Bind(KEYBIND::RECORD_MACRO,           Binds::RecordMacro);
  Bind(KEYBIND::EXECUTE_MACRO,          Binds::ExecuteMacro);
  Bind(KEYBIND::HELP,                   Binds::Help);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = false;
  
  RenderBar(VISUAL::BASE_NAME);
}

void  InstallWriteBinds()
{
  Unbind();
  Bind(KEYBIND::EXIT,         Binds::Exit);
  Bind(KEYBIND::DELETE_FRONT, Binds::FrameDeleteFront);
  Bind(KEYBIND::DELETE_BACK,  Binds::FrameDeleteBack);
  Bind(KEYBIND::DELETE_WORD,  Binds::FrameDeleteWord);
  Bind(KEYBIND::NEWLINE,      Binds::Newline);
  Bind(KEYBIND::LEFT_PAREN,   Binds::FrameLeftParen);
  Bind(KEYBIND::LEFT_BRACKET, Binds::FrameLeftBracket);
  Bind(KEYBIND::LEFT_BRACE,   Binds::FrameLeftBrace);
  Bind(KEYBIND::DOUBLE_QUOTE, Binds::FrameDoubleQuote);
  Bind(KEYBIND::TAB,          Binds::Tab);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = true;
  
  RenderBar(VISUAL::WRITE_NAME);
}

void  InstallPromptBinds()
{
  Unbind();
  Bind(KEYBIND::EXIT,                   Binds::QuitPromptFail);
  Bind(KEYBIND::NEWLINE,                Binds::QuitPromptSuccess);
  Bind(KEYBIND::PROMPT_MOVE_LEFT,       Binds::PromptMoveLeft);
  Bind(KEYBIND::PROMPT_MOVE_RIGHT,      Binds::PromptMoveRight);
  Bind(KEYBIND::PROMPT_MOVE_START,      Binds::PromptMoveStart);
  Bind(KEYBIND::PROMPT_MOVE_END,        Binds::PromptMoveEnd);
  Bind(KEYBIND::PROMPT_MOVE_WORD_LEFT,  Binds::PromptMoveWordLeft);
  Bind(KEYBIND::PROMPT_MOVE_WORD_RIGHT, Binds::PromptMoveWordRight);
  Bind(KEYBIND::DELETE_FRONT,           Binds::PromptDeleteFront);
  Bind(KEYBIND::DELETE_BACK,            Binds::PromptDeleteBack);
  Bind(KEYBIND::DELETE_WORD,            Binds::PromptDeleteWord);
  Bind(KEYBIND::LEFT_PAREN,             Binds::PromptLeftParen);
  Bind(KEYBIND::LEFT_BRACKET,           Binds::PromptLeftBracket);
  Bind(KEYBIND::LEFT_BRACE,             Binds::PromptLeftBrace);
  Bind(KEYBIND::DOUBLE_QUOTE,           Binds::PromptDoubleQuote);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = false;
}

void  InstallPathPromptBinds()
{
  Unbind();
  Bind(KEYBIND::EXIT,                   Binds::QuitPromptFail);
  Bind(KEYBIND::NEWLINE,                Binds::QuitPromptSuccess);
  Bind(KEYBIND::PROMPT_MOVE_LEFT,       Binds::PromptMoveLeft);
  Bind(KEYBIND::PROMPT_MOVE_RIGHT,      Binds::PromptMoveRight);
  Bind(KEYBIND::PROMPT_MOVE_START,      Binds::PromptMoveStart);
  Bind(KEYBIND::PROMPT_MOVE_END,        Binds::PromptMoveEnd);
  Bind(KEYBIND::PROMPT_MOVE_WORD_LEFT,  Binds::PromptMoveWordLeft);
  Bind(KEYBIND::PROMPT_MOVE_WORD_RIGHT, Binds::PromptMoveWordRight);
  Bind(KEYBIND::DELETE_FRONT,           Binds::PromptDeleteFront);
  Bind(KEYBIND::DELETE_BACK,            Binds::PromptDeleteBack);
  Bind(KEYBIND::DELETE_WORD,            Binds::PromptDeleteWord);
  Bind(KEYBIND::COMPLETE,               Binds::Complete);
  Bind(KEYBIND::LEFT_PAREN,             Binds::PromptLeftParen);
  Bind(KEYBIND::LEFT_BRACKET,           Binds::PromptLeftBracket);
  Bind(KEYBIND::LEFT_BRACE,             Binds::PromptLeftBrace);
  Bind(KEYBIND::DOUBLE_QUOTE,           Binds::PromptDoubleQuote);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = false;
}

void  InstallConfirmPromptBinds()
{
  Unbind();
  Bind(KEYBIND::PROMPT_YES, Binds::QuitPromptSuccess);
  Bind(KEYBIND::PROMPT_NO,  Binds::QuitPromptFail);
  Bind(KEYBIND::EXIT,       Binds::QuitPromptFail);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = false;
}

void  InstallNumberPromptBinds()
{
  Unbind();
  Bind(KEYBIND::EXIT,                   Binds::QuitPromptFail);
  Bind(KEYBIND::NEWLINE,                Binds::QuitPromptSuccess);
  Bind(KEYBIND::PROMPT_MOVE_LEFT,       Binds::PromptMoveLeft);
  Bind(KEYBIND::PROMPT_MOVE_RIGHT,      Binds::PromptMoveRight);
  Bind(KEYBIND::PROMPT_MOVE_START,      Binds::PromptMoveStart);
  Bind(KEYBIND::PROMPT_MOVE_END,        Binds::PromptMoveEnd);
  Bind(KEYBIND::PROMPT_MOVE_WORD_LEFT,  Binds::PromptMoveWordLeft);
  Bind(KEYBIND::PROMPT_MOVE_WORD_RIGHT, Binds::PromptMoveWordRight);
  Bind(KEYBIND::DELETE_FRONT,           Binds::PromptDeleteFront);
  Bind(KEYBIND::DELETE_BACK,            Binds::PromptDeleteBack);
  Bind(KEYBIND::DELETE_WORD,            Binds::PromptDeleteWord);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = false;
}

namespace Binds
{

static void FrameMoveLeft()
{
  Frame&  f = CurrentFrame();
  if (f.m_Cursor > 0)
  {
    --f.m_Cursor;
    f.SaveCursor();
  }
}

static void FrameMoveRight()
{
  Frame&  f = CurrentFrame();
  if (f.m_Cursor < f.m_Buffer.m_Length)
  {
    ++f.m_Cursor;
    f.SaveCursor();
  }
}

static void FrameMoveUp()
{
  Frame&  f = CurrentFrame();
  f.m_Cursor -= f.m_Cursor > 0;
  while (f.m_Cursor > 0 && f.m_Buffer.m_Data[f.m_Cursor].m_Codepoint != '\n')
  {
    --f.m_Cursor;
  }
  f.LoadCursor();
}

static void FrameMoveDown()
{
  Frame&  f = CurrentFrame();
  while (f.m_Cursor < f.m_Buffer.m_Length && f.m_Buffer.m_Data[f.m_Cursor].m_Codepoint != '\n')
  {
    ++f.m_Cursor;
  }
  f.m_Cursor += f.m_Cursor < f.m_Buffer.m_Length;
  f.LoadCursor();
}

static void FrameMoveStart()
{
  Frame&  f = CurrentFrame();
  while (f.m_Cursor > 0 && f.m_Buffer.m_Data[f.m_Cursor - 1].m_Codepoint != '\n')
  {
    --f.m_Cursor;
  }
  f.SaveCursor();
}

static void FrameMoveEnd()
{
  Frame&  f = CurrentFrame();
  while (f.m_Cursor < f.m_Buffer.m_Length && f.m_Buffer.m_Data[f.m_Cursor].m_Codepoint != '\n')
  {
    ++f.m_Cursor;
  }
  f.SaveCursor();
}

static void FrameMoveWordLeft()
{
  Frame&  f = CurrentFrame();
  while (f.m_Cursor && !f.m_Buffer.m_Data[f.m_Cursor - 1].IsAlnum())
  {
    --f.m_Cursor;
  }
  while (f.m_Cursor && f.m_Buffer.m_Data[f.m_Cursor - 1].IsAlnum())
  {
    --f.m_Cursor;
  }
  f.SaveCursor();
}

static void FrameMoveWordRight()
{
  Frame&  f = CurrentFrame();
  while (f.m_Cursor < f.m_Buffer.m_Length && !f.m_Buffer.m_Data[f.m_Cursor].IsAlnum())
  {
    ++f.m_Cursor;
  }
  while (f.m_Cursor < f.m_Buffer.m_Length && f.m_Buffer.m_Data[f.m_Cursor].IsAlnum())
  {
    ++f.m_Cursor;
  }
  f.SaveCursor();
}

static void PromptMoveLeft()
{
  g_Prompt.m_Cursor -= (u32)g_Prompt.m_Cursor > g_Prompt.m_Start;
}

static void PromptMoveRight()
{
  g_Prompt.m_Cursor += (u32)g_Prompt.m_Cursor < g_Prompt.m_Data.m_Length;
}

static void PromptMoveStart()
{
  g_Prompt.m_Cursor = g_Prompt.m_Start;
}

static void PromptMoveEnd()
{
  g_Prompt.m_Cursor = g_Prompt.m_Data.m_Length;
}

static void PromptMoveWordLeft()
{
  while ((u32)g_Prompt.m_Cursor > g_Prompt.m_Start && !g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor - 1].IsAlnum())
  {
    --g_Prompt.m_Cursor;
  }
  while ((u32)g_Prompt.m_Cursor > g_Prompt.m_Start && g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor - 1].IsAlnum())
  {
    --g_Prompt.m_Cursor;
  }
}

static void PromptMoveWordRight()
{
  while ((u32)g_Prompt.m_Cursor < g_Prompt.m_Data.m_Length && !g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor].IsAlnum())
  {
    ++g_Prompt.m_Cursor;
  }
  while ((u32)g_Prompt.m_Cursor < g_Prompt.m_Data.m_Length && g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor].IsAlnum())
  {
    ++g_Prompt.m_Cursor;
  }
}

static void Quit()
{
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    if (!(g_Editor.m_Frames[i].m_Flags & FRAME_UNSAVED))
    {
      continue;
    }
    
    InstallConfirmPromptBinds();
    BeginPrompt("Frames have unsaved changes, quit anyway? (y/n)");
    g_Prompt.m_Cursor = -1;
    while (!g_Prompt.m_Status)
    {
      RenderEditor();
      RenderPrompt();
      RenderPresent();
      
      ReadKey();
    }
    EndPrompt();
    
    if (g_Prompt.m_Status == PROMPT_FAIL)
    {
      InstallBaseBinds();
      return;
    }
    
    break;
  }
  
  g_Editor.m_Running = false;
}

static void Exit()
{
  InstallBaseBinds();
}

static void QuitPromptFail()
{
  g_Prompt.m_Status = PROMPT_FAIL;
}

static void QuitPromptSuccess()
{
  g_Prompt.m_Status = PROMPT_SUCCESS;
}

static void Next()
{
  g_Editor.m_CurFrame = g_Editor.m_CurFrame == g_Editor.m_NFrames - 1 ? 0 : g_Editor.m_CurFrame + 1;
}

static void Previous()
{
  g_Editor.m_CurFrame = g_Editor.m_CurFrame == 0 ? g_Editor.m_NFrames - 1 : g_Editor.m_CurFrame - 1;
}

static void WriteMode()
{
  InstallWriteBinds();
}

static void FrameDeleteFront()
{
  Frame&  f = CurrentFrame();
  if (f.m_Cursor < f.m_Buffer.m_Length)
  {
    f.Erase(f.m_Cursor);
  }
}

static void FrameDeleteBack()
{
  Frame&  f = CurrentFrame();
  if (f.m_Cursor == 0)
  {
    return;
  }
  
  if (f.m_Cursor < f.m_Buffer.m_Length)
  {
    u32 prev  = f.m_Buffer.m_Data[f.m_Cursor - 1].m_Codepoint;
    u32 cur   = f.m_Buffer.m_Data[f.m_Cursor].m_Codepoint;
    if ((prev == '(' && cur == ')') || (prev == '[' && cur == ']') || (prev == '{' && cur == '}') || (prev == '"' && cur == '"'))
    {
      --f.m_Cursor;
      f.Erase(f.m_Cursor, f.m_Cursor + 2);
      f.SaveCursor();
      return;
    }
  }
  
  if (g_Options.m_TabSpaces && g_Options.m_TabSize != 1)
  {
    // deletion of tabspace indentation requires special handling
    --f.m_Cursor;
    bool  done  = !f.m_Buffer.m_Data[f.m_Cursor].IsSpace();
    f.Erase(f.m_Cursor);
    
    if (!done)
    {
      u32 lineBegin = f.m_Cursor;
      while (lineBegin && f.m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
      {
        --lineBegin;
      }
      
      u32 linePos     = f.m_Cursor - lineBegin;
      u32 lastIndent  = linePos / g_Options.m_TabSize * g_Options.m_TabSize;
      while (linePos > lastIndent && f.m_Buffer.m_Data[f.m_Cursor - 1].m_Codepoint == ' ')
      {
        --f.m_Cursor;
        f.Erase(f.m_Cursor);
        --linePos;
      }
    }
    
    f.SaveCursor();
  }
  else
  {
    --f.m_Cursor;
    f.Erase(f.m_Cursor);
    f.SaveCursor();
  }
}

static void FrameDeleteWord()
{
  Frame&  f           = CurrentFrame();
  u32     upperBound  = f.m_Cursor;
  while (f.m_Cursor > 0 && !f.m_Buffer.m_Data[f.m_Cursor - 1].IsAlnum())
  {
    --f.m_Cursor;
  }
  while (f.m_Cursor > 0 && f.m_Buffer.m_Data[f.m_Cursor - 1].IsAlnum())
  {
    --f.m_Cursor;
  }
  f.SaveCursor();
  
  if (f.m_Cursor < upperBound)
  {
    f.Erase(f.m_Cursor, upperBound);
  }
}

static void PromptDeleteFront()
{
  if ((u32)g_Prompt.m_Cursor < g_Prompt.m_Data.m_Length)
  {
    g_Prompt.m_Data.Erase(g_Prompt.m_Cursor);
  }
}

static void PromptDeleteBack()
{
  if ((u32)g_Prompt.m_Cursor <= g_Prompt.m_Start)
  {
    return;
  }
  
  if ((u32)g_Prompt.m_Cursor < g_Prompt.m_Data.m_Length)
  {
    u32 prev  = g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor - 1].m_Codepoint;
    u32 cur   = g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor].m_Codepoint;
    if ((prev == '(' && cur == ')') || (prev == '[' && cur == ']') || (prev == '{' && cur == '}') || (prev == '"' && cur == '"'))
    {
      --g_Prompt.m_Cursor;
      PromptErase(g_Prompt.m_Cursor, g_Prompt.m_Cursor + 2);
      return;
    }
  }
  
  --g_Prompt.m_Cursor;
  PromptErase(g_Prompt.m_Cursor);
}

static void PromptDeleteWord()
{
  u32 upperBound  = g_Prompt.m_Cursor;
  while ((u32)g_Prompt.m_Cursor > g_Prompt.m_Start && !g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor - 1].IsAlnum())
  {
    --g_Prompt.m_Cursor;
  }
  while ((u32)g_Prompt.m_Cursor > g_Prompt.m_Start && g_Prompt.m_Data.m_Data[g_Prompt.m_Cursor - 1].IsAlnum())
  {
    --g_Prompt.m_Cursor;
  }
  
  if ((u32)g_Prompt.m_Cursor < upperBound)
  {
    PromptErase(g_Prompt.m_Cursor, upperBound);
  }
}

static void Newline()
{
  Frame&  f         = CurrentFrame();
  u32     lineBegin = f.m_Cursor;
  while (lineBegin > 0 && f.m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
  {
    --lineBegin;
  }
  
  u32 nIndent = 0;
  if (g_Options.m_TabSpaces)
  {
    u32 nSpaces = 0;
    while (lineBegin + nSpaces < f.m_Buffer.m_Length && f.m_Buffer.m_Data[lineBegin + nSpaces].m_Codepoint == ' ')
    {
      ++nSpaces;
    }
    nIndent = nSpaces / g_Options.m_TabSize;
  }
  else
  {
    while (lineBegin + nIndent < f.m_Buffer.m_Length && f.m_Buffer.m_Data[lineBegin + nIndent].m_Codepoint == '\t')
    {
      ++nIndent;
    }
  }
  
  // unfold parentheticals
  bool  unfolded  = false;
  if (f.m_Cursor > 0 && f.m_Cursor < f.m_Buffer.m_Length)
  {
    u32 prev  = f.m_Buffer.m_Data[f.m_Cursor - 1].m_Codepoint;
    u32 cur   = f.m_Buffer.m_Data[f.m_Cursor].m_Codepoint;
    if (prev == '(' || prev == '[' || prev == '{')
    {
      f.Write('\n', f.m_Cursor);
      ++f.m_Cursor;
      
      for (u32 i = 0; i < nIndent + 1; ++i)
      {
        f.m_Cursor = f.Tabulate(f.m_Cursor);
      }
      
      f.SaveCursor();
      
      unfolded = true;
    }
    
    if ((prev == '(' && cur == ')') || (prev == '[' && cur == ']') || (prev == '{' && cur == '}'))
    {
      u32 cursor  = f.m_Cursor;
      f.Write('\n', cursor);
      ++cursor;
      
      for (u32 i = 0; i < nIndent; ++i)
      {
        cursor = f.Tabulate(cursor);
      }
    }
  }
  
  if (unfolded)
  {
    return;
  }
  
  f.Write('\n', f.m_Cursor);
  ++f.m_Cursor;
  
  while (nIndent)
  {
    f.m_Cursor = f.Tabulate(f.m_Cursor);
    --nIndent;
  }
  
  f.SaveCursor();
}

static void Undo()
{
  Frame&  f = CurrentFrame();
  if (!f.m_HistoryLength)
  {
    Info("Binds: Nothing to undo");
    return;
  }
  
  f.Undo();
}

static void NewFrame()
{
  if (g_Editor.m_NFrames >= FUNCTIONAL::MAX_FILES)
  {
    Error("Binds: Cannot open more than %u frames!", FUNCTIONAL::MAX_FILES);
    return;
  }
  
  EmptyFrame(g_Editor.m_Frames[g_Editor.m_NFrames++]);
  g_Editor.m_CurFrame = g_Editor.m_NFrames - 1;
}

static void KillFrame()
{
  Frame&  f = CurrentFrame();
  if (!(f.m_Flags & FRAME_UNSAVED))
  {
    DestroyFrame(g_Editor.m_CurFrame);
    return;
  }
  
  InstallConfirmPromptBinds();
  BeginPrompt("Frame has unsaved changes, kill anyway? (y/n)");
  g_Prompt.m_Cursor = -1;
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    ReadKey();
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_SUCCESS)
  {
    DestroyFrame(g_Editor.m_CurFrame);
  }
}

static void Save()
{
  Frame&  f = CurrentFrame();
  if (f.m_Source)
  {
    if (f.m_Flags & FRAME_UNSAVED)
    {
      f.Save();
    }
    return;
  }
  
  InstallPathPromptBinds();
  BeginPrompt("Save as: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (WritableToPrompt(key))
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  char* path  = PromptDataCString();
  
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    if (!g_Editor.m_Frames[i].m_Source)
    {
      continue;
    }
    
    if (FileID(g_Editor.m_Frames[i].m_Source, true) == FileID(path, true))
    {
      Error("Binds: Cannot save multiple frames to a single file!");
      free(path);
      return;
    }
  }
  
  f.m_Source = path;
  f.Save();
}

static void Focus()
{
  Frame tmp = CurrentFrame();
  CurrentFrame() = g_Editor.m_Frames[0];
  g_Editor.m_Frames[0] = tmp;
  g_Editor.m_CurFrame = 0;
}

static void OpenFile()
{
  if (g_Editor.m_NFrames >= FUNCTIONAL::MAX_FILES)
  {
    Error("Binds: Cannot open more than %u frames!", FUNCTIONAL::MAX_FILES);
    return;
  }
  
  InstallPathPromptBinds();
  BeginPrompt("Open file: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (WritableToPrompt(key))
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  char* path  = PromptDataCString();
  
  for (usize i = 0; i < g_Editor.m_NFrames; ++i)
  {
    if (!g_Editor.m_Frames[i].m_Source)
    {
      continue;
    }
    
    if (FileID(g_Editor.m_Frames[i].m_Source, true) == FileID(path, true))
    {
      // redirect user to already opened file
      g_Editor.m_CurFrame = i;
      free(path);
      return;
    }
  }
  
  if (FileFrame(g_Editor.m_Frames[g_Editor.m_NFrames], path))
  {
    free(path);
    return;
  }
  
  free(path);
  
  g_Editor.m_CurFrame = g_Editor.m_NFrames;
  ++g_Editor.m_NFrames;
}

static void Search()
{
  InstallPromptBinds();
  BeginPrompt("Search literally: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (WritableToPrompt(key))
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  Frame&  f       = CurrentFrame();
  EString needle  = PromptData();
  if (needle.m_Length == 0)
  {
    needle.Free();
    return;
  }
  
  for (usize i = f.m_Cursor + 1; i + needle.m_Length <= f.m_Buffer.m_Length; ++i)
  {
    for (usize j = 0; j < needle.m_Length; ++j)
    {
      if (needle.m_Data[j].m_Codepoint != f.m_Buffer.m_Data[i + j].m_Codepoint)
      {
        goto nextChar;
      }
    }
    
    f.m_Cursor = i;
    f.SaveCursor();
    needle.Free();
    return;
  nextChar:;
  }
  
  Info("Binds: Didn't find search string");
  needle.Free();
}

static void ReverseSearch()
{
  InstallPromptBinds();
  BeginPrompt("Reverse search literally: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (WritableToPrompt(key))
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  Frame&  f       = CurrentFrame();
  EString needle  = PromptData();
  if (needle.m_Length == 0)
  {
    needle.Free();
    return;
  }
  
  for (isize i = f.m_Cursor; i >= (isize)needle.m_Length; --i)
  {
    for (isize j = 0; j < (isize)needle.m_Length; ++j)
    {
      if (needle.m_Data[j].m_Codepoint != f.m_Buffer.m_Data[i - needle.m_Length + j].m_Codepoint)
      {
        goto nextChar;
      }
    }
    
    f.m_Cursor = i - needle.m_Length;
    f.SaveCursor();
    needle.Free();
    return;
  nextChar:;
  }
  
  Info("Binds: Didn't find search string");
  needle.Free();
}

static void FrameLeftParen()
{
  Frame&  f = CurrentFrame();
  f.Write("()", f.m_Cursor);
  ++f.m_Cursor;
  f.SaveCursor();
}

static void FrameLeftBracket()
{
  Frame&  f = CurrentFrame();
  f.Write("[]", f.m_Cursor);
  ++f.m_Cursor;
  f.SaveCursor();
}

static void FrameLeftBrace()
{
  Frame&  f = CurrentFrame();
  f.Write("{}", f.m_Cursor);
  ++f.m_Cursor;
  f.SaveCursor();
}

static void FrameDoubleQuote()
{
  Frame&  f = CurrentFrame();
  f.Write("\"\"", f.m_Cursor);
  ++f.m_Cursor;
  f.SaveCursor();
}

static void PromptLeftParen()
{
  PromptWrite("()", g_Prompt.m_Cursor);
  ++g_Prompt.m_Cursor;
}

static void PromptLeftBracket()
{
  PromptWrite("[]", g_Prompt.m_Cursor);
  ++g_Prompt.m_Cursor;
}

static void PromptLeftBrace()
{
  PromptWrite("{}", g_Prompt.m_Cursor);
  ++g_Prompt.m_Cursor;
}

static void PromptDoubleQuote()
{
  PromptWrite("\"\"", g_Prompt.m_Cursor);
  ++g_Prompt.m_Cursor;
}

static void Paste()
{
  if (!g_Editor.m_Clipboard.m_Data)
  {
    Info("Binds: Clipboard is empty");
    return;
  }
  
  Frame&  f = CurrentFrame();
  
  f.SaveCursor();
  while (f.m_Cursor < f.m_Buffer.m_Length && f.m_Buffer.m_Data[f.m_Cursor].m_Codepoint != '\n')
  {
    ++f.m_Cursor;
  }
  
  f.Write('\n', f.m_Cursor);
  ++f.m_Cursor;
  f.Write(g_Editor.m_Clipboard, f.m_Cursor);
  f.LoadCursor();
}

static void CopyLine()
{
  Frame&  f = CurrentFrame();
  
  u32 lineBegin = f.m_Cursor;
  while (lineBegin > 0 && f.m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
  {
    --lineBegin;
  }
  
  u32 lineEnd = f.m_Cursor;
  while (lineEnd < f.m_Buffer.m_Length && f.m_Buffer.m_Data[lineEnd].m_Codepoint != '\n')
  {
    ++lineEnd;
  }
  
  g_Editor.m_Clipboard.Free();
  g_Editor.m_Clipboard = f.m_Buffer.Substring(lineBegin, lineEnd);
  
  Info("Binds: Copied %u characters", lineEnd - lineBegin);
}

static void CutLine()
{
  Frame&  f = CurrentFrame();
  
  u32 lineBegin = f.m_Cursor;
  while (lineBegin > 0 && f.m_Buffer.m_Data[lineBegin - 1].m_Codepoint != '\n')
  {
    --lineBegin;
  }
  
  u32 lineEnd = f.m_Cursor;
  while (lineEnd < f.m_Buffer.m_Length && f.m_Buffer.m_Data[lineEnd].m_Codepoint != '\n')
  {
    ++lineEnd;
  }
  
  g_Editor.m_Clipboard.Free();
  g_Editor.m_Clipboard = f.m_Buffer.Substring(lineBegin, lineEnd);
  
  f.SaveCursor();
  f.Erase(lineBegin, lineEnd + (lineEnd < f.m_Buffer.m_Length));
  f.m_Cursor = lineBegin;
  f.LoadCursor();
  
  Info("Binds: Cut %u characters", lineEnd - lineBegin);
}

static void CopyLines()
{
  InstallNumberPromptBinds();
  BeginPrompt("Copy lines: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (key.m_Codepoint < 128 && key.IsDigit())
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  char* linesString = PromptDataCString();
  u64   lines       = strtoll(linesString, nullptr, 10);
  free(linesString);
  
  if (lines == 0)
  {
    Info("Binds: Ignoring copy of zero lines");
    return;
  }
  
  Frame&  f = CurrentFrame();
  
  u32 begin = f.m_Cursor;
  while (begin > 0 && f.m_Buffer.m_Data[begin - 1].m_Codepoint != '\n')
  {
    --begin;
  }
  
  u32 end = begin;
  while (end < f.m_Buffer.m_Length)
  {
    lines -= f.m_Buffer.m_Data[end].m_Codepoint == '\n';
    if (lines == 0)
    {
      break;
    }
    ++end;
  }
  
  g_Editor.m_Clipboard.Free();
  g_Editor.m_Clipboard = f.m_Buffer.Substring(begin, end);
  
  Info("Binds: Copied %u characters", end - begin);
}

static void CutLines()
{
  InstallNumberPromptBinds();
  BeginPrompt("Cut lines: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (key.m_Codepoint < 128 && key.IsDigit())
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  char* linesString = PromptDataCString();
  u64   lines       = strtoll(linesString, nullptr, 10);
  free(linesString);
  
  if (lines == 0)
  {
    Info("Binds: Ignoring copy of zero lines");
    return;
  }
  
  Frame&  f = CurrentFrame();
  
  u32 begin = f.m_Cursor;
  while (begin > 0 && f.m_Buffer.m_Data[begin - 1].m_Codepoint != '\n')
  {
    --begin;
  }
  
  u32 end = begin;
  while (end < f.m_Buffer.m_Length)
  {
    lines -= f.m_Buffer.m_Data[end].m_Codepoint == '\n';
    if (lines == 0)
    {
      break;
    }
    ++end;
  }
  
  g_Editor.m_Clipboard.Free();
  g_Editor.m_Clipboard = f.m_Buffer.Substring(begin, end);
  
  f.SaveCursor();
  f.Erase(begin, end + (end < f.m_Buffer.m_Length));
  f.m_Cursor = begin;
  f.LoadCursor();
  
  Info("Binds: Cut %u characters", end - begin);
}

static void CopyUntilLine()
{
  // TODO: implement CopyUntilLine() bind
}

static void CutUntilLine()
{
  // TODO: implement CutUntilLine() bind
}

static void Zoom()
{
  u32 x {};
  u32 y {};
  u32 w {};
  u32 h {};
  ArrangeFrame(g_Editor.m_CurFrame, x, y, w, h);
  
  Frame&  f = CurrentFrame();
  f.m_Start = 0;
  f.ComputeBounds(w, h / 2);
}

static void Goto()
{
  InstallNumberPromptBinds();
  BeginPrompt("Goto line: ");
  while (!g_Prompt.m_Status)
  {
    RenderEditor();
    RenderPrompt();
    RenderPresent();
    
    EChar key = ReadKey();
    if (key.m_Codepoint < 128 && key.IsDigit())
    {
      PromptWrite(key, g_Prompt.m_Cursor);
      ++g_Prompt.m_Cursor;
    }
  }
  EndPrompt();
  InstallBaseBinds();
  
  if (g_Prompt.m_Status == PROMPT_FAIL)
  {
    return;
  }
  
  char* lineString  = PromptDataCString();
  u64   line        = strtoll(lineString, nullptr, 10);
  free(lineString);
  line -= line > 0;
  
  Frame&  f = CurrentFrame();
  
  // move cursor to needed line
  f.m_Cursor = 0;
  while (f.m_Cursor < f.m_Buffer.m_Length && line)
  {
    if (f.m_Buffer.m_Data[f.m_Cursor++].m_Codepoint == '\n')
    {
      --line;
    }
  }
  f.SaveCursor();
  
  // focus selected line
  u32 x {};
  u32 y {};
  u32 w {};
  u32 h {};
  ArrangeFrame(g_Editor.m_CurFrame, x, y, w, h);
  
  f.m_Start = 0;
  f.ComputeBounds(w, h / 2);
}

static void RecordMacro()
{
  if (!IsRecordingMacro())
  {
    Info("Binds: Recording macro");
    RecordMacro();
  }
}

static void ExecuteMacro()
{
  if (IsRecordingMacro())
  {
    Info("Binds: Stopped recording macro");
    StopRecordingMacro();
  }
  else
  {
    Info("Binds: Executing macro");
    ExecuteMacro();
  }
}

static void Help()
{
  if (g_Editor.m_NFrames >= FUNCTIONAL::MAX_FILES)
  {
    Error("Binds: Cannot open more than %u frames, kill some using C-k!", FUNCTIONAL::MAX_FILES);
    return;
  }
  
  StringFrame(g_Editor.m_Frames[g_Editor.m_NFrames++], FRAME::HELP_TEXT);
  g_Editor.m_CurFrame = g_Editor.m_NFrames - 1;
}

static void Tab()
{
  Frame&  f = CurrentFrame();
  f.m_Cursor = f.Tabulate(f.m_Cursor);
  f.SaveCursor();
}

static void Complete()
{
  CompletePromptPath();
}

}
