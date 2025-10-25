// SPDX-License-Identifier: GPL-3.0-or-later

#include <Args.hh>
#include <Editor.hh>
#include <Options.hh>
#include <Render.hh>

int main(int argc, char* argv[])
{
  if (ParseArgs(argc, argv))
  {
    return (1);
  }
  
  if (ParseOptions())
  {
    return (1);
  }
  
  if (InitRender())
  {
    return (1);
  }
  
  if (InitEditor())
  {
    QuitRender(false);
    return (1);
  }
  
  EditorLoop();
  QuitRender(true);
}
