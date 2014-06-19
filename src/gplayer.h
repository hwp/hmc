// gplayer.h
// An implementation of playter using Gstreamer
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#ifndef GPLAYER_H_
#define GPLAYER_H_

#include <gst/gst.h>

typedef enum {
  FINISH_EOS,
  FINISH_ERROR
} finish_t;

typedef void (*CALLBACK_FUNC)(void*, finish_t);

typedef struct {
  GstElement* pipeline;
  GstState state;

  /**
   * The onfinish will be invoked when end-of-stream reached or error occurs.
   * The first argument is the cbdata.
   * The second argument is the type of finish: FINISH_EOS or FINISH_ERROR.
   */
  CALLBACK_FUNC onfinish;

  /**
   * Callback data.
   */
  void* cbdata;
} gplayer;

void gplayer_init(void);

/**
 * Get an instance of gplayer.
 */
gplayer* gplayer_alloc(void);

/**
 * Free a gplayer.
 */
void gplayer_free(gplayer* gp);

/**
 * Start to play audio from uri.
 */
void gplayer_start(gplayer* gp, const char* uri);

/**
 * Set volume.
 * vol should be a real number between 0.0 and 10.0.
 * No effect if player not started.
 */
void gplayer_set_volume(gplayer* gp, double vol);

/**
 * Get the current volume.
 * @return volume, 0.0 if player not started.
 */
double gplayer_get_volume(gplayer* gp);

/**
 * Pause.
 * Set status to GST_STATE_PAUSED.
 * No effect if player not started or paused.
 */
void gplayer_pause(gplayer* gp);

/**
 * Unpause.
 * Set status to GST_STATE_PLAYING.
 * No effect if player not started or playing.
 */
void gplayer_unpause(gplayer* gp);

/**
 * Unpause.
 * Set status to GST_STATE_NULL and unref pipeline.
 * No effect if player not started.
 */
void gplayer_stop(gplayer* gp);

#endif  // GPLAYER_H_

