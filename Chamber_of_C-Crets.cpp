#include <iostream>
#include <fstream>
#include <vector>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
//#include <random>

//using namespace std; // Nope.

class Player {
    private:
        int pos_X;
        int pos_Y;

    public:
    /* Returns a container with non-obstructed positions for the player to start from */
    std::vector<std::pair<int, int>> getStartingPositions(const std::vector<std::vector<char>>& maze) {
        std::vector<std::pair<int, int>> validPositions;
        for (int i = 0; i < maze.size(); ++i) {
            for (int j = 0; j < maze[i].size(); ++j) {
                if (maze[i][j] != '*') {
                    validPositions.emplace_back(j, i);
                }
            }
        }
        return validPositions;
    }

    /* Randomly picks one of the valid starting positions */
    std::pair<int, int> randomizeStart(const std::vector<std::pair<int, int>>& validPositions) {
        int randomIndex = std::rand() % validPositions.size();
        return validPositions[randomIndex];
    }


    /* Getters for player position */
        [[nodiscard]] int get_X() const {
            return pos_X;
        }

        [[nodiscard]] int get_Y() const {
            return pos_Y;
        }

    /* Setters for player position */
        void set_X(int X) {
            pos_X = X;
        }

        void set_Y(int Y) {
            pos_Y = Y;
        }
    };

// Displaying character elements read from the maze layout file.
void traceMaze(const std::vector<std::vector<char>>& maze, const Player& player) {
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[i].size(); ++j) {
            if (i == player.get_Y() && j == player.get_X()) {
                attron(COLOR_PAIR(1)); // Coloring the player sprite red.
                addch('M');
                attron(COLOR_PAIR(1));
            } else if (maze[i][j] == '*') {
                addch(ACS_BLOCK); // Adding a block character for walls.
            } else if (maze[i][j] == '.') {
                addch(' '); // Adding a space for traversable space.
            } else {
                addch(maze[i][j]);
            }
        }
        addch('\n');
    }
    refresh();
}

std::vector<std::vector<char>> readMazeLayout(const std::string& filename) {
    std::ifstream layoutFile(filename);
    // Covering the case of reading failure.
    if (!layoutFile) {
        std::cerr << "Can not read maze layout layoutFile: " << filename << std::endl;
        return {};
    }

    // Reading file contents line by line.
    std::vector<std::vector<char>> maze;
    std::string line;
    while (std::getline(layoutFile, line)) {
        std::vector<char> row(line.begin(), line.end());
        maze.push_back(row);
    }

    return maze;
}

int main() {

    std::string mazeFile;
    std::cout << "Enter the name of the maze layout file: ";
    std::cin >> mazeFile;

    setlocale(LC_ALL, "");
    initscr(); // Initializing screen
    keypad(stdscr, TRUE);  // Enable capturing of special keys
    cbreak();   // Disable line buffering
    noecho();   // Don't display input characters
    curs_set(0); // Hiding cursor
    nodelay(stdscr, true); // Don't wait for user input on getch() calls.

    init_pair(1, COLOR_RED, COLOR_BLACK);  // Defining color pair 1 (terminal dependant).

    std::vector<std::vector<char>> maze = readMazeLayout(mazeFile);
    if (maze.empty()) {
        std::cout << "The file provided is empty!" << std::endl;
        endwin();
        return(EXIT_FAILURE);
    }

    Player Potter;

    std::srand(std::time(nullptr));  // Generic seeding of the random number generator
    std::vector<std::pair<int, int>> validPositions = Potter.getStartingPositions(maze);
    if (validPositions.empty()) {
        endwin();
        std::cerr << "No valid starting positions in the maze." << std::endl;
        return(EXIT_FAILURE);
    }
    std::pair<int, int> startingPosition = Potter.randomizeStart(validPositions);
    Potter.set_X(startingPosition.first);
    Potter.set_Y(startingPosition.second);

    traceMaze(maze, Potter);

    int playerInput;
    while ((playerInput = getch()) != 27 /* Esc in ASCII */) {
        switch (playerInput) {
            case KEY_UP:
                if (Potter.get_Y() > 0 && maze[Potter.get_Y() - 1][Potter.get_X()] != '*') {
                    Potter.set_Y(Potter.get_Y() - 1);
                }
                break;
            case KEY_DOWN:
                if (Potter.get_Y() < maze.size() - 1 && maze[Potter.get_Y() + 1][Potter.get_X()] != '*') {
                    Potter.set_Y(Potter.get_Y() + 1);
                }
                break;
            case KEY_LEFT:
                if (Potter.get_X() > 0 && maze[Potter.get_Y()][Potter.get_X() - 1] != '*') {
                    Potter.set_X(Potter.get_X() - 1);
                }
                break;
            case KEY_RIGHT:
                if (Potter.get_X() < maze[Potter.get_Y()].size() - 1 && maze[Potter.get_Y()][Potter.get_X() + 1] != '*') {
                    Potter.set_X(Potter.get_X() + 1);
                }
                break;
            case ' ':
                break;
        }
        erase();
        traceMaze(maze, Potter);
    }

    endwin(); // Clean up ncurses
    return 0;
}
