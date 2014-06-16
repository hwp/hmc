# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(HMC)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(GLIB REQUIRED COMPONENTS gobject)
include_directories(${GLIB_INCLUDE_DIRS})

find_package(GStreamer REQUIRED)
include_directories(${GSTREAMER_INCLUDE_DIRS})

find_package(Threads)

set(CMAKE_C_FLAGS "-Wall")

add_subdirectory(src)

