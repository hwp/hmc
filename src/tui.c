// tui.c
// Text User Interface
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#define INPUT_BUF_SIZE 200

#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

struct termios pterm;

// Restore previous attributes
void restore_attr() {
	tcsetattr(STDIN_FILENO, TCSANOW, &pterm);
}

void tui_init() {
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);

  pterm = term;
  atexit(restore_attr);
}

// Warning: Concurrent use not permitted
void tui_get_input(d_string* input) {
  // Set term attributes
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);

	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	
  char line[INPUT_BUF_SIZE + 1];
  int cont = 0;
  int cont2 = 0;
  char* end;
  
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
      case 't':
      case 'q':
      case 'n':
      case ',':
      case '.':
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
      case 'a':
        cmd = getchar();
        switch (cmd) {
          case 'c':
            cont2 = 1;
            while (cont2) {
              printf("Add CD track (#|all): ");
              term.c_lflag |= ECHO;
              tcsetattr(STDIN_FILENO, TCSANOW, &term);
              fgets(line, INPUT_BUF_SIZE, stdin);
              if (!strcmp(line, "all\n")) {
                dstr_cat(input, "aca");
                cont2 = 0;
              }
              else {
                strtol(line, &end, 10);
                if (*end == '\n') {
                  *end = '\0';
                  dstr_cat(input, "ac");
                  dstr_cat(input, line);
                  cont2 = 0;
                }
              }
            }
            break;
          case 'd':
            dstr_cat(input, "ad");
            break;
          default:
            cont = 1;
        }
        break;
      default:
        cont = 1;
    }
  } while (cont);

  tcsetattr(STDIN_FILENO, TCSANOW, &pterm);
}

