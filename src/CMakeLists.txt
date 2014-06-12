# Author : Weipeng He <heweipeng@gmail.com>
# Copyright (c) 2014, All rights reserved.

add_executable(hmc main.c utils.c)
target_link_libraries(hmc ${CURL_LIBRARIES})

