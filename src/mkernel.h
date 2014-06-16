// mkernel.h
// Media Center Kernel
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#ifndef MKERNEL_H_
#define MKERNEL_H_

#include "utils.h"
#include "gplayer.h"

/**
 * Start media center
 */
void start_mc(void (*get_input)(d_string*), gplayer* player);

#endif  // MKERNEL_H_

