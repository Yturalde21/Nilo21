#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Define the constants, variables, and vectors
const int width = 40;
const int height = 20;
bool gameOver;
int spaceshipX, spaceshipY, score = 0;
bool bulletActive = false;
int bulletX, bulletY;
vector<vector<char>> playArea(height, vector<char>(width, ' '));
vector<pair<int, int>> asteroids;

// The setup function sets initial values
void Setup() {
    gameOver = false;
    spaceshipX = width / 2;
    spaceshipY = height - 1;
    srand(time(nullptr));
    for (int i = 0; i < 3; i++) {
        asteroids.push_back({rand() % width, rand() % (height - 10)});
    }
}

// The Draw function displays the game area
void Draw() {
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "";
    cout << endl;

    for (int i = 0; i < height; i++) {
        cout << "";
        for (int j = 0; j < width; j++) {
            if (i == spaceshipY && j == spaceshipX)
                cout << "_∆_";
            else if (bulletActive && i == bulletY && j == bulletX)
                cout << "|";
            else if (playArea[i][j] == '~') // Print the faded effect
                cout << ".";
            else {
                bool isAsteroid = false;
                for (const auto& asteroid : asteroids) {
                    if (i == asteroid.second && j == asteroid.first) {
                        cout << "◻️";
                        isAsteroid = true;
                    }
                }
                if (!isAsteroid)
                    cout << " ◾";
            }
        }
        cout << "                  ";
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "";
    cout << endl;
    cout << "Score:" << score << endl;
}


// The Input function handles user input
void Input() {
    static int fireCooldown = 0;
    static int fireDelay = 0; // Adjust this value for the firing delay

    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (spaceshipX > 0)
                spaceshipX--;
            break;
        case 'd':
            if (spaceshipX < width - 1)
                spaceshipX++;
            break;
        case 'w': // Move the spaceship forward
            if (spaceshipY > 0)
                spaceshipY--;
            break;
        case 's': // Move the spaceship down
            if (spaceshipY < height - 1)
                spaceshipY++;
            break;
        case 'x':
            gameOver = true;
            break;
        case ' ':
            if (fireCooldown <= 0) {
                fireCooldown = fireDelay;
                if (!bulletActive) {
                    bulletActive = true;
                    bulletX = spaceshipX;
                    bulletY = spaceshipY - 2;
                }
            }
            break;
        }
    }
    fireCooldown = max(0, fireCooldown - 1);
}

// The Logic function handles game logic
void Logic() {
    if (bulletActive) {
        for (size_t i = 0; i < asteroids.size(); i++) {
            if (asteroids[i].first == bulletX && asteroids[i].second == bulletY) {
                score++;
                bulletActive = false;
                playArea[bulletY][bulletX] = '~'; // Set the faded effect for the bullet
                asteroids[i] = {rand() % width, 0};
            }
        }
        if (playArea[bulletY][bulletX] != '~' && playArea[bulletY][bulletX] != 'l')
            playArea[bulletY][bulletX] = '6'; // Set the faded effect for every step of the bullet's path
        
        bulletY--;
        if (bulletY < 0)
            bulletActive = false;
    }

    for (auto& asteroid : asteroids) {
        asteroid.second++;
        if (asteroid.second >= height) {
            asteroid.first = rand() % width;
            asteroid.second = 0;
        }
        if (spaceshipX == asteroid.first && spaceshipY == asteroid.second) {
            gameOver = true;
            break;
        }
    }
}

// The main function controls the game flow
int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Normal speed of the game
    }
    cout << "Game Over!" << endl;
    return 0;
}