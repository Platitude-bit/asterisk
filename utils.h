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
#define ABUF_INIT {NULL, 0}
#define ASTERIKS_VER "0.0.1"

/* data */
struct editorConfig {
    int scrRows;
    int scrCols;
    struct termios orig_termios;
};

struct appBuff {
    char *b;
    int len;
};

struct editorConfig editConf;

/* function prototypes */
void disableRawMode();
void enableRawMode();
void die(const char *s);
void editorProcessKeyPress();
void editorRefreshScreen();
void editorDrawRows(struct appBuff *app);
void initEditor();
void buffAppend(struct appBuff *app, const char *s, int len);
void buffFree(struct appBuff *app);
char editorReadKey();
int getWindowSize(int *, int *);
int getCursorPos(int *, int *);
