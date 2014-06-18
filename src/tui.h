// tui.h
// Text User Interface
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#ifndef TUI_H_
#define TUI_H_

#include "utils.h"

/**
 * This function should be called before using tui.
 */
void tui_init();

/**
 * Result will be appended to input.
 */
void tui_get_input(d_string* input);

#endif  // TUI_H_

