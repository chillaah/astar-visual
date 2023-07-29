#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <random> // Include the random header

// Constants
const int GRID_SIZE = 20;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;

// Representing a cell in the grid
struct Cell {
    int x, y;       // Coordinates in the grid
    bool obstacle;  // Is this cell an obstacle?
    bool visited;   // Has this cell been visited during the algorithm?
    float f, g, h;  // A*, G, and H scores for the cell
    Cell* parent;   // Parent cell in the path
};

// Heuristic function for A* (Euclidean distance)
float heuristic(Cell* a, Cell* b) {
    return std::sqrt(std::pow(b->x - a->x, 2) + std::pow(b->y - a->y, 2));
}

// A* algorithm
std::vector<Cell*> AStar(Cell* start, Cell* goal, std::vector<std::vector<Cell>>& grid) {
    std::priority_queue<Cell*, std::vector<Cell*>, 
                        bool(*)(Cell*, Cell*)> open([](Cell* a, Cell* b) { return a->f > b->f; });
    std::vector<Cell*> path;

    // Reset cell properties
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j].visited = false;
            grid[i][j].f = grid[i][j].g = grid[i][j].h = 0;
            grid[i][j].parent = nullptr;
        }
    }

    start->g = 0;
    start->h = heuristic(start, goal);
    start->f = start->g + start->h;
    open.push(start);

    while (!open.empty()) {
        Cell* current = open.top();
        open.pop();

        if (current == goal) {
            // Reconstruct the path
            Cell* ptr = current;
            while (ptr != nullptr) {
                path.push_back(ptr);
                ptr = ptr->parent;
            }
            std::reverse(path.begin(), path.end());
            break;
        }

        current->visited = true;

        // Implement cell expansion here (up, down, left, right, and diagonals)
        const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        const int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

        for (int i = 0; i < 8; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            // Check if the neighbor is within the grid boundaries
            if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE && !grid[nx][ny].obstacle && !grid[nx][ny].visited) {
                float new_g = current->g + 1; // Assuming a cost of 1 to move to a neighbor

                if (new_g < grid[nx][ny].g || !grid[nx][ny].parent) {
                    grid[nx][ny].g = new_g;
                    grid[nx][ny].h = heuristic(&grid[nx][ny], goal);
                    grid[nx][ny].f = grid[nx][ny].g + grid[nx][ny].h;
                    grid[nx][ny].parent = current;
                    open.push(&grid[nx][ny]);
                }
            }
        }
    }

    return path;
}

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "A* Algorithm Visualization");

    // Create the grid of cells
    std::vector<std::vector<Cell>> grid(GRID_SIZE, std::vector<Cell>(GRID_SIZE));
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j].x = i;
            grid[i][j].y = j;
            grid[i][j].obstacle = false;
            grid[i][j].visited = false;
        }
    }

    // Create the start and goal cells
    Cell* start = &grid[0][0];
    Cell* goal = &grid[GRID_SIZE - 1][GRID_SIZE - 1];
    start->obstacle = false;
    goal->obstacle = false;

    // Randomly generate obstacles
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> obstacle_dist(0.0f, 1.0f);
    const float obstacle_probability = 0.3f; // Adjust this probability as needed

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (obstacle_dist(rng) < obstacle_probability) {
                grid[i][j].obstacle = true;
            }
        }
    }

    // Find the path using A*
    std::vector<Cell*> path = AStar(start, goal, grid);

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        // Draw grid lines
        for (int i = 0; i < GRID_SIZE; ++i) {
            sf::Vertex v1(sf::Vector2f(i * CELL_SIZE, 0), sf::Color::Black);
            sf::Vertex v2(sf::Vector2f(i * CELL_SIZE, SCREEN_HEIGHT), sf::Color::Black);
            sf::Vertex h1(sf::Vector2f(0, i * CELL_SIZE), sf::Color::Black);
            sf::Vertex h2(sf::Vector2f(SCREEN_WIDTH, i * CELL_SIZE), sf::Color::Black);
            window.draw(&v1, 2, sf::Lines);
            window.draw(&v2, 2, sf::Lines);
            window.draw(&h1, 2, sf::Lines);
            window.draw(&h2, 2, sf::Lines);
        }

        // Draw cells
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                cell.setPosition(i * CELL_SIZE + 1, j * CELL_SIZE + 1);
                if (grid[i][j].obstacle) {
                    cell.setFillColor(sf::Color::Black);
                } else if (grid[i][j].visited) {
                    cell.setFillColor(sf::Color(128, 128, 128));
                } else {
                    cell.setFillColor(sf::Color::White);
                }
                window.draw(cell);
            }
        }

        // Draw starting cell (green)
        sf::RectangleShape startCell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        startCell.setPosition(start->x * CELL_SIZE + 1, start->y * CELL_SIZE + 1);
        startCell.setFillColor(sf::Color::Green);
        window.draw(startCell);

        // Draw ending cell (red)
        sf::RectangleShape goalCell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
        goalCell.setPosition(goal->x * CELL_SIZE + 1, goal->y * CELL_SIZE + 1);
        goalCell.setFillColor(sf::Color::Red);
        window.draw(goalCell);

        // Draw path
        for (size_t i = 0; i < path.size(); ++i) {
            sf::CircleShape circle(CELL_SIZE / 4);
            circle.setFillColor(sf::Color::Blue);
            circle.setPosition(path[i]->x * CELL_SIZE + CELL_SIZE / 4, path[i]->y * CELL_SIZE + CELL_SIZE / 4);
            window.draw(circle);
        }

        window.display();
    }

    return 0;
}
