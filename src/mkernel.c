// mkernel.c
// Media Center Kernel
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "mkernel.h"
#include "gplayer.h"

#include <stdio.h>

#include <gst/gst.h>

void toggle(gplayer* player) {
  if (player->state == GST_STATE_PLAYING) {
    gplayer_pause(player);
  }
  else {
    gplayer_play(player);
  }
}

void start_mc(void (*get_input)(d_string*), gplayer* player) {
  gplayer_set_uri(player, "http://uk1.internet-radio.com:15476/");

  d_string* cmd = dstr_alloc();
  int cont = 1;
  while (cont) {
    dstr_clear(cmd);
    get_input(cmd);
    if (cmd->size == 0) {
      cont = 0;
    }
    else {
      switch (cmd->str[0]) {
        case 's':
          gplayer_play(player);
          break;
        case 'p':
          gplayer_pause(player);
          break;
        case 'q':
          cont = 0;
          break;
        case 't':
          toggle(player);
          break;
        default:
          fprintf(stderr, "Invalid Command : %s", cmd->str);
      }
    }
  }

  gplayer_stop(player);
  dstr_free(cmd);
}

