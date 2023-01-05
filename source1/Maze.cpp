#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "Miscellaneous.cpp"

using Maze = std::vector<std::vector<int>>;
using Connection = std::map<std::vector<int>, std::vector<std::vector<int>>>;


Maze createEmptyMaze(int weidth, int height) {
    Maze maze;
    for(int i = 0; i < height; i++) {
        std::vector<int> row;
        for(int j = 0; j < weidth; j++) {
            row.push_back(0);
        }
        maze.push_back(row);
    }
    return maze;
}

bool isValidPosition(int x, int y, int weidth, int height) {
    if(x < 0 || x >= weidth || y < 0 || y >= height) {
        return false;
    } else {
        return true;
    }
}

bool canNewBlockFit(Maze& maze, int x, int y, int weidth, int height) {
    if(isValidPosition(x, y, weidth, height) && isValidPosition(x+3, y+3, weidth, height)) {
        for(int j = y; j < y+4; j++) {
            for(int i = x; i < x+4; i++) {
                if(maze[j][i] != 0) {
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

std::vector<std::vector<int>> getPossibleStarts(Maze& maze) {
    std::vector<std::vector<int>> possibleStarts;
    for(int y = 0; y < maze.size(); y++) {
        for(int x = 0; x < maze[0].size(); x++) {
            if(canNewBlockFit(maze, x, y, maze[0].size(), maze.size())) {
                std::vector<int> position = {x, y};
                possibleStarts.push_back(position);
            }
        }
    }
    return possibleStarts;
}
void addConnection(Maze& maze, Connection& connections, std::vector<std::vector<int>>& possibleStarts, int x, int y, int dx, int dy);
void connect(Maze& maze, Connection& connections, std::vector<std::vector<int>>& possibleStarts, int x, int y, int x0, int y0);

Connection getConnections(Maze& maze, std::vector<std::vector<int>>& possibleStarts) {
    Connection connections;
    for(int y = 0; y < maze.size(); y++) {
        for(int x = 0; x < maze[0].size(); x++) {
            std::vector<int> position = {x, y};
            if(std::find(possibleStarts.begin(), possibleStarts.end(), position) != possibleStarts.end()) {
                for(int y0 = 0; y0 < 4; y0++) {
                    if(!isValidPosition(x-1, y+y0, maze[0].size(), maze.size())) {
                        continue;
                    }
                    if(maze[y+y0][x-1] == 1) {
                        addConnection(maze, connections, possibleStarts, x, y, 1, 0);
                        break;
                    }
                }
                for(int y0 = 0; y0 < 4; y0++) {
                    if(!isValidPosition(x+4, y+y0, maze[0].size(), maze.size())) {
                        continue;
                    }
                    if (maze[y+y0][x+4] == 1) {
                        addConnection(maze, connections, possibleStarts, x, y, -1 ,0);
                        break;
                    }
                }
                for(int x0 = 0; x0 < 4; x0++) {
                    if(!isValidPosition(x+x0, y-1, maze[0].size(), maze.size())) {
                        continue;
                    }
                    if (maze[y-1][x+x0] == 1) {
                        addConnection(maze, connections, possibleStarts, x, y, 0, 1);
                        break;
                    }
                }
                for(int x0 = 0; x0 < 4; x0++) {
                    if(!isValidPosition(x+x0, y+4, maze[0].size(), maze.size())) {
                        continue;
                    }
                    if (maze[y+4][x+x0] == 1) {
                        addConnection(maze, connections, possibleStarts, x, y, 0, -1);
                        break;
                    }
                }
            }

        }
    }
    return connections;
}

void addConnection(Maze& maze, Connection& connections, std::vector<std::vector<int>>& possibleStarts, int x, int y, int dx, int dy) {
    std::vector<int> source = {x, y};
    if(std::find(possibleStarts.begin(), possibleStarts.end(), source) != possibleStarts.end()) {
        connect(maze, connections, possibleStarts, x, y, x+dx, y+dy);
        connect(maze, connections, possibleStarts, x, y, x+2*dx, y+2*dy);
        source = {x-dy, y-dx};
        if(std::find(possibleStarts.begin(), possibleStarts.end(), source) != possibleStarts.end()) {
            
        } else {
            connect(maze, connections, possibleStarts, x, y, x+dx-dy, y+dy-dx);
        }
        source = {x+dy, y+dx};
        if(std::find(possibleStarts.begin(), possibleStarts.end(), source) != possibleStarts.end()) {
            
        } else {
            connect(maze, connections, possibleStarts, x, y, x+dx+dy, y+dy+dx);
        }
        source = {x+dx-dy, y+dy-dx};
        if(std::find(possibleStarts.begin(), possibleStarts.end(), source) != possibleStarts.end()) {
            
        } else {
            connect(maze, connections, possibleStarts, x, y, x+2*dx-dy, y+2*dy-dx);
        }
        source = {x+dx+dy, y+dy+dx};
        if(std::find(possibleStarts.begin(), possibleStarts.end(), source) != possibleStarts.end()) {

        } else {
            connect(maze, connections, possibleStarts, x, y, x+2*dx+dy, y+2*dy+dx);
        }
    }
}

void connect(Maze& maze, Connection& connections, std::vector<std::vector<int>>& possibleStarts, int x, int y, int x0, int y0) {
    std::vector<int> source = {x, y};
    std::vector<int> destination = {x0, y0};
    if(std::find(possibleStarts.begin(), possibleStarts.end(), destination) != possibleStarts.end()) {
        if(connections.count(destination) == 0) {
            connections[destination] = {source};
        } else {
            connections[destination].push_back(source);
        }
    } else {
        return;
    }
}

void expandWall(Maze& maze, Connection& connections);

bool addRandomBlock(Maze& maze) {
    std::vector<std::vector<int>> possibleStarts = getPossibleStarts(maze);
    Connection connections = getConnections(maze, possibleStarts);
    // std::cout << connections.size() << std::endl;
    if(possibleStarts.size() == 0) {
        return false;
    } else {
        std::vector<int> position = possibleStarts[getRandomInt(0, possibleStarts.size()-1)];
        int x = position[0];
        int y = position[1];

        for(int i = x+1; i <= x+2; i++) {
            for(int j = y+1; j <= y+2; j++) {
                maze[j][i] = 1;
            }
        }

        expandWall(maze, connections);

        return true;
    }
}

int main() {
    Maze maze = createEmptyMaze(16,31);

    while(addRandomBlock(maze)) {
        continue;
    }

    for(int y = 0; y < 31; y++) {
        for(int x = 0; x < 16; x++) {
            std::cout << maze[y][x] << ' ';
        }
        std::cout << std::endl;
    }

}