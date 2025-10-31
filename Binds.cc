// SPDX-License-Identifier: GPL-3.0-or-later

#include <Binds.hh>
#include <Editor.hh>
#include <Input.hh>
#include <Options.hh>
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
  // TODO: implement
}

static void PromptMoveRight()
{
  // TODO: implement
}

static void PromptMoveStart()
{
  // TODO: implement
}

static void PromptMoveEnd()
{
  // TODO: implement
}

static void PromptMoveWordLeft()
{
  // TODO: implement
}

static void PromptMoveWordRight()
{
  // TODO: implement
}

static void Quit()
{
  // TODO: write logic for unsaved loss prevention
  
  g_Editor.m_Running = false;
}

static void Exit()
{
  InstallBaseBinds();
}

static void QuitPromptFail()
{
  // TODO: implement
}

static void QuitPromptSuccess()
{
  // TODO: implement
}

static void Next()
{
  // TODO: implement
}

static void Previous()
{
  // TODO: implement
}

static void WriteMode()
{
  InstallWriteBinds();
}

static void FrameDeleteFront()
{
  // TODO: implement
}

static void FrameDeleteBack()
{
  // TODO: implement
}

static void FrameDeleteWord()
{
  // TODO: implement
}

static void PromptDeleteFront()
{
  // TODO: implement
}

static void PromptDeleteBack()
{
  // TODO: implement
}

static void PromptDeleteWord()
{
  // TODO: implement
}

static void Newline()
{
  // TODO: implement
}

static void Undo()
{
  // TODO: implement
}

static void NewFrame()
{
  // TODO: implement
}

static void KillFrame()
{
  // TODO: implement
}

static void Save()
{
  // TODO: implement
}

static void Focus()
{
  // TODO: implement
}

static void OpenFile()
{
  // TODO: implement
}

static void Search()
{
  // TODO: implement
}

static void ReverseSearch()
{
  // TODO: implement
}

static void FrameLeftParen()
{
  // TODO: implement
}

static void FrameLeftBracket()
{
  // TODO: implement
}

static void FrameLeftBrace()
{
  // TODO: implement
}

static void FrameDoubleQuote()
{
  // TODO: implement
}

static void PromptLeftParen()
{
  // TODO: implement
}

static void PromptLeftBracket()
{
  // TODO: implement
}

static void PromptLeftBrace()
{
  // TODO: implement
}

static void PromptDoubleQuote()
{
  // TODO: implement
}

static void Paste()
{
  // TODO: implement
}

static void CopyLine()
{
  // TODO: implement
}

static void CutLine()
{
  // TODO: implement
}

static void CopyLines()
{
  // TODO: implement
}

static void CutLines()
{
  // TODO: implement
}

static void Zoom()
{
  // TODO: implement
}

static void Goto()
{
  // TODO: implement
}

static void RecordMacro()
{
  // TODO: implement
}

static void ExecuteMacro()
{
  // TODO: implement
}

static void Help()
{
  // TODO: implement
}

static void Tab()
{
  // TODO: implement
}

static void Complete()
{
  // TODO: implement
}

}
