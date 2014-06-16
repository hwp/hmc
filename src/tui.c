// tui.c
// Text User Interface
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#define INPUT_BUF_SIZE 200

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

// Warning: Concurrent use not permitted
void tui_get_input(d_string* input) {
  // Set term attributes
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	struct termios pterm = term;
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	
  char line[INPUT_BUF_SIZE + 1];
  int cont = 0;
  
  do {
    cont = 0;

    int cmd = getchar();
    if (cmd == EOF) {
      fprintf(stderr, "EOF\n");
      break;
    }

    switch (cmd) {
      case ' ':
        cmd = 't';
      case 's':
      case 'p':
      case 't':
      case 'q':
        dstr_app(input, cmd);
        break;
      case 'f':
        printf("Search song: ");
        term.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        fgets(line, INPUT_BUF_SIZE, stdin);
        dstr_app(input, cmd);
        dstr_app(input, ' ');
        dstr_cat(input, line);
        break;
      case 'c':
        printf("Search channel: ");
        term.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        fgets(line, INPUT_BUF_SIZE, stdin);
        dstr_app(input, cmd);
        dstr_app(input, ' ');
        dstr_cat(input, line);
        break;
      default:
        cont = 1;
    }
  } while (cont);

  // Restore previous attributes
	tcsetattr(STDIN_FILENO, TCSANOW, &pterm);
}

