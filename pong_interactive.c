#include <ncurses.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#define field_x 80
#define field_y 25
#define racket1_x 2
#define racket2_x 77
#define WIN_CONDITION 21

void graphField(int graph_x, int graph_y, int racket1_y, int racket2_y);
int graphPoint(float x, float y);
int checkBorderlineY(int graph_y);
int checkRacket(int graph_x, int graph_y, int racket_y);
int readButton(int racket1_y, int racket2_y);
int intCode(int a, int b);
int move_racket1_up(int racket1_y);
int move_racket1_down(int racket1_y);
int move_racket2_up(int racket2_y);
int move_racket2_down(int racket2_y);
void graphScore(int pl1, int pl2);
void graphWin(int player_1);

int main() {
    initscr();
    noecho();
    cbreak();
    float x = field_x / 2;
    float y = field_y / 2 - 6;
    float kx = 0.1;
    float ky = 0.1;
    int graph_x = x;
    int graph_y = y;
    int player_1 = 0;
    int player_2 = 0;
    int racket1_y = 12;
    int racket2_y = 12;
    char cmd;
   // graphScore(player_1, player_2);
    graphField(graph_x, graph_y, racket1_y, racket2_y);
    while (1) {
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);

        x += kx;
        y += ky;
        usleep(5000);
        int coded_coordinates = graphPoint(x, y);
        graph_x = coded_coordinates / 100;
        graph_y = coded_coordinates % 100;
        if (checkBorderlineY(graph_y)) ky *= -1;
        if (graph_x == 0) {
            player_2++;
            x = 39;
        }
        if (graph_x == 79) {
            player_1++;
            x = 39;
        }
        //if (scanf("%c", &cmd) != 1) continue;
        //if (cmd == ' ') continue;
        int racket12_y_coded = readButton(racket1_y, racket2_y);
        racket1_y = racket12_y_coded / 100;
        racket2_y = racket12_y_coded % 100;
        if (checkRacket(graph_x, graph_y, racket1_y) && graph_x < 9) {
            kx *= -1;
        }
        if (checkRacket(graph_x, graph_y, racket2_y) && graph_x > 70) {
            kx *= -1;
        }
        printf("\033[0d\033[2J");
        graphScore(player_1, player_2);
        graphField(graph_x, graph_y, racket1_y, racket2_y);
        if (player_1 == WIN_CONDITION || player_2 == WIN_CONDITION) {
           graphWin(player_1);
           break;
       }
    }
    endwin();
    return 0;
}

int graphPoint(float x, float y) {
    int res = 0;
    float dx = x - floor(x);
    float dy = y - floor(y);
    if (x > -1 && x < 80 && y > -1 && y < 25) {
        res = intCode(ceil(x), ceil(y));
    } else if (x < 0 && y > 24)
        res = intCode(ceil(x), floor(y));
    else if (x < 0 && y < 0)
        res = intCode(ceil(x), ceil(y));
    else if (x > 79 && y > 24)
        res = intCode(floor(x), floor(y));
    else if (x > 79 && y < 0)
        res = intCode(floor(x), floor(y));
    else if (x < 0) {
        if (dy > 0.5)
            res = intCode(ceil(x), ceil(y));
        else
            res = intCode(ceil(x), floor(y));
    } else if (x > 79) {
        if (dy > 0.5)
            res = intCode(floor(x), floor(y));
        else
            res = intCode(floor(x), floor(y));
    } else if (y < 0) {
        if (dx > 0.5)
            res = intCode(ceil(x), ceil(y));
        else
            res = intCode(floor(x), ceil(y));
    } else if (y > 24) {
        if (dx > 0.5)
            res = intCode(floor(x), ceil(y));
        else
            res = intCode(floor(x), floor(y));
    }
    return res;
}

void graphField(int graph_x, int graph_y, int racket1_y, int racket2_y) {
    char ball = '@';
    for (int i = 24; i >= 0; i--) {
        for (int j = 0; j < field_x; j++) {
            if (i == graph_y && j == graph_x) {
                mvprintw(i, j,"%c", ball);
            } else if (i == 0 || i == field_y - 1) {
                mvprintw(i, j, "-");
            } else if (j == 0 || j == field_x - 1 || j == field_x / 2) {
                mvprintw(i, j, "|");
            } else if ((j == 2) && (i == racket1_y || i == racket1_y - 1 || i == racket1_y + 1)) {
                mvprintw(i, j, "]");
            } else if ((j == 77) && (i == racket2_y || i == racket2_y - 1 || i == racket2_y + 1)) {
                mvprintw(i, j, "[");
            } else {
                mvprintw(i, j, " ");
            }
        }
       // printf("\n");
    }
}

int checkBorderlineY(int graph_y) {
    if (graph_y == 24 || graph_y == 0) {
        return 1;
    } else
        return 0;
}

int checkRacket(int graph_x, int graph_y, int racket_y) {
    if (((graph_x == 2) || (graph_x == 77)) &&
        ((graph_y == racket_y) || (graph_y == racket_y + 1) || (graph_y == racket_y - 1)))
        return 1;
    else
        return 0;
}

int readButton(int racket1_y, int racket2_y) {
    int result;
    int cmd = getch();
    if (cmd == 'z')
        racket1_y = move_racket1_up(racket1_y);
    else if (cmd == 'a') {
        racket1_y = move_racket1_down(racket1_y);
    } else if (cmd == 'm') {
        racket2_y = move_racket2_up(racket2_y);
    } else if (cmd == 'k') {
        racket2_y = move_racket2_down(racket2_y);
    }
    result = intCode(racket1_y, racket2_y);
    return result;
}

int intCode(int a, int b) {
    int res = a * 100 + b;
    return res;
}

int move_racket1_up(int racket1_y) {
    if (fabs((double)racket1_y - 24) >= 4)
        racket1_y += 2;
    else if (fabs((double)racket1_y - 24) == 3)
        racket1_y += 1;
    return racket1_y;
}

int move_racket1_down(int racket1_y) {
    if (fabs((double)racket1_y) >= 4)
        racket1_y -= 2;
    else if (fabs((double)racket1_y) == 3)
        racket1_y -= 1;
    return racket1_y;
}

int move_racket2_up(int racket2_y) {
    if (fabs((double)racket2_y - 24) >= 4)
        racket2_y += 2;
    else if (fabs((double)racket2_y - 24) == 3)
        racket2_y += 1;
    return racket2_y;
}

int move_racket2_down(int racket2_y) {
    if (fabs((double)racket2_y) >= 4)
        racket2_y -= 2;
    else if (fabs((double)racket2_y - 24) == 3)
        racket2_y -= 1;
    return racket2_y;
}

void graphScore(int pl1, int pl2) {
    for (int i = 0; i <= 6; i++) {
        for (int j = 0; j < field_x; j++) {
            if (i == 0) {
                printf("-");
            } else if ((j == 0 || j == field_x / 2 || j == field_x - 1) && (i != 3 && i != 4)) {
                printf("|");
            } else if ((i == 3 && (j == field_x / 2 - 8 || j == 0 || j == field_x - 17))) {
                printf("|");
            } else if (i == 3 && j == 16) {
                printf("Player 1:");
            } else if (i == 3 && j == 47) {
                printf("Player 2:");
            } else if ((i == 4 && (j == field_x / 2 - 1 || j == 0 || j == field_x - 3))) {
                printf("|");
            } else if (i == 4 && j == 19) {
                printf("%02d", pl1);
            } else if (i == 4 && j == 58) {
                printf("%02d", pl2);
            } else
                printf(" ");
        }
        printf("\n");
    }
}

void graphWin(int player_1) {
    if (player_1 == WIN_CONDITION) {
        printf("  ____    _                                   _                _                \n");
        printf(" |  _ \\  | |   __ _   _   _    ___   _ __    / |   __      __ (_)  _ __    ___  \n");
        printf(" | |_) | | |  / _` | | | | |  / _ \\ | '__|   | |   \\ \\ /\\ / / | | | '_ \\  / __| \n");
        printf(" |  __/  | | | (_| | | |_| | |  __/ | |      | |    \\ V  V /  | | | | | | \\__ \\ \n");
        printf(" |_|     |_|  \\__,_|  \\__, |  \\___| |_|      |_|     \\_/\\_/   |_| |_| |_| |___/ \n");
        printf("                      |___/                                                     \n");
    } else {
        printf("  ____    _                                   ____                 _                \n");
        printf(" |  _ \\  | |   __ _   _   _    ___   _ __    |___ \\    __      __ (_)  _ __    ___  \n");
        printf(" | |_) | | |  / _` | | | | |  / _ \\ | '__|     __) |   \\ \\ /\\ / / | | | '_ \\  / __| \n");
        printf(" |  __/  | | | (_| | | |_| | |  __/ | |       / __/     \\ V  V /  | | | | | | \\__ \\ \n");
        printf(" |_|     |_|  \\__,_|  \\__, |  \\___| |_|      |_____|     \\_/\\_/   |_| |_| |_| |___/ \n");
        printf("                      |___/                                                         \n");
    }
}
