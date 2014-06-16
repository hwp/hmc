// gplayer.h
// An implementation of playter using Gstreamer
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#ifndef GPLAYER_H_
#define GPLAYER_H_

#include <gst/gst.h>

typedef struct {
  GstElement* pipeline;
  GstState state;
  char* uri;
  void (*onfinish)(void*);
  void* cbdata;
} gplayer;

void gplayer_init(void);

/**
 * Get an instance of gplayer.
 */
gplayer* gplayer_alloc(void);
void gplayer_free(gplayer* gp);

void gplayer_set_uri(gplayer* gp, const char* uri);
void gplayer_set_volume(gplayer* gp, double vol);
double gplayer_get_volume(gplayer* gp);
void gplayter_set_cbdata(gplayer* gp, void*);
void gplayer_play(gplayer* gp);
void gplayer_pause(gplayer* gp);
void gplayer_stop(gplayer* gp);

#endif  // GPLAYER_H_

