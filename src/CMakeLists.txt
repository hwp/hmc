# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

add_executable(hmc main.c utils.c gplayer.c mkernel.c tui.c)
target_link_libraries(hmc ${CURL_LIBRARIES} ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES} ${GLIB_GOBJECT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CDIO_LIBRARIES})
#target_link_libraries(hmc ${GSTREAMER_LIBRARIES} ${GLIB_LIBRARIES})

