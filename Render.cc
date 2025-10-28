// SPDX-License-Identifier: GPL-3.0-or-later

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <Prompt.hh>
#include <Render.hh>

extern "C"
{
#include <sys/ioctl.h>
#include <termios.h>
}

static void ApplyAttributes(u8 fg, u8 bg);
static void SIGWINCHHandler(int arg);

static EChar*         g_CellChars;
static Attributes*    g_CellAttributes;
static u32            g_Width;
static u32            g_Height;
static struct termios g_OldTermIOS;
static EString        g_Bar;
static u32            g_BarHeight;

Attributes::Attributes()
{
}

Attributes::Attributes(u8 fg, u8 bg)
  : m_FG(fg),
  m_BG(bg)
{
}

Attributes::Attributes(Color color)
  : m_FG(color.m_FG),
  m_BG(color.m_BG)
{
}

i32 InitRender()
{
  if (tcgetattr(STDIN_FILENO, &g_OldTermIOS))
  {
    Error("Render: Failed on tcgetattr() for old stdin!");
    return (1);
  }
  
  struct termios  newTermIOS  = g_OldTermIOS;
  newTermIOS.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  newTermIOS.c_iflag &= ~(ICRNL | IXON | ISTRIP);
  newTermIOS.c_oflag &= ~OPOST;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTermIOS))
  {
    Error("Render: Failed on tcsetattr() for new stdin!");
    return (1);
  }
  
  setvbuf(stdin, nullptr, _IONBF, 0);
  
  printf("\x1b[?25l");
  
  struct winsize  winSize {};
  ioctl(0, TIOCGWINSZ, &winSize);
  g_Width = winSize.ws_col;
  g_Height = winSize.ws_row;
  
  g_CellChars = (EChar*)calloc(g_Width * g_Height, sizeof(EChar));
  g_CellAttributes = (Attributes*)calloc(g_Width * g_Height, sizeof(Attributes));
  
  struct sigaction  sigAction {};
  sigaction(SIGWINCH, nullptr, &sigAction);
  sigAction.sa_handler = SIGWINCHHandler;
  sigaction(SIGWINCH, &sigAction, nullptr);
  
  return (0);
}

void  QuitRender(bool clearScreen)
{
  printf("\x1b[?25h\x1b[0m\r");
  
  if (clearScreen)
  {
    printf("\x1b[2J\x1b[H");
  }
  
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_OldTermIOS))
  {
    Error("Render: Failed on tcsetattr() for old stdin!");
  }
}

void  RenderFill(EChar ch, u32 x, u32 y, u32 w, u32 h)
{
  if (x >= g_Width || y >= g_Width)
  {
    return;
  }
  
  w = x + w >= g_Width ? g_Width - x : w;
  h = y + h >= g_Height ? g_Height - y : h;
  
  for (u32 cx = x; cx < x + w; ++cx)
  {
    for (u32 cy = y; cy < y + h; ++cy)
    {
      g_CellChars[g_Width * cy + cx] = ch;
    }
  }
}

void  RenderFill(Attributes a, u32 x, u32 y, u32 w, u32 h)
{
  if (x >= g_Width || y >= g_Width)
  {
    return;
  }
  
  w = x + w >= g_Width ? g_Width - x : w;
  h = y + h >= g_Height ? g_Height - y : h;
  
  for (u32 cx = x; cx < x + w; ++cx)
  {
    for (u32 cy = y; cy < y + h; ++cy)
    {
      g_CellAttributes[g_Width * cy + cx] = a;
    }
  }
}

void  RenderFill(EChar ch, Attributes a, u32 x, u32 y, u32 w, u32 h)
{
  if (x >= g_Width || y >= g_Width)
  {
    return;
  }
  
  w = x + w >= g_Width ? g_Width - x : w;
  h = y + h >= g_Height ? g_Height - y : h;
  
  for (u32 cx = x; cx < x + w; ++cx)
  {
    for (u32 cy = y; cy < y + h; ++cy)
    {
      g_CellChars[g_Width * cy + cx] = ch;
      g_CellAttributes[g_Width * cy + cx] = a;
    }
  }
}

void  RenderPut(EChar ch, u32 x, u32 y)
{
  if (x >= g_Width || y >= g_Height)
  {
    return;
  }
  
  g_CellChars[g_Width * y + x] = ch;
}

void  RenderPut(Attributes a, u32 x, u32 y)
{
  if (x >= g_Width || y >= g_Height)
  {
    return;
  }
  
  g_CellAttributes[g_Width * y + x] = a;
}

void  RenderPut(EChar ch, Attributes a, u32 x, u32 y)
{
  if (x >= g_Width || y >= g_Height)
  {
    return;
  }
  
  g_CellChars[g_Width * y + x] = ch;
  g_CellAttributes[g_Width * y + x] = a;
}

void  RenderGet(OUT EChar& ch, u32 x, u32 y)
{
  ch = g_CellChars[g_Width * y + x];
}

void  RenderGet(OUT Attributes& a, u32 x, u32 y)
{
  a = g_CellAttributes[g_Width * y + x];
}

void  RenderGet(OUT EChar& ch, OUT Attributes& a, u32 x, u32 y)
{
  ch = g_CellChars[g_Width * y + x];
  a = g_CellAttributes[g_Width * y + x];
}

void  RenderPresent()
{
  // allow cursor to be drawn on newline if width is exceeded
  u32 barHeight = g_BarHeight + ((u32)g_Prompt.m_Cursor >= g_Prompt.m_Data.m_Length && g_Prompt.m_Cursor % g_Width == 0);
  
  fputs("\x1b[H\x1b[0m", stdout);
  
  // draw out the bar
  ApplyAttributes(g_Options.m_Global.m_FG, g_Options.m_Global.m_BG);
  for (usize i = 0; i < g_Width * barHeight; ++i)
  {
    if ((i64)i == g_Prompt.m_Cursor)
    {
      ApplyAttributes(g_Options.m_GlobalCursor.m_FG, g_Options.m_GlobalCursor.m_BG);
    }
    
    if (i < g_Bar.m_Length)
    {
      PrintEChar(g_Bar.m_Data[i]);
    }
    else
    {
      fputc(' ', stdout);
    }
    
    if ((i64)i == g_Prompt.m_Cursor)
    {
      ApplyAttributes(g_Options.m_Global.m_FG, g_Options.m_Global.m_BG);
    }
  }
  
  // draw out the rendered frame
  Attributes  a {};
  for (usize i = 0; i < g_Width * (g_Height - barHeight); ++i)
  {
    if (g_CellAttributes[i].m_FG != a.m_FG || g_CellAttributes[i].m_BG != a.m_BG)
    {
      a = g_CellAttributes[i];
      ApplyAttributes(a.m_FG, a.m_BG);
    }
    
    PrintEChar(g_CellChars[i]);
  }
}

void  WindowSize(OUT u32& width, OUT u32& height)
{
  width = g_Width;
  height = g_Height - g_BarHeight - ((u32)g_Prompt.m_Cursor >= g_Prompt.m_Data.m_Length && g_Prompt.m_Cursor % g_Width == 0);
}

void  RenderBar(OWNS EString str)
{
  g_Bar.Free();
  g_Bar = str;
}

void  RenderBar(const char* str)
{
  g_Bar.Free();
  g_Bar = str;
}

static void ApplyAttributes(u8 fg, u8 bg)
{
  u8  fg0 = fg % 10;
  u8  fg1 = fg / 10 % 10;
  u8  fg2 = fg / 100;
  
  u8  bg0 = bg % 10;
  u8  bg1 = bg / 10 % 10;
  u8  bg2 = bg / 100;
  
  char  buffer[]  = "\x1b[38;5;000m\x1b[48;5;000m\0";
  buffer[7]   += fg2;
  buffer[8]   += fg1;
  buffer[9]   += fg0;
  buffer[18]  += bg2;
  buffer[19]  += bg1;
  buffer[20]  += bg0;
  
  fputs(buffer, stdout);
}

static void SIGWINCHHandler(int arg)
{
  (void)arg;
  
  struct winsize  winSize {};
  ioctl(0, TIOCGWINSZ, &winSize);
  
  g_Width = winSize.ws_col;
  g_Height = winSize.ws_row;
  RenderBar(g_Bar.Copy()); // recompute bar
}
