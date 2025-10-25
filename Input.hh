// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>
#include <Util.hh>

void  Unbind();
i32   Bind(const EChar* bind, void (*function)());
void  OrganizeInputs();
EChar ReadRawKey();
EChar ReadKey();
void  RecordMacro();
void  StopRecordingMacro();
bool  IsRecordingMacro();
void  ExecuteMacro();
