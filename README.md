# astar-visual

## A* Algorithm Visualization Code Explanation

### Introduction
The code is a C++ implementation of the A* (A-star) algorithm visualization using the SFML library for graphics. It creates a 20x20 grid and demonstrates how the A* algorithm finds the shortest path from the starting cell to the goal cell while avoiding obstacles. The visualization allows us to observe the pathfinding process step by step.

### 1. Importing Necessary Libraries
The code begins by importing the necessary C++ libraries for the project. It includes SFML/Graphics.hpp for graphics rendering, and standard C++ libraries for input/output, vectors, queues, and mathematical calculations.

### 2. Constants and Struct Definition
The code defines some constants such as `GRID_SIZE`, `SCREEN_WIDTH`, and `CELL_SIZE`. It also defines a `struct Cell` that represents each cell in the grid. The `Cell` struct contains properties like `x` and `y` coordinates, whether it's an `obstacle` or has been `visited`, and the `f`, `g`, and `h` scores used in the A* algorithm. The `parent` property is a pointer to the cell's parent in the path.

### 3. Heuristic Function
The `heuristic` function calculates the heuristic value (Euclidean distance) between two cells. It is used in the A* algorithm to estimate the distance from a cell to the goal cell.

### 4. A* Algorithm Implementation
The core of the A* algorithm is implemented in the `AStar` function. It takes the starting cell, goal cell, and the grid as input. The function returns a vector of cells representing the optimal path from the starting cell to the goal cell.

1. A priority queue named `open` is defined to store cells sorted by their `f` scores (lowest `f` score first).
2. The properties of all cells in the grid are reset to their initial state.
3. The `f`, `g`, and `h` scores of the starting cell are calculated and set.
4. The starting cell is added to the priority queue.
5. The algorithm continues as long as the priority queue is not empty.
6. The top cell from the priority queue is removed and stored in the `current` variable.
7. If the `current` cell is the goal cell, the path is reconstructed from the goal to the start by following the parent pointers.
8. The `current` cell is marked as visited, and its neighbors are expanded in all directions.
9. For each unvisited neighbor, the `g` score, `h` score, and `f` score are calculated.
10. If the neighbor is not in the priority queue or has a lower `g` score, it is added to the queue and marked with the current cell as its parent.
11. The algorithm repeats until the goal is reached or no path is found.

### 5. Initializing Grid and Obstacles
The main function initializes the SFML window and creates the grid of cells. It sets the start and goal cells, and then randomly generates obstacles on the grid based on the `obstacle_probability`.

### 6. Visualization Loop
The main loop runs while the SFML window is open. It handles window events, clears the screen, and then draws the grid lines, cells, starting cell in green, ending cell in red, and the path in blue.

1. The grid lines are drawn to create the visual representation of the cells in the grid.
2. Each cell is drawn as a rectangle with different colors:
   - Obstacles are drawn in black.
   - Visited cells during the A* algorithm execution are drawn in gray.
   - Unvisited cells are drawn in white.
3. The starting cell is drawn as a green rectangle to differentiate it from other cells.
4. The goal cell is drawn as a red rectangle to mark the destination point.
5. The path found by the A* algorithm is drawn as a series of blue circles, representing the sequence of cells forming the path.

### 7. Conclusion
The A* algorithm visualization code demonstrates a simple implementation of the A* algorithm for pathfinding on a grid. It uses SFML graphics to show the progress of the algorithm in finding the shortest path from the starting cell to the goal cell while avoiding obstacles. The visualization provides an interactive way to observe the pathfinding process, making it easier to understand how the A* algorithm works.
