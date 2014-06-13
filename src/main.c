// main.c
// Main Program for HWP Media Center
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "utils.h"

#include <stdio.h>

int main(int argc, char** argv) {
  d_string* response = url_request(argv[1]);
  printf("%s\n", response->str);

  return 0;
}

