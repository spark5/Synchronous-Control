#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

const static int turn_right[] = { 1, 2, 3, 0 };
const static int turn_left[] = { 3, 0, 1, 2 };
const static int dr[] = { -1, 0, 1, 0 };
const static int dc[] = { 0, 1, 0, -1 };
const static char empty = '.', wall = '#', exit_cell = 'x';

typedef struct { int x, y, dir; string command; } info_cell;	
typedef struct { int x, y; } one_move;

class Robot {

	public:
		int row, col, dir;
		int x, y;
		bool active, moved;
			 
		Robot(int r, int c, int d, int n, int m)
		{
			row = r;
			col = c;
			dir = d;
			x = n;
			y = m;
		
			active = true;
			moved = false;
		}

		void turnLeft()
		{
			dir = turn_left[dir];
		}

		void turnRight()
		{
			dir = turn_right[dir];
		}

		void moveForward(vector<string> cave) 
		{
			int r = row + dr[dir];
			int c = col + dc[dir];

			if(cave[r][c] != wall && r >= 0 && r <= x - 1 && 
				c >= 0 && c <= y - 1)
			{
				row = r;
				col = c;
				moved = true;
			}
			else
			{
				moved = false;
			}
		
			if(cave[row][col] == exit_cell)
			{
				active = false;
			}
		}

		bool isActive()
		{
			return active;
		}

		void Robot::executeCommand(char c, vector<string> &cave)
		{
			if(c == 'L')
			{
				turnLeft();
			}

			if(c == 'R')
			{
				turnRight();
			}
	
			if(c == 'F')
			{
				moveForward(cave);	
			}
		}
};

class SynchronousControl {
		
	public:
	
		//main parameters for the cave
		int r, c;	
		int n, m;
		double v;

		vector<string> cave;
		vector<one_move> exits;
		vector<Robot> robots;
			
		SynchronousControl::SynchronousControl()
		{
			n = 0;
			m = 0;
		}

		SynchronousControl::~SynchronousControl()
		{
			cave.clear();
			exists.clear();
		}
		
		void bfs(vector<string> cave, info_cell source, vector<one_move> exits, 
					string &path);	
		string evacuateAll(vector<string> cave, double V);
};

//returns the path to the closest exit for a robot
void SynchronousControl::bfs(vector<string> cave, info_cell source, 		
	vector<one_move> exits, string &path)
{
	info_cell nowhere;
	queue<info_cell> Q;
	info_cell parent[r][c];		//parent matrix for reconstructing the path
	
	nowhere.x = -1;
	nowhere.y = -1;
	nowhere.dir = -1;
		
	for(int i = 0; i < r; i++)
	{
		for(int j = 0; j < c; j++)
		{
			parent[i][j] = nowhere;
		}
	}
	
	Q.push(source);
	parent[source.x][source.y] = source;
	
	while(!Q.empty())
	{
		info_cell currentState = Q.front();
		Q.pop();
		
		//test for ending condition
		for(int i = 0; i < m; i++)
		{
			if(exits[i].x == currentState.x && exits[i].y == currentState.y)
			{
				//we reconstruct the path from the source to destination
				do 
				{	
					path.insert(0, currentState.command);
					currentState = parent[currentState.x][currentState.y];
					
				} while(currentState.x != source.x && currentState.y != source.y);
				
				return;	
			}	
		}
		
		//we expand the next states from the currentState
		info_cell newState;
		
		newState.x = currentState.x + dr[currentState.dir];
		newState.y = currentState.y + dc[currentState.dir];
		newState.dir = currentState.dir;
		newState.command.push_back('F');
		
		if(cave[newState.x][newState.y] != wall && 
			newState.x >= 0 && newState.x <= r - 1 &&
			newState.y >= 0 && newState.x <= c - 1 &&
			parent[newState.x][newState.y].x == -1 && 
			parent[newState.x][newState.y].y == -1)
		{	
			Q.push(newState);
			parent[newState.x][newState.y] = currentState;
		}
			
		newState.command.clear();
			
		for(int i = 0; i < 1; i++)
		{	
			newState.dir = turn_left[currentState.dir]; 
			newState.x = currentState.x + dr[newState.dir];
			newState.y = currentState.y + dc[newState.dir];
			
			if(cave[newState.x][newState.y] != wall &&
				newState.x >= 0 && newState.x <= r - 1 &&
				newState.y >= 0 && newState.x <= c - 1 &&
				parent[newState.x][newState.y].x == -1 && 
				parent[newState.x][newState.y].y == -1)
			{
				if(i == 0)
				{
					newState.command.append("LF");
				}
				else
				{
					newState.command.append("LLF");
				}
				
				Q.push(newState);
				parent[newState.x][newState.y] = currentState;
				newState.command.clear();
			}
		}
		
		for(int i = 0; i < 1; i++)
		{	
			newState.dir = turn_right[currentState.dir]; 
			newState.x = currentState.x + dr[newState.dir];
			newState.y = currentState.y + dc[newState.dir];
			
			if(cave[newState.x][newState.y] != wall &&
				newState.x >= 0 && newState.x <= r - 1 &&
				newState.y >= 0 && newState.x <= c - 1 &&
				parent[newState.x][newState.y].x == -1 && 
				parent[newState.x][newState.y].y == -1)
			{
				if(i == 0)
				{
					newState.command.append("RF");
				}
				else
				{
					newState.command.append("RRF");
				}
				
				Q.push(newState);
				parent[newState.x][newState.y] = currentState;
				newState.command.clear();
			}
		}
	} 
}

//function that evacuates all the robots
string SynchronousControl::evacuateAll(vector<string> cave, double V)
{
	int k;
	info_cell source;
	string ret, path;
	
	while(robots.size() != 0)
	{
		for(int k = 0; k < n; k++)
		{
			source.x = robots[k].row;
			source.y = robots[k].col;
			source.dir = robots[k].dir;
		
			//we find the closest path to an exit for the current robot
			bfs(cave, source, exits, path);
		
			if(path.length() != 0)
			{
				//we save the current solution into the global solution
				ret.append(path);
				
				//we dismiss the robot from the robots' set
				robots.erase(robots.begin() + k);
				n--;
		
				//we make changes for the rest of the robots and in the cave
				for(unsigned int i = 0; i < path.length(); i++)
				{
					for(int j = 0; j < n; j++)
					{
						if(robots[j].isActive() == true)
						{
							robots[j].executeCommand(path[i], cave);
						}
						
						if(robots[j].isActive() == false)
						{
							robots.erase(robots.begin() + j);
							j--;
							n--;
						}
					}
				}
				
				path.clear();
			}
		}
	}
	
	return ret;
}
	
int main()
{
	char line[120];
	one_move info;
	string ret;
	SynchronousControl sc;
	
	//we are reading the main parameters for the cave 
	scanf("%d", &sc.r);
	
	for(int i = 0; i < sc.r; i++)
	{
		scanf("%s", line);
		sc.cave.push_back(line);
		sc.c = sc.cave[0].length();
	
		for(int j = 0; j < sc.cave[i].length(); j++)
		{
			char dir = sc.cave[i][j]; 
		
			//we make a vector with all the exists
			if(dir == exit_cell)
			{
				info.x = i;
				info.y = j; 
				sc.exits.push_back(info);		
				sc.m++;	
			}
			
			//we make a vector with all the robots' initial locations
			if(dir == 'N') 
			{
				sc.robots.push_back(Robot(i, j, 0, sc.r, sc.c));
				sc.n++;
			}	
			
			if(dir == 'E') 
			{
				sc.robots.push_back(Robot(i, j, 1, sc.r, sc.c));
				sc.n++;
			}	
			
			if(dir == 'S') 
			{
				sc.robots.push_back(Robot(i, j, 2, sc.r, sc.c));
				sc.n++;
			}	
			
			if(dir == 'W') 
			{
				sc.robots.push_back(Robot(i, j, 3, sc.r, sc.c));
				sc.n++;
			}	
		}
	}
	
	scanf("%lf", &sc.v);

	//we compute the commands for all the robots
	ret = sc.evacuateAll(sc.cave, sc.v);
	
	cout << ret;

	fflush(stdout);
	
	return 0;
}
