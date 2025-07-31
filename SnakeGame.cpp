#include <iostream>
#include <conio.h>    // For _kbhit() and _getch()
#include <windows.h>  // For Sleep() and console functions
#include <cstdlib>    // For rand()
using namespace std;

// Node definition for snake body linked list
struct Node {
    int x, y;
    Node* next;
    Node(int x, int y) : x(x), y(y), next(nullptr) {}
};

int width = 20, height = 10;
int foodX, foodY;
int score;
bool gameOver;
char dir;  // Direction 'w','a','s','d'

Node* snakeHead = nullptr;

void gotoXY(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void placeFood() {
    bool onSnake;
    do {
        onSnake = false;
        foodX = rand() % width;
        foodY = rand() % height;

        Node* curr = snakeHead;
        while (curr) {
            if (curr->x == foodX && curr->y == foodY) {
                onSnake = true;
                break;
            }
            curr = curr->next;
        }
    } while (onSnake);
}

void setup() {
    score = 0;
    gameOver = false;
    dir = 'd';

    // Delete old snake if any
    while (snakeHead) {
        Node* temp = snakeHead;
        snakeHead = snakeHead->next;
        delete temp;
    }

    snakeHead = new Node(width / 2, height / 2);
    placeFood();
    system("cls");
}

void draw() {
    gotoXY(0, 0);

    // Draw top border
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << '\n';

    for (int y = 0; y < height; y++) {
        cout << "#";
        for (int x = 0; x < width; x++) {
            if (x == foodX && y == foodY) {
                cout << "@";
            }
            else {
                bool printed = false;
                Node* curr = snakeHead;
                while (curr) {
                    if (curr->x == x && curr->y == y) {
                        cout << "O";
                        printed = true;
                        break;
                    }
                    curr = curr->next;
                }
                if (!printed) cout << " ";
            }
        }
        cout << "#\n";
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\nScore: " << score << endl;
}

void move() {
    int dx = 0, dy = 0;
    if (dir == 'w') dy = -1;
    else if (dir == 's') dy = 1;
    else if (dir == 'a') dx = -1;
    else if (dir == 'd') dx = 1;
    else return; // invalid

    int newX = snakeHead->x + dx;
    int newY = snakeHead->y + dy;

    if (newX < 0 || newX >= width || newY < 0 || newY >= height) {
        gameOver = true;
        return;
    }

    Node* curr = snakeHead;
    while (curr) {
        if (curr->x == newX && curr->y == newY) {
            gameOver = true;
            return;
        }
        curr = curr->next;
    }

    // Create new head
    Node* newHead = new Node(newX, newY);
    newHead->next = snakeHead;
    snakeHead = newHead;

    if (newX == foodX && newY == foodY) {
        score += 10;
        placeFood();
    } else {
        // Remove tail
        curr = snakeHead;
        while (curr->next && curr->next->next) {
            curr = curr->next;
        }
        delete curr->next;
        curr->next = nullptr;
    }
}

int main() {
    setup();
    draw();

    while (!gameOver) {
        Sleep(100);

        if (_kbhit()) {
            char c = _getch();
            if ((c == 'w' || c == 'W') && dir != 's') dir = 'w';
            else if ((c == 's' || c == 'S') && dir != 'w') dir = 's';
            else if ((c == 'a' || c == 'A') && dir != 'd') dir = 'a';
            else if ((c == 'd' || c == 'D') && dir != 'a') dir = 'd';
        }
        move();
        draw();
    }

    cout << "Game Over! Final Score: " << score << endl;

    // Cleanup memory
    while (snakeHead) {
        Node* temp = snakeHead;
        snakeHead = snakeHead->next;
        delete temp;
    }

    system("pause");
    return 0;
}
