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
static void Quit();
static void QuitPromptFail();
static void QuitPromptSuccess();
static void Next();
static void Previous();
static void FrameDeleteFront();
static void FrameDeleteBack();
static void FrameDeleteWord();
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

}

void  InstallBaseBinds()
{
  Unbind();
  Bind(KEYBIND::FRAME_MOVE_LEFT, Binds::FrameMoveLeft);
  Bind(KEYBIND::QUIT, Binds::Quit);
  OrganizeInputs();
  
  g_Editor.m_WriteInput = false;
  
  RenderBar(VISUAL::BASE_NAME);
}

void  InstallWriteBinds()
{
  // TODO: implement
}

void  InstallPromptBinds()
{
  // TODO: implement
}

void  InstallPathPromptBinds()
{
  // TODO: implement
}

void  InstallConfirmPromptBinds()
{
  // TODO: implement
}

void  InstallNumberPromptBinds()
{
  // TODO: implement
}

namespace Binds
{

static void FrameMoveLeft()
{
  // TODO: implement
}

static void FrameMoveRight()
{
  // TODO: implement
}

static void FrameMoveUp()
{
  // TODO: implement
}

static void FrameMoveDown()
{
  // TODO: implement
}

static void FrameMoveStart()
{
  // TODO: implement
}

static void FrameMoveEnd()
{
  // TODO: implement
}

static void FrameMoveWordLeft()
{
  // TODO: implement
}

static void FrameMoveWordRight()
{
  // TODO: implement
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

static void Quit()
{
  // TODO: write logic for unsaved loss prevention
  
  g_Editor.m_Running = false;
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

}
