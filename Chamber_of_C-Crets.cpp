#include <iostream>
#include <fstream>
#include <vector>
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <unistd.h>

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

class Gem: public Player {
};

class NPC: public Player {
};

// Displaying character elements read from the maze layout file.
void traceMaze(const std::vector<std::vector<char>>& maze, const Player& player, const Gem& gem, const NPC& npc) {
    for(int i = 0; i < maze.size(); ++i) {
        for(int j = 0; j < maze[i].size(); ++j) {
            if(i == player.get_Y() && j == player.get_X()) {
                attron(COLOR_PAIR(1)); // Coloring the player sprite red.
                addch('M');
                attron(COLOR_PAIR(1));
            } else if(i == gem.get_Y() && j == gem.get_X()) {
                attron(COLOR_PAIR(2));  // Coloring the gem sprite green.
                addch('G');
                attroff(COLOR_PAIR(2));
            } else if(i == npc.get_Y() && j == npc.get_X()) {
                attron(COLOR_PAIR(3));
                addch('L');
                attroff(COLOR_PAIR(3));
            } else if(maze[i][j] == '*') {
                addch(ACS_BLOCK); // Adding a block character for walls.
            } else if(maze[i][j] == '.') {
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

bool isWithinBounds(int X, int Y, int Width, int Height) {
    return X >= 0 && Y < Width && Y >= 0 && Y < Height;
}

std::vector<std::pair<int,int>> getAdjacentCells(const std::vector<std::vector<char>>& maze, const std::pair<int, int>& coords) {
    std::vector<std::pair<int,int>> adjacentCells;
    int x = coords.first;
    int y = coords.second;

    // Check left
    if (x > 0 && maze[y][x - 1] != '*') {
        adjacentCells.emplace_back( x - 1, y );
    }

    // Check right
    if (x < maze[0].size() - 1 && maze[y][x + 1] != '*') {
        adjacentCells.emplace_back( x + 1, y );
    }

    // Check up
    if (y > 0 && maze[y - 1][x] != '*') {
        adjacentCells.emplace_back( x, y - 1 );
    }

    // Check down
    if (y < maze.size() - 1 && maze[y + 1][x] != '*') {
        adjacentCells.emplace_back( x, y + 1 );
    }

    return adjacentCells;
}

bool isValidMove(const std::vector<std::vector<char>>& maze, int X, int Y) {
    int Width = maze[0].size();
    int Height = maze.size();
    return isWithinBounds(X, Y, Width, Height) && maze[Y][X] != '*';
}

std::vector<std::pair<int,int>> findPath(const std::vector<std::vector<char>>& maze, const NPC& start, const Gem& gem) {
    std::vector<std::pair<int, int>> path;
    std::vector<std::vector<bool>> visited(maze.size(), std::vector<bool>(maze[0].size(), false));

    std::vector<std::vector<std::pair<int, int>>> prev(maze.size(), std::vector<std::pair<int,int>>(maze[0].size(), { -1, -1 }));

    std::vector<std::pair<int, int>> queue;
    queue.emplace_back(start.get_X(), start.get_Y());
    visited[start.get_X()][start.get_Y()] = true;

    bool gemFound = false;
    while(!queue.empty() && !gemFound) {
        std::pair<int, int> current = queue.front();
        queue.erase(queue.begin());

        std::vector<std::pair<int, int>> adjacentCells = getAdjacentCells(maze, {current.first, current.second});
        for(const auto& adjacentCell : adjacentCells) {
            int new_X = adjacentCell.first;
            int new_Y = adjacentCell.second;

            if(!visited[new_Y][new_X]) {
                queue.emplace_back(new_X, new_Y);
                visited[new_Y][new_X] = true;
                prev[new_Y][new_X] = {current.first, current.second};

                if(new_X == gem.get_X() && new_Y == gem.get_Y()) {
                     gemFound = true;
                    break;
                }
            }
        }
    }

    if(gemFound) {
        std::pair<int, int> current = {gem.get_X(), gem.get_Y()};
        while(current.first != start.get_X() || current.second != start.get_Y()) {
            path.push_back(current);  // Convert current to Character
            std::pair<int, int> prevCell = prev[current.second][current.first];
            current.first = prevCell.first;
            current.second = prevCell.second;
        }
        path.emplace_back(start.get_X(), start.get_Y());
        std::reverse(path.begin(), path.end());
    }

    return path;
}

void updateNPC(NPC& npc, const std::vector<std::pair<int, int>>& path) {
    if (!path.empty()) {
        npc.set_X(path.back().first);
        npc.set_Y(path.back().second);
    }
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
    init_pair(2, COLOR_BLUE, COLOR_BLACK); // Defining color pair 2...
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // And so on...

    std::vector<std::vector<char>> maze = readMazeLayout(mazeFile);
    if (maze.empty()) {
        std::cout << "The file provided is empty!" << std::endl;
        endwin();
        return(EXIT_FAILURE);
    }

    Player Potter;

    std::srand(std::time(nullptr) + getpid() + 1337);  // Generic seeding of the random number generator
    std::vector<std::pair<int, int>> validPositions = Potter.getStartingPositions(maze);
    if (validPositions.empty()) {
        endwin();
        std::cerr << "No valid starting positions in the maze." << std::endl;
        return(EXIT_FAILURE);
    }

    std::pair<int, int> startingPosition = Potter.randomizeStart(validPositions);
    Potter.set_X(startingPosition.first);
    Potter.set_Y(startingPosition.second);

    Gem Philosopher_Stone;
    while (Potter.get_X() == startingPosition.first && Potter.get_Y() == startingPosition.second) {
        startingPosition = Philosopher_Stone.randomizeStart(validPositions);
    }
    Philosopher_Stone.set_X(startingPosition.first);
    Philosopher_Stone.set_Y(startingPosition.second);

    NPC Malfoy;
    while (Malfoy.get_X() == startingPosition.first && Malfoy.get_Y() == startingPosition.second) {
        startingPosition = Philosopher_Stone.randomizeStart(validPositions);
    }
    Malfoy.set_X(startingPosition.first);
    Malfoy.set_Y(startingPosition.second);

    traceMaze(maze, Potter, Philosopher_Stone, Malfoy);

    int playerInput;
    while ((playerInput = getch()) != 27 /* Esc in ASCII */) {
        switch (playerInput) {
            case KEY_UP:
                if (isValidMove(maze, Potter.get_X(), Potter.get_Y() - 1)) {
                    Potter.set_Y(Potter.get_Y() - 1);
                    std::vector<std::pair<int, int>> path = findPath(maze, Malfoy, Philosopher_Stone);
                    updateNPC(Malfoy, path);
                }
                break;
            case KEY_DOWN:
                if (isValidMove(maze, Potter.get_X(), Potter.get_Y() + 1)) {
                    Potter.set_Y(Potter.get_Y() + 1);
                    std::vector<std::pair<int, int>> path = findPath(maze, Malfoy, Philosopher_Stone);
                    updateNPC(Malfoy, path);
                }
                break;
            case KEY_LEFT:
                if (isValidMove(maze, Potter.get_X() - 1, Potter.get_Y())) {
                    Potter.set_X(Potter.get_X() - 1);
                    std::vector<std::pair<int, int>> path = findPath(maze, Malfoy, Philosopher_Stone);
                    updateNPC(Malfoy, path);
                }
                break;
            case KEY_RIGHT:
                if (isValidMove(maze, Potter.get_X() + 1, Potter.get_Y())) {
                    Potter.set_X(Potter.get_X() + 1);
                    std::vector<std::pair<int, int>> path = findPath(maze, Malfoy, Philosopher_Stone);
                    updateNPC(Malfoy, path);
                }
                break;
            case ' ':

                break;
        }
        if ((Potter.get_X() == Philosopher_Stone.get_X()) && (Potter.get_Y() == Philosopher_Stone.get_Y())) {
            endwin();
            std::cout << "Teleportation commenced!";
            exit(EXIT_SUCCESS);
        }
        erase();
        traceMaze(maze, Potter, Philosopher_Stone, Malfoy);

    }

    return (EXIT_SUCCESS);
}
