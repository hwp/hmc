// gplayer.c
// An implementation of playter using Gstreamer
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "gplayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

void* start_gmainloop(void* data) {
  GMainLoop* loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);

  return NULL;
}

void gplayer_init(void) {
  int argc = 0;
  char** argv = NULL;
  gst_init(&argc, &argv);

  pthread_t loop_thread;
  if (pthread_create(&loop_thread, NULL, start_gmainloop, NULL)) {
    fprintf(stderr, "Error creating thread (%s:%d)\n", __FILE__, __LINE__);
    exit(2);
  }
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
    gst_element_set_state(gp->pipeline, GST_STATE_NULL);
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

static gboolean message_callback(GstBus *bus,
    GstMessage *msg, gplayer* gp) {
  GError *err;
  gchar *debug_info;
  GstState old_state, new_state, pending_state;

  switch (GST_MESSAGE_TYPE(msg)) {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error(msg, &err, &debug_info);
      fprintf(stderr, "Error received from element %s: %s\n",
          GST_OBJECT_NAME(msg->src), err->message);
      fprintf(stderr, "Debugging information: %s\n",
          debug_info ? debug_info : "none");
      g_clear_error (&err);
      g_free (debug_info);
      exit(2);
      break;
    case GST_MESSAGE_EOS:
      fprintf(stderr, "End-Of-Stream reached.\n");
      if (gp->onfinish) {
        gp->onfinish(gp->cbdata);
      }
      break;
    case GST_MESSAGE_STATE_CHANGED:
      gst_message_parse_state_changed(msg, 
          &old_state, &new_state, &pending_state);
      gp->state = new_state;
      /*
      if (GST_MESSAGE_SRC(msg) == GST_OBJECT(gp->pipeline)) {
        fprintf(stderr, "State changed from %d to %d with %d pending\n",
            old_state, new_state, pending_state);
      }
      */
      break;
    default:
      /* Ignore */
      break;
  }

  /* We want to keep receiving messages */
  return TRUE;
}

void gplayer_play(gplayer* gp) {
  if (!gp->pipeline) {
    assert(gp->state == GST_STATE_NULL);
    gp->pipeline = gst_element_factory_make("playbin", "playbin");
    if (!gp->pipeline) {
      fprintf(stderr, "Failed to create playbin\n");
      exit(2);
    }
    g_object_set(gp->pipeline, "uri", gp->uri, NULL);

    GstBus* bus = gst_element_get_bus(gp->pipeline);
    int id = gst_bus_add_watch(bus, (GstBusFunc)message_callback, gp);
    fprintf(stderr, "Bus watch registered with id %d\n", id);
  }

  GstStateChangeReturn r =
    gst_element_set_state(gp->pipeline, GST_STATE_PLAYING);
  if (r == GST_STATE_CHANGE_FAILURE) {
    fprintf(stderr, "Failed to change state (%s:%d)\n", __FILE__, __LINE__);

    gst_object_unref(gp->pipeline);
    exit(2);
  }
}

void gplayer_pause(gplayer* gp) {
  if (gp->pipeline) {
    GstStateChangeReturn r =
      gst_element_set_state (gp->pipeline, GST_STATE_PAUSED);
    if (r == GST_STATE_CHANGE_FAILURE) {
      fprintf(stderr, "Failed to change state (%s:%d)\n", __FILE__, __LINE__);
      gst_object_unref(gp->pipeline);
      exit(2);
    }
  }
}

void gplayer_stop(gplayer* gp) {
  if (gp->pipeline) {
    GstStateChangeReturn r =
      gst_element_set_state (gp->pipeline, GST_STATE_NULL);
    if (r == GST_STATE_CHANGE_FAILURE) {
      fprintf(stderr, "ERROR: Failed to change state (%s:%d)\n", __FILE__, __LINE__);
      gst_object_unref(gp->pipeline);
      exit(2);
    }
    gst_object_unref(gp->pipeline);
    gp->pipeline = NULL;
    gp->state = GST_STATE_NULL;
  }
}

void gplayer_set_volume(gplayer* gp, double vol) {
  if (gp->pipeline) {
    g_object_set(gp->pipeline, "volume", vol, NULL);
  }
}

double gplayer_get_volume(gplayer* gp) {
  double vol = 0.0;
  if (gp->pipeline) {
    g_object_get(gp->pipeline, "volume", &vol, NULL);
  }
  return vol;
}

