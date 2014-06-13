// main.c
// Main Program for HWP Media Center
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "utils.h"
#include "gplayer.h"

#include <stdio.h>

int main(int argc, char** argv) {
/*
  d_string* response = url_request(argv[1]);
  printf("%s\n", response->str);
*/
  gplayer_init();

  gplayer* p = gplayer_alloc();
  gplayer_set_uri(p, "http://uk1.internet-radio.com:15476/");
  gplayer_play(p);

  getchar();
  gplayer_free(p);

  return 0;
}

