// main.c
// Main Program for HWP Media Center
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "mkernel.h"
#include "tui.h"
#include "gplayer.h"

int main(int argc, char** argv) {
  gplayer_init();
  gplayer* p = gplayer_alloc();

  tui_init();

  start_mc(tui_get_input, p);

  gplayer_free(p);
  return 0;
}

