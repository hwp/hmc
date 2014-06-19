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
} kernel_data;

void next_track(kernel_data* data) {
  gplayer_stop(data->player);
  d_string* next = (d_string*)queue_pop(data->plist);
  if (next) {
    gplayer_start(data->player, next->str);
  }
  else {
    fprintf(stderr, "End of playlist\n");
  }
}

void onfinish(kernel_data* data, finish_t type) {
  if (type == FINISH_EOS) {
    next_track(data);
  }
  else {
    fprintf(stderr, "An error occured. Continue next track\n");
    next_track(data);
  }
}

void toggle(kernel_data* data) {
  switch (data->player->state) {
    case GST_STATE_PLAYING:
      gplayer_pause(data->player);
      break;
    case GST_STATE_READY:
    case GST_STATE_PAUSED:
      gplayer_unpause(data->player);
      break;
    default:
      next_track(data);
      break;
  }
}

void start_mc(void (*get_input)(d_string*), gplayer* player) {
  queue* plist = load_file(PL_FILE);
  if (!plist) {
    fprintf(stderr, "ERROR: Failed to load playlist\n");
    exit(2);
  }

  kernel_data kd;
  kd.player = player;
  kd.plist = plist;
  player->onfinish = (CALLBACK_FUNC)onfinish;
  player->cbdata = &kd;

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
        case 'q':
          cont = 0;
          break;
        case 't':
          toggle(&kd);
          break;
        case 'n':
          next_track(&kd);
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

