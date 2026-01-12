// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Encoding.hh>

void            GatherCompletions();
const EString*  CompleteWord(const EString& word);
void            RenderCompletion(usize frame, const EString& word);
