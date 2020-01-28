#include <iostream>
#include <thread>
#include <chrono>

const char HEAD = 'O';
const char BODY = 'o';
const char EMPTY = ' ';
const char FOOD = '*';
const char WALL = '#';
const int TIME = 256;
const int HEIGHT = 16;
const int WIDTH = 16;
// Höhe * Breite muss eine Primzahlpotenz sein
const int PRIME = 911;
int field[HEIGHT][WIDTH];
int spawn;
enum Dir {NORTH, EAST, SOUTH, WEST};
Dir dir;
int curRow;
int curCol;

void quit() {
    system("stty cooked");
    system("stty echo");
    exit(0);
}

void keyboard() {
    char key;
    while (true) {
        key = std::cin.get();
        if (key == 'w') {
            dir = NORTH;
        }
        if (key == 'a') {
            dir = WEST;
        }
        if (key == 's') {
            dir = SOUTH;
        }
        if (key == 'd') {
            dir = EAST;
        }
        if (key == 'q') {
            quit();
        }
    }
}

void draw() {
    for (int col = 0; col < WIDTH + 2; col++) {
        std::cout << WALL;
    }
    std::cout << std::endl;
    for (int row = 0; row < HEIGHT; row++) {
        std::cout << WALL;
        for (int col = 0; col < WIDTH; col++) {
            int cell = field[row][col];
            char c;
            if (cell == 0) {
                c = EMPTY;
            }
            if (cell == -1) {
                c = FOOD;
            }
            if (cell > 0) {
                c = curRow == row && curCol == col ? HEAD : BODY;
            }
            std::cout << c;
        }
        std::cout << WALL << std::endl;
    }
    for (int col = 0; col < WIDTH + 2; col++) {
        std::cout << WALL;
    }
    std::cout << std::endl;
}

void spawnFood() {
    int row;
    int col;
    do {
        spawn += PRIME;
        spawn %= HEIGHT * WIDTH;
        row = spawn / WIDTH;
        col = spawn % WIDTH;
    } while (field[row][col] != 0);
    field[row][col] = -1;
}

void init() {
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            field[row][col] = 0;
        }
    }
    spawn = HEIGHT / 2 * WIDTH + WIDTH / 2;
    curRow = spawn / WIDTH;
    curCol = spawn % WIDTH;
    field[curRow][curCol] = 1;
    dir = NORTH;
    spawnFood();
}

void win() {
    system("clear");
    draw();
    std::cout << "~~~~~~~~:-)" << std::endl;
    quit();
}

void lose() {
    std::cout << "GAME OVER" << std::endl;
    quit();
}

void loop() {
    system("stty cooked");
    
    int nextRow = curRow;
    int nextCol = curCol;
    switch (dir) {
        case NORTH: nextRow--; break;
        case EAST: nextCol++; break;
        case SOUTH: nextRow++; break;
        case WEST: nextCol--; break;
    }
    if (nextRow < 0 || nextRow >= HEIGHT || nextCol < 0 || nextCol >= WIDTH || field[nextRow][nextCol] > 0) {
        lose();
    } else {
        if (field[nextRow][nextCol] == -1) {
            field[nextRow][nextCol] = field[curRow][curCol] + 2;
            bool won = true;
            for (int row = 0; row < HEIGHT && won; row++) {
                for (int col = 0; col < WIDTH && won; col++) {
                    if (field[row][col] == 0) {
                        won = false;
                    }
                }
            }
            if (won) {
                win();
            }
            spawnFood();
        } else {
            field[nextRow][nextCol] = field[curRow][curCol] + 1;
        }
        curRow = nextRow;
        curCol = nextCol;
        for (int row = 0; row < HEIGHT; row++) {
            for (int col = 0; col < WIDTH; col++) {
                if (field[row][col] > 0) {
                    field[row][col]--;
                }
            }
        }
    }

    system("clear");
    draw();
    system("stty raw");
}

int main() {
    system("stty -echo");
    std::thread(keyboard).detach();
    init();
    while (true) {
        loop();
        std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    }
}
