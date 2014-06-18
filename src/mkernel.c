// mkernel.c
// Media Center Kernel
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "mkernel.h"
#include "gplayer.h"

#include <stdio.h>

#include <gst/gst.h>

#define PL_FILE "playlist"

typedef struct {
  gplayer* player;
  queue* plist;
} user_data;

void toggle(gplayer* player) {
  if (player->state == GST_STATE_PLAYING) {
    gplayer_pause(player);
  }
  else {
    gplayer_play(player);
  }
}

void next_song(user_data* data) {
  gplayer_stop(data->player);
  d_string* next = (d_string*)queue_pop(data->plist);
  if (next) {
    gplayer_set_uri(data->player, next->str);
    gplayer_play(data->player);
  }
  else {
    fprintf(stderr, "End of playlist\n");
  }
}

void start_mc(void (*get_input)(d_string*), gplayer* player) {
  queue* plist = load_file(PL_FILE);
  if (!plist) {
    fprintf(stderr, "ERROR: Failed to load playlist\n");
    exit(2);
  }

  user_data ud;
  ud.player = player;
  ud.plist = plist;
  player->onfinish = (CALLBACK_FUNC)next_song;
  player->cbdata = &ud;

  
  gplayer_set_uri(player, ((d_string*)queue_pop(plist))->str);

  d_string* cmd = dstr_alloc();
  int cont = 1;
  while (cont) {
    dstr_clear(cmd);
    get_input(cmd);
    if (cmd->size == 0) {
      cont = 0;
    }
    else {
      double v = 0;
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
        case 'n':
          next_song(&ud);
          break;
        case ',':
          v = gplayer_get_volume(player) - 0.001;
          if (v < 0.0) {
            v = 0.0;
          }
          gplayer_set_volume(player, v);
          break;
        case '.':
          v = gplayer_get_volume(player) + 0.001;
          if (v > 10.0) {
            v = 10.0;
          }
          gplayer_set_volume(player, v);
          break;
        default:
          fprintf(stderr, "Invalid Command : %s", cmd->str);
      }
    }
  }

  gplayer_stop(player);
  dstr_free(cmd);
}

