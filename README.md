Synchronous-Control
===================

The cave can be modelled as a 2-dimensional maze with R rows and C columns. It will be represented as a String[] cave, where the j-th character of the i-th element describes the contents of the cell in row i, column j (0-based):

    '.' - for an empty cell;
    '#' - if the cell contains an impassable wall;
    'x' - if the cell is one of the exits from the maze;
    'S' - if the cell is occupied by a robot faced to the South;
    'N' - if the cell is occupied by a robot faced to the North;
    'E' - if the cell is occupied by a robot faced to the East;
    'W' - if the cell is occupied by a robot faced to the West.

The northwest corner of the maze is located at the cell in row 0, column 0, and the southeast corner is at the cell in row R-1, column C-1.

There are three kinds of commands that can be send to the robots:

    'R' - each robot turns 90 degrees clockwise (for example, if robot’s direction is South, it will become West).
    'L' - each robot turns 90 degrees counterclockwise (for example, if robot’s direction is West, it will become South).
    'F' - each robot checks whether it is possible to move 1 cell forward in its current direction. If it’s possible, the robot makes such move, otherwise it stays in its current cell. A move is possible if destination cell does not contain a wall (it is allowed to contain an exit or another robot).

Each command requires some time to be executed. For 'R' and 'L' commands it is simple: each of them requires exactly 1 time unit. 'F' command is more complicated. The reason is that when a robot makes a move, it leaves an "oil" trail in the destination cell and this "oil" will slow down the robots moving to the same cell later. Exactly, it works as follows:

    Each cell has a certain "contamination level". Since all robots were inaccessible during a long time period, all "oil" from their previous moves had time to dry out. Therefore, at the start of your method the maze is completely "clean" and all contamination levels are equal to zero.
    When commands 'R' and 'L' are executed, contamination levels remain unchanged.
    For 'F' command, let Robots be the set of all robots which can make a move and Cells be the set of destination cells for all robots from Robots. If Cells is empty (nobody can make a move), the command takes exactly 1 time unit and its execution is over. Otherwise, let Max be the maximum contamination level among all cells from Cells. The command will take exactly 1 + ln (1 + Max) / V time units, where double V is an input parameter. Once all robots finished their moves, for each robot from Robots, the contamination level of its destination cell is increased by 1 (this happens even if this cell contains an exit). Note that if several robots move to the same destination cell, its contamination level will get increased several times. Finally, all robots that reached exit cells after the move get evacuated.