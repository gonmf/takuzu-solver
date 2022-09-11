**Takuzu solver**

A simple Takuzu puzzle solver for boards between 3x3 and 31x31.

Takuzu, also known as Binairo, Binero, Tohu-Wa-Vohu, Eins und Zwei, Binoxxo, Zernero or just Binary Sudoku, is a puzzle game of constraints similar to Sudoku but with only 1s and 0s.

Rules vary, though this solver is specifically for rules:

* Every column and row must be filled in with the same number of 1s.
* There can be no more than two 1s or 0s in a row.

Some formulations specify that the rows and columns have to be unique, that the puzzle need not be square, etc. These are not considered in this version.

It uses local checks to rule out candidates around or next to two filles squares in a row, counts of line and row 1 counts to rule out candidates after the first row/column is filled, and finally does a single position substitution at random.
