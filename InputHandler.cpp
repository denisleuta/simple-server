#include "InputHandler.h"


#ifdef __linux__
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

char InputHandler::GetInput() {
    struct termios oldt, newt;
    char ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        return ch;
    }

    return 0;
}
#elif _WIN32
#include <conio.h>

char InputHandler::GetInput() {
    if (_kbhit()) {
        return _getch();
    }
    return 0;
}
#else
#error "Unsupported operating system"
#endif