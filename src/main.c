// main.c
// Main Program for HWP Media Center
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "utils.h"
#include "gplayer.h"

#include <stdio.h>

int main(int argc, char** argv) {
  d_string* response = url_request(argv[1]);
  printf("%s\n", response->str);
  dstr_free(response);

  gplayer_init();

  gplayer* p = gplayer_alloc();
  gplayer_set_uri(p, "http://uk1.internet-radio.com:15476/");

  d_string* cmd = dstr_alloc();
  int cont = 1;
  while (cont) {
    dstr_clear(cmd);
    tui_get_input(cmd);
    if (cmd->size == 0) {
      cont = 0;
    }
    else {
      switch (cmd->str[0]) {
        case 's':
          gplayer_play(p);
          break;
        case 'p':
          gplayer_pause(p);
          break;
        case 'q':
          cont = 0;
          break;
        default:
          fprintf(stderr, "Invalid Command : %s", cmd->str);
      }
    }
  }

  dstr_free(cmd);
  gplayer_free(p);

  return 0;
}

