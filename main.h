#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* data */
struct termios orig_termios;

/* terminal methods */
void disableRawMode();
void enableRawMode();
void die(const char *s);
void editorReadKey();
