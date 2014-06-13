// gplayer.c
// An implementation of playter using Gstreamer
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "gplayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void gplayer_init() {
  int argc = 0;
  char** argv = NULL;
  gst_init(&argc, &argv);
}

gplayer* gplayer_alloc() {
  gplayer* p = malloc(sizeof(gplayer));
  p->pipeline = NULL;
  p->uri = NULL;
  p->status = 0;
  p->onfinish = NULL;
  p->cbdata = NULL;

  return p;
}

void gplayer_free(gplayer* gp) {
  if (gp->pipeline != NULL) { 
    gst_element_set_state (gp->pipeline, GST_STATE_NULL);
    gst_object_unref(gp->pipeline);
  }
  if (gp->uri != NULL) {
    free(gp->uri);
  }
  free(gp);
}

void gplayer_set_uri(gplayer* gp, const char* uri) {
  if (gp->uri != NULL) {
    free(gp->uri);
  }
  gp->uri = strdup(uri);
}

void gplayer_play(gplayer* gp) {
  if (gp->pipeline == NULL) {
    char* script = malloc(sizeof(char) * (20 + strlen(gp->uri)));
    sprintf(script, "playbin uri=%s", gp->uri);
    GError* error = NULL;
    gp->pipeline = gst_parse_launch(script, &error);
    if (!gp->pipeline) {
      g_print ("Parse error: %s\n", error->message);
      exit (1);
    }
    free(script);
  }

  gst_element_set_state(gp->pipeline, GST_STATE_PLAYING);
}

