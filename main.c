#include "main.h"

int main() {
    enableRawMode();
    initEditor();

    while(1) {
        editorRefreshScreen();
        editorProcessKeyPress();
    }
    return 0;
}

/* exit raw mode */
void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &editConf.orig_termios) == -1)
        die("tcsetattr");
}

/* enter raw mode */
void enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &editConf.orig_termios) == - 1)
        die("tcgetattr");

    atexit(disableRawMode);

    struct termios raw = editConf.orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); 
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;


    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/* print error message */
void die(const char *s) {
    editorRefreshScreen();
    perror(s);
    exit(1);
}

/* Keyboard input */
char editorReadKey() {
    int reader;
    char c;
    while((reader = read(STDIN_FILENO, &c, 1)) != 1) {
        if(reader == -1 && errno != EAGAIN)
            die("read");
    }
    return c;
}

void editorProcessKeyPress() {
    char c = editorReadKey();

    if(c == CTRL_KEY('q')) {
        editorRefreshScreen();
        exit(0);
    }
}

/* refresh the screen */
void editorRefreshScreen() {
    struct appBuff app = ABUF_INIT;


    buffAppend(&app, "\x1b[?25l", 6);
    buffAppend(&app, "\x1b[2J", 4);
    buffAppend(&app, "\x1b[H", 3);

    editorDrawRows(&app);
    buffAppend(&app, "\x1b[H", 3);
    buffAppend(&app, "\x1b[?25h", 6);

    write(STDOUT_FILENO, app.b, app.len);
    buffFree(&app);
}

/* draw rows of '-' */
void editorDrawRows(struct appBuff *app) {
    for(int h=0;h<editConf.scrRows;++h) {
        buffAppend(app, "~", 1);

        if(h < editConf.scrRows - 1) 
            buffAppend(app, "\r\n", 2);
    }
}

/* return position of the cursor */
int getCursorPos(int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;

    if(write(STDOUT_FILENO, "\x1b[6n", 4) != 4) 
        return -1;
    
    while(i < sizeof(buf) - 1) {
        if(read(STDIN_FILENO, &buf[i], 1) != 1) 
            break;
        if(buf[i] == 'R')
            break;
        ++i;
    }
    buf[i] = '\0';

    if(buf[0] != '\x1b' || buf[1] != '[')
        return -1;

    if(sscanf(&buf[2], "%d;%d", rows, cols) != 2)
        return -1;


    return -1;
}

/* return the size of the window */
int getWindowSize(int *rows, int *cols) {

    struct winsize ws;

    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) 
            return -1;

        return getCursorPos(rows, cols);

        editorReadKey();
        return -1;
    }

    else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

/* initialize all fields in struct editConf */
void initEditor() {
    if(getWindowSize(&editConf.scrRows, &editConf.scrCols) == -1) 
        die("getWindowSize");
}

void buffAppend(struct appBuff *app, const char *s, int len) {
    char *new = realloc(app->b, app->len + len);

    if(new == NULL) 
        return;

    memcpy(&new[app->len], s, len);
    app->b = new;
    app->len += len;
}

void buffFree(struct appBuff *app) {
    free(app->b);
}



