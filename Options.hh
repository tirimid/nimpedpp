// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>
#include <vector>

enum LangMode
{
  LANG_MODE_C = 0,
  LANG_MODE_SH,
  LANG_MODE_JS,
  LANG_MODE_CC,
  
  LANG_MODE_END
};

struct InternalOptions
{
  usize       MAX_LOG_LENGTH      = 512;
  usize       CONFIG_KEY_LENGTH   = 128;
  usize       CONFIG_VALUE_LENGTH = 128;
  const char* CONFIG_SCAN         = "%127s = %127[^\r\n]";
};

struct FunctionalOptions
{
  usize       MAX_FILES         = 8;
  const char* CONFIG_DIR        = ".config/nimped++";
  const char* LAYOUT_CONFIG     = "layout.conf";
  const char* COLOR_CONF        = "color.conf";
  const char* LANG_CONF         = "lang.conf";
  const char* EDITING_CONF      = "editing.conf";
  usize       MAX_BAR_LENGTH    = 512;
  usize       MAX_PROMPT_LENGTH = 512;
};

struct FrameOptions
{
  const char* GREETER_TEXT  = "nimped++\n";
  const char* HELP_TEXT     = "nimped++ help text\n";
};

struct VisualOptions
{
  char        MARGIN_CHAR   = '|';
  const char* SCRATCH_NAME  = "-scratch-";
  const char* BASE_NAME     = "-base-";
  const char* WRITE_NAME    = "-write-";
};

struct KeybindOptions
{
};

struct Color
{
  u8  m_FG;
  u8  m_BG;
};

struct DynamicOptions
{
  // layout options
  u32               m_MasterNumer;
  u32               m_MasterDenom;
  u32               m_LeftGutter;
  u32               m_RightGutter;
  std::vector<u32>  m_Margins;
  u32               m_TabSize;
  
  // editor color options
  Color m_Global;
  Color m_Window;
  Color m_CurrentWindow;
  Color m_Normal;
  Color m_LineNumber;
  Color m_LineNumberHighlight;
  Color m_Margin;
  Color m_Cursor;
  Color m_GlobalCursor;
  
  // highlight color options
  Color m_Comment;
  Color m_Macro;
  Color m_Special;
  Color m_Keyword;
  Color m_Primitive;
  Color m_Type;
  Color m_Emphasis;
  Color m_String;
  Color m_Number;
  
  // editing options
  bool  m_TabSpaces;
  
  // language mode options
  struct
  {
    std::vector<EString>  m_Keywords;
    std::vector<EString>  m_Primitives;
  } m_Lang[LANG_MODE_END];
};

constexpr InternalOptions   INTERNAL;
constexpr FunctionalOptions FUNCTIONAL;
constexpr FrameOptions      FRAME;
constexpr VisualOptions     VISUAL;
constexpr KeybindOptions    KEYBIND;

extern DynamicOptions g_Options;

i32 ParseOptions();
