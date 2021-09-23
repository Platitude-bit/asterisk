/* libs */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

/* macros */
#define CTRL_KEY(k) ((k) & 0x1f)    
#define ABUF_INIT {NULL, 0}         // constructor for the append buffer
#define ASTERIKS_VER "0.0.1"        // editor version 

/* data */
struct editorConfig {
    int curX, curY;
    int scrRows, scrCols;
    struct termios orig_termios;
};

struct appBuff {
    char *b;
    int len;
};

struct editorConfig editConf;   /* struct for terminal window size 
                                    and other terminal attributes */

/* function prototypes */
void disableRawMode();
void enableRawMode();
void die(const char *s);

void editorProcessKeyPress();
void editorRefreshScreen();
void editorDrawRows(struct appBuff *app);
void editorMoveCursor(char);
char editorReadKey();

void initEditor();

void buffAppend(struct appBuff *app, const char *s, int len);
void buffFree(struct appBuff *app);

int getWindowSize(int *, int *);
int getCursorPos(int *, int *);
