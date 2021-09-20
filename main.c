#include "main.h"

int main() {
    enableRawMode();

    while(1) {
        editorRefreshScreen();
        editorProcessKeyPress();
    }
    return 0;
}

/* exit raw mode */
void disableRawMode() {
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/* enter raw mode */
void enableRawMode() {
    if(tcgetattr(STDIN_FILENO, &orig_termios) == - 1)
        die("tcgetattr");

    atexit(disableRawMode);

    struct termios raw = orig_termios;

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
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    editorDrawRows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}

/* draw rows of '+' */
void editorDrawRows() {
    for(int h=0;h<30;++h) {
        write(STDIN_FILENO, "+\r\n", 3);
    }
}
    






















