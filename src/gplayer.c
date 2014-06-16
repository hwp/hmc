// gplayer.c
// An implementation of playter using Gstreamer
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "gplayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void gplayer_init(void) {
  int argc = 0;
  char** argv = NULL;
  gst_init(&argc, &argv);
}

gplayer* gplayer_alloc(void) {
  gplayer* p = malloc(sizeof(gplayer));
  p->pipeline = NULL;
  p->state = GST_STATE_NULL;
  p->uri = NULL;
  p->onfinish = NULL;
  p->cbdata = NULL;

  return p;
}

void gplayer_free(gplayer* gp) {
  if (gp->pipeline) { 
    gst_element_set_state (gp->pipeline, GST_STATE_NULL);
    gst_object_unref(gp->pipeline);
  }
  if (gp->uri) {
    free(gp->uri);
  }
  free(gp);
}

void gplayer_set_uri(gplayer* gp, const char* uri) {
  if (gp->uri) {
    free(gp->uri);
  }
  gp->uri = strdup(uri);
}

void gplayer_play(gplayer* gp) {
  if (!gp->pipeline) {
    gp->pipeline = gst_element_factory_make("playbin", "playbin");
    if (!gp->pipeline) {
      fprintf(stderr, "Failed to create playbin\n");
      exit(2);
    }
    g_object_set(gp->pipeline, "uri", gp->uri, NULL);
  }

  gst_element_set_state(gp->pipeline, GST_STATE_PLAYING);
  gp->state = GST_STATE_PLAYING;
}

void gplayer_pause(gplayer* gp) {
  if (gp->pipeline) {
    gst_element_set_state (gp->pipeline, GST_STATE_PAUSED);
    gp->state = GST_STATE_PAUSED;
  }
}

void gplayer_stop(gplayer* gp) {
  if (gp->pipeline) {
    gst_element_set_state (gp->pipeline, GST_STATE_NULL);
    gp->state = GST_STATE_NULL;
  }
}

