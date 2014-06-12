# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

cmake_minimum_required(VERSION 2.6)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(HMC)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_subdirectory(src)

