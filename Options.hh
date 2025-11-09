// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>

#define KEY(key)      EChar{(u32)key}
#define KEY_END       KEY(0)
#define KEY_CTRL(key) KEY(key - 'a' + 1)
#define KEY_FN(n)     KEY(27), KEY(79), KEY(79 + n)
#define KEY_META(key) KEY(27), KEY(key)

enum LangMode
{
  LANG_MODE_C = 0,
  LANG_MODE_SH,
  LANG_MODE_JS,
  LANG_MODE_CC,
  
  LANG_MODE_END
};

struct INTERNAL
{
  static constexpr usize        MAX_LOG_LENGTH      = 512;
  static constexpr usize        CONFIG_KEY_LENGTH   = 128;
  static constexpr usize        CONFIG_VALUE_LENGTH = 128;
  static constexpr const char*  CONFIG_SCAN         = "%127s = %127[^\r\n]";
  static constexpr const char*  CONFIG_COLOR_SCAN   = "%127s %127s";
};

struct FUNCTIONAL
{
  static constexpr usize        MAX_FILES         = 8;
  static constexpr const char*  CONFIG_DIR        = ".config/nimped++";
  static constexpr const char*  COLOR_CONF        = "color.conf";
  static constexpr const char*  LANG_CONF         = "lang.conf";
  static constexpr const char*  EDITOR_CONF       = "editor.conf";
  static constexpr usize        MAX_BAR_LENGTH    = 512;
  static constexpr usize        MAX_PROMPT_LENGTH = 512;
  static constexpr usize        MAX_BIND_LENGTH   = 16;
  static constexpr usize        MAX_BINDS         = 128;
};

struct FRAME
{
  static constexpr const char*  GREETER_TEXT  =
    "\n"
    "         _                          _\n"
    "   _ __ (_)_ __ ___  _ __   ___  __| |  _     _\n"
    "  | '_ \\| | '_ ` _ \\| '_ \\ / _ \\/ _` |_| |_ _| |_\n"
    "  | | | | | | | | | | |_) |  __/ (_| |_   _|_   _|\n"
    "  |_| |_|_|_| |_| |_| .__/ \\___|\\__,_| |_|   |_|\n"
    "                    |_|\n"
    "\n"
    "Resources:\n"
    "\n"
    "  Source code: https://git.tirimid.net/nimpedpp\n"
    "  Press C-h to bring up help information\n"
    "\n"
    "nimped++ is Free Software licensed under the GNU GPLv3.\n";
  
  // TODO: write up help text
  static constexpr const char*  HELP_TEXT     = "nimped++ help text (placeholder)\n";
};

struct VISUAL
{
  static constexpr char         MARGIN_CHAR   = '|';
  static constexpr const char*  SCRATCH_NAME  = "(Scratch)";
  static constexpr const char*  BASE_NAME     = "Base";
  static constexpr const char*  WRITE_NAME    = "Write";
};

struct KEYBIND
{
  static constexpr EChar  FRAME_MOVE_LEFT[]         = {KEY('h'), KEY_END};
  static constexpr EChar  FRAME_MOVE_RIGHT[]        = {KEY('l'), KEY_END};
  static constexpr EChar  FRAME_MOVE_UP[]           = {KEY('k'), KEY_END};
  static constexpr EChar  FRAME_MOVE_DOWN[]         = {KEY('j'), KEY_END};
  static constexpr EChar  FRAME_MOVE_START[]        = {KEY('a'), KEY_END};
  static constexpr EChar  FRAME_MOVE_END[]          = {KEY('e'), KEY_END};
  static constexpr EChar  FRAME_MOVE_WORD_LEFT[]    = {KEY('b'), KEY_END};
  static constexpr EChar  FRAME_MOVE_WORD_RIGHT[]   = {KEY('f'), KEY_END};
  static constexpr EChar  PROMPT_MOVE_LEFT[]        = {KEY_CTRL('b'), KEY_END};
  static constexpr EChar  PROMPT_MOVE_RIGHT[]       = {KEY_CTRL('f'), KEY_END};
  static constexpr EChar  PROMPT_MOVE_START[]       = {KEY_CTRL('a'), KEY_END};
  static constexpr EChar  PROMPT_MOVE_END[]         = {KEY_CTRL('e'), KEY_END};
  static constexpr EChar  PROMPT_MOVE_WORD_LEFT[]   = {KEY_META('b'), KEY_END};
  static constexpr EChar  PROMPT_MOVE_WORD_RIGHT[]  = {KEY_META('f'), KEY_END};
  static constexpr EChar  QUIT[]                    = {KEY_CTRL('x'), KEY_CTRL('c'), KEY_END};
  static constexpr EChar  EXIT[]                    = {KEY_CTRL('g'), KEY_END};
  static constexpr EChar  NEXT[]                    = {KEY('n'), KEY_END};
  static constexpr EChar  PREVIOUS[]                = {KEY('p'), KEY_END};
  static constexpr EChar  WRITE_MODE[]              = {KEY('i'), KEY_END};
  static constexpr EChar  DELETE_FRONT[]            = {KEY_CTRL('d'), KEY_END};
  static constexpr EChar  DELETE_BACK[]             = {KEY(127), KEY_END};
  static constexpr EChar  DELETE_WORD[]             = {KEY_META(127), KEY_END};
  static constexpr EChar  NEWLINE[]                 = {KEY(13), KEY_END};
  static constexpr EChar  UNDO[]                    = {KEY('u'), KEY_END};
  static constexpr EChar  NEW_FRAME[]               = {KEY_CTRL('n'), KEY_END};
  static constexpr EChar  KILL_FRAME[]              = {KEY_CTRL('k'), KEY_END};
  static constexpr EChar  SAVE[]                    = {KEY_CTRL('s'), KEY_END};
  static constexpr EChar  FOCUS[]                   = {KEY('m'), KEY_END};
  static constexpr EChar  OPEN_FILE[]               = {KEY_CTRL('f'), KEY_END};
  static constexpr EChar  COMPLETE[]                = {KEY(9), KEY_END};
  static constexpr EChar  SEARCH[]                  = {KEY('/'), KEY_END};
  static constexpr EChar  REVERSE_SEARCH[]          = {KEY('?'), KEY_END};
  static constexpr EChar  LEFT_PAREN[]              = {KEY('('), KEY_END};
  static constexpr EChar  LEFT_BRACKET[]            = {KEY('['), KEY_END};
  static constexpr EChar  LEFT_BRACE[]              = {KEY('{'), KEY_END};
  static constexpr EChar  DOUBLE_QUOTE[]            = {KEY('"'), KEY_END};
  static constexpr EChar  PASTE[]                   = {KEY('v'), KEY_END};
  static constexpr EChar  COPY_LINE[]               = {KEY('c'), KEY_END};
  static constexpr EChar  CUT_LINE[]                = {KEY('d'), KEY_END};
  static constexpr EChar  COPY_LINES[]              = {KEY('q'), KEY('c'), KEY_END};
  static constexpr EChar  CUT_LINES[]               = {KEY('q'), KEY('d'), KEY_END};
  static constexpr EChar  COPY_UNTIL_LINE[]         = {KEY('o'), KEY('c'), KEY_END};
  static constexpr EChar  CUT_UNTIL_LINE[]          = {KEY('o'), KEY('d'), KEY_END};
  static constexpr EChar  ZOOM[]                    = {KEY('z'), KEY_END};
  static constexpr EChar  GOTO[]                    = {KEY('g'), KEY_END};
  static constexpr EChar  RECORD_MACRO[]            = {KEY_FN(3), KEY_END};
  static constexpr EChar  EXECUTE_MACRO[]           = {KEY_FN(4), KEY_END};
  static constexpr EChar  PROMPT_YES[]              = {KEY('y'), KEY_END};
  static constexpr EChar  PROMPT_NO[]               = {KEY('n'), KEY_END};
  static constexpr EChar  HELP[]                    = {KEY_CTRL('h'), KEY_END};
  static constexpr EChar  TAB[]                     = {KEY(9), KEY_END};
};

struct Color
{
  u8  m_FG;
  u8  m_BG;
};

struct NamedColor
{
  const char* m_Name;
  u8          m_Color;
};

struct DynamicOptions
{
  // layout options
  u32         m_MasterNumer;
  u32         m_MasterDenom;
  u32         m_LeftGutter;
  u32         m_RightGutter;
  u32*        m_Margins;
  usize       m_NMargins;
  u32         m_TabSize;
  
  // editing options
  bool        m_TabSpaces;
  
  // theme options
  Color       m_Global;
  Color       m_GlobalCursor;
  Color       m_Window;
  Color       m_CurrentWindow;
  Color       m_Normal;
  Color       m_LineNumber;
  Color       m_LineNumberHighlight;
  Color       m_Margin;
  Color       m_Cursor;
  u8          m_CursorHighlightBG;
  Color       m_Comment;
  Color       m_Macro;
  Color       m_Special;
  Color       m_Keyword;
  Color       m_Primitive;
  Color       m_Type;
  Color       m_Emphasis;
  Color       m_String;
  Color       m_Number;
  
  // language mode options
  struct
  {
    EString*  m_Keywords;
    usize     m_NKeywords;
    EString*  m_Primitives;
    usize     m_NPrimitives;
  }           m_Lang[LANG_MODE_END];
};

constexpr NamedColor  NAMED_COLORS[]  =
{
  // terminal
  {"Terminal::DarkBG",          0},
  {"Terminal::DarkRed",         1},
  {"Terminal::DarkGreen",       2},
  {"Terminal::DarkYellow",      3},
  {"Terminal::DarkBlue",        4},
  {"Terminal::DarkPurple",      5},
  {"Terminal::DarkAqua",        6},
  {"Terminal::DarkFG",          7},
  {"Terminal::LightBG",         8},
  {"Terminal::LightRed",        9},
  {"Terminal::LightGreen",      10},
  {"Terminal::LightYellow",     11},
  {"Terminal::LightBlue",       12},
  {"Terminal::LightPurple",     13},
  {"Terminal::LightAqua",       14},
  {"Terminal::LightFG",         15},
  
  // gruvbox dark
  {"GruvboxDark::BG",           235},
  {"GruvboxDark::DarkRed",      124},
  {"GruvboxDark::DarkGreen",    106},
  {"GruvboxDark::DarkYellow",   172},
  {"GruvboxDark::DarkBlue",     66},
  {"GruvboxDark::DarkPurple",   132},
  {"GruvboxDark::DarkAqua",     72},
  {"GruvboxDark::LightGray",    246},
  {"GruvboxDark::DarkGray",     245},
  {"GruvboxDark::LightRed",     167},
  {"GruvboxDark::LightGreen",   142},
  {"GruvboxDark::LightYellow",  214},
  {"GruvboxDark::LightBlue",    109},
  {"GruvboxDark::LightPurple",  175},
  {"GruvboxDark::LightAqua",    108},
  {"GruvboxDark::FG",           223},
  {"GruvboxDark::BG0_H",        234},
  {"GruvboxDark::BG1",          237},
  {"GruvboxDark::BG2",          239},
  {"GruvboxDark::BG3",          241},
  {"GruvboxDark::BG4",          243},
  {"GruvboxDark::DarkOrange",   166},
  {"GruvboxDark::BG0_S",        236},
  {"GruvboxDark::FG4",          246},
  {"GruvboxDark::FG3",          248},
  {"GruvboxDark::FG2",          250},
  {"GruvboxDark::FG1",          223},
  {"GruvboxDark::FG0",          229},
  {"GruvboxDark::LightOrange",  208},
  
  // github dark
  {"GithubDark::Base0",         233},
  {"GithubDark::Base1",         235},
  {"GithubDark::Base2",         237},
  {"GithubDark::Base3",         243},
  {"GithubDark::Base4",         249},
  {"GithubDark::Base5",         252},
  {"GithubDark::LightRed",      210},
  {"GithubDark::DarkRed",       203},
  {"GithubDark::DarkYellow",    178},
  {"GithubDark::LightYellow",   221},
  {"GithubDark::LightBlue",     153},
  {"GithubDark::DarkBlue",      75},
  {"GithubDark::LightPurple",   183},
  {"GithubDark::DarkPurple",    135},
  {"GithubDark::LightGreen",    114},
  {"GithubDark::DarkGreen",     76},
  
  // vs dark
  {"VSDark::BG",                233},
  {"VSDark::FG0",               188},
  {"VSDark::FG1",               108},
  {"VSDark::Beige",             180},
  {"VSDark::Aqua0",             50},
  {"VSDark::Aqua1",             108},
  {"VSDark::Purple",            169},
  {"VSDark::Green",             64},
  {"VSDark::Blue0",             152},
  {"VSDark::Blue1",             30},
  {"VSDark::Blue2",             39},
  {"VSDark::Yellow",            230},
  
  // ridiculous light
  {"RidiculousLight::BGLight",  254},
  {"RidiculousLight::BGDark",   251},
  {"RidiculousLight::BGNight",  249},
  {"RidiculousLight::FGLight",  237},
  {"RidiculousLight::FGDark",   233},
  {"RidiculousLight::Blue",     21},
  {"RidiculousLight::Red",      124},
  {"RidiculousLight::Orange",   130},
  {"RidiculousLight::Aqua",     65},
  {"RidiculousLight::Purple",   126}
};

extern DynamicOptions g_Options;

i32 ParseOptions();
i32 ValidateOptions();
