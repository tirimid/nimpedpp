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

static void ApplyColor(Color color);
static void SIGWINCHHandler(int arg);

static EChar*         g_CellChars;
static Color*         g_CellColors;
static u32            g_Width;
static u32            g_Height;
static struct termios g_OldTermIOS;
static EString        g_Bar;
static u32            g_BarHeight;

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
  g_CellColors = (Color*)calloc(g_Width * g_Height, sizeof(Color));
  
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
  if (x >= g_Width || y >= g_Height)
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

void  RenderFill(Color color, u32 x, u32 y, u32 w, u32 h)
{
  if (x >= g_Width || y >= g_Height)
  {
    return;
  }
  
  w = x + w >= g_Width ? g_Width - x : w;
  h = y + h >= g_Height ? g_Height - y : h;
  
  for (u32 cx = x; cx < x + w; ++cx)
  {
    for (u32 cy = y; cy < y + h; ++cy)
    {
      g_CellColors[g_Width * cy + cx] = color;
    }
  }
}

void  RenderFill(EChar ch, Color color, u32 x, u32 y, u32 w, u32 h)
{
  if (x >= g_Width || y >= g_Height)
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
      g_CellColors[g_Width * cy + cx] = color;
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

void  RenderPut(Color color, u32 x, u32 y)
{
  if (x >= g_Width || y >= g_Height)
  {
    return;
  }
  
  g_CellColors[g_Width * y + x] = color;
}

void  RenderPut(EChar ch, Color color, u32 x, u32 y)
{
  if (x >= g_Width || y >= g_Height)
  {
    return;
  }
  
  g_CellChars[g_Width * y + x] = ch;
  g_CellColors[g_Width * y + x] = color;
}

void  RenderGet(OUT EChar& ch, u32 x, u32 y)
{
  ch = g_CellChars[g_Width * y + x];
}

void  RenderGet(OUT Color& color, u32 x, u32 y)
{
  color = g_CellColors[g_Width * y + x];
}

void  RenderGet(OUT EChar& ch, OUT Color& color, u32 x, u32 y)
{
  ch = g_CellChars[g_Width * y + x];
  color = g_CellColors[g_Width * y + x];
}

void  RenderPresent()
{
  // allow cursor to be drawn on newline if width is exceeded
  u32 barHeight = g_BarHeight + ((u32)g_Prompt.m_Cursor >= g_Prompt.m_Data.m_Length && g_Prompt.m_Cursor % g_Width == 0);
  
  fputs("\x1b[H\x1b[0m", stdout);
  
  // draw out the bar
  ApplyColor(g_Options.m_Global);
  for (usize i = 0; i < g_Width * barHeight; ++i)
  {
    if ((i64)i == g_Prompt.m_Cursor)
    {
      ApplyColor(g_Options.m_GlobalCursor);
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
      ApplyColor(g_Options.m_Global);
    }
  }
  
  // draw out the rendered frame
  Color color {};
  ApplyColor(color);
  for (usize i = 0; i < g_Width * (g_Height - barHeight); ++i)
  {
    if (g_CellColors[i].m_FG != color.m_FG || g_CellColors[i].m_BG != color.m_BG)
    {
      color = g_CellColors[i];
      ApplyColor(color);
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
  g_BarHeight = (g_Bar.m_Length + g_Width - 1) / g_Width;
  g_BarHeight += !g_BarHeight;
}

void  RenderBar(const char* str)
{
  g_Bar.Free();
  g_Bar = str;
  g_BarHeight = (g_Bar.m_Length + g_Width - 1) / g_Width;
  g_BarHeight += !g_BarHeight;
}

static void ApplyColor(Color color)
{
  u8  fg0 = color.m_FG % 10;
  u8  fg1 = color.m_FG / 10 % 10;
  u8  fg2 = color.m_FG / 100;
  
  u8  bg0 = color.m_BG % 10;
  u8  bg1 = color.m_BG / 10 % 10;
  u8  bg2 = color.m_BG / 100;
  
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
  
  g_CellChars = (EChar*)reallocarray(g_CellChars, g_Width * g_Height, sizeof(EChar));
  g_CellColors = (Color*)reallocarray(g_CellColors, g_Width * g_Height, sizeof(Color));
  
  RenderBar(g_Bar.Copy()); // recompute bar
}
