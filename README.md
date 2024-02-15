# Bi-Directional Breadth-First Search (BiBFS) Maze Solver

This C++ program demonstrates the Bi-Directional Breadth-First Search (BiBFS) algorithm to find the shortest path in a maze. The maze is represented as a grid where the algorithm searches for the path from the start point to the target point and vice versa.

## Features

- **Bi-Directional BFS**: Utilizes a bidirectional approach to simultaneously search from the start and target points, aiming to find the shortest path.
- **OpenGL Visualization**: Utilizes OpenGL (via GLUT) for graphical representation of the maze and pathfinding process.
- **Maze Initialization**: Generates a maze with walls, start, and target points using a randomized algorithm.
- **Path Restoration**: Restores and visualizes the shortest path found by the BiBFS algorithm.

## Requirements

- **OpenGL**: The program uses the GLUT library for graphics. Make sure to have an OpenGL environment set up.

## Compilation and Execution

1. Clone the repository:

   ```bash
   git clone <repository_url>
   cd <repository_directory>
   ```

2. Compile the code:

   ```bash
   g++ -o maze_solver maze_solver.cpp -lGL -lGLU -lglut
   ```

3. Run the executable:

   ```bash
   ./maze_solver
   ```

## Usage

1. Upon execution, a window titled "BFS Example" will open, displaying the maze.
2. Right-click to access the menu and select "run BFS" to initiate the BiBFS algorithm.
3. The algorithm will search for the shortest path simultaneously from the start and target points.
4. The path, once found, will be visualized in magenta color.

## Note

- This program assumes a Unix-like environment. Adjust compilation commands accordingly for Windows or other platforms.

Feel free to explore and modify the code as needed for your understanding or customization. If you encounter any issues or have questions, please don't hesitate to ask.

Happy coding!
