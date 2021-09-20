#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

#define CTRL_KEY(k) ((k) & 0x1f)

/* data */
struct termios orig_termios;

void disableRawMode();
void enableRawMode();
void die(const char *s);
char editorReadKey();
void editorProcessKeyPress();
void editorRefreshScreen();
