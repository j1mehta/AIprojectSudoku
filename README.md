# AIprojectSudoku
Problem Description
The problem description is to find an algorithm that returns the correct solution to a given Sudoku puzzle. The Sudoku Puzzle can be stated a follows:

The puzzle consists of a 9 x 9 grid which is divided into nine 3 x 3 sub-grids.
The objective is to fill in digits from 1 to 9 such that each row, each column, and each of the nine 3×3 sub-grids that compose the grid contain all of the digits from 1 to 9.
As a result of the previous statement, we have the constraint that the same single integer may not appear twice in the same row, column, or any of the nine 3×3 sub-grids

The main approach followed is backtracking search with constraint propagation. Additionally, to improve efficiency, I applied the Minimum-Remaining-Values (MRV) heuristic.
The constraint network is as follows:
Variables: 81 squares
Domain: Integer values 1, 2, 3 … 9.
Constraints: In a given row, column, or 3 x 3 sub-grids, the same digit cannot appear twice. Total number of constraints is 27 for a 9X9 grid.
