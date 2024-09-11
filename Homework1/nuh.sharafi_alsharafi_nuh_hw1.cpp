//
// Created by nuh alsharafi on 7/24/24.
//
#include <iostream>
#include <fstream>
#include "stack.h"
#include "randgen.h"
using namespace std;

//--------------------------Maze Generation Functions----------------------------//
struct blockm //m for maze
{
    int x,y; //coordinates
    int u,d,l,r; //0,1 to indicate presence of a wall
    bool visited; //to indicate if cell has been visited

    //default constructor
    blockm()
    {
        x = y = -1;
        u = d = l = r = 1;
        visited = false;
    }

    //parametric constructor
    blockm(int xc, int yc, int us = 1, int ds = 1, int ls = 1, int rs = 1, bool vis=false)
        : x(xc), y(yc), u(us), d(ds), l(ls), r(rs), visited(vis) {}
};

void saveMaze(blockm** const &, int, int, int);

void makeMaze(blockm** &maze, const int M, const int N) //M and N are the matrix dimensions
{
    /* Function would turn the 2D matrix into a maze using a stack */

    //set the indices of the matrix (bottom left is 0,0)
    for(int i=M-1; i>=0; i--)
    {
        for (int j=0; j<N; j++)
        {
            maze[i][j].x = j;
            maze[i][j].y = M-i-1;
        }
    }

    //create stack and start exploration
    Stack<blockm*> mazeMaker;

    //push the first block (0,0) (bottom left)
    mazeMaker.push(&maze[M-1][0]);

    bool unvisitedExist = true;
    while (unvisitedExist)
    {
        //while there's unvisited blocks, continue the process

        //get top
        blockm* current = mazeMaker.top();
        current->visited = true; //mark as visited

        //test options (wall is up and is not an edge)
        char options[4];
        int index = -1;
        //check up
        if (current->u == 1 && current->y != M - 1 && !(maze[M - current->y - 2][current->x].visited))
        {
            index++;
            options[index] = 'u';
        }

        if (current->d == 1 && current->y != 0 && !(maze[M-current->y][current->x].visited))
        {
            index++;
            options[index] = 'd';
        }

        if (current->l == 1 && current->x != 0 && !(maze[M - current->y - 1][current->x-1].visited))
        {
            index++;
            options[index] = 'l';
        }

        if (current->r == 1 && current->x != N-1 && !(maze[M - current->y - 1][current->x+1].visited))
        {
            index++;
            options[index] = 'r';
        }

        //Case 1: option available
        if(index !=-1)
        {
            //choose a random option
            RandGen rand;
            char choice = options[rand.RandInt(0, index)];

            if(choice == 'u')
            {
                //drop wall
                current->u = 0;

                //add new block to stack
                mazeMaker.push(&maze[M-current->y-2][current->x]);
                //drop the opposite wall for the new block
                mazeMaker.top()->d = 0;

            }
            else if (choice == 'd')
            {
                //drop wall
                current->d = 0;

                //add new block to stack
                mazeMaker.push(&maze[M-current->y][current->x]);

                //drop the opposite wall for the new block
                mazeMaker.top()->u = 0;
            }
            else if (choice == 'l')
            {
                //drop wall
                current->l = 0;

                //add new block to stack
                mazeMaker.push(&maze[M- current->y - 1][current->x-1]);

                //drop opposite wall from new block
                mazeMaker.top()->r = 0;
            }
            else
            {
                //drop wall
                current->r = 0;

                //add new block to stack
                mazeMaker.push(&maze[M - current->y- 1][current->x+1]);

                //drop opposite wall from new block
                mazeMaker.top()->l = 0;
            }

        }
        //Case 2: no option, go back and re-evaluate
        else
        {
            if(!mazeMaker.isEmpty())
            {
                mazeMaker.pop();
            }
            else
            {
                cout << "stack pop logic error";
                return;
            }
        }

        //check if there's no blocks left unvisited
        unvisitedExist = false; //assume there's non left unvisited
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N; j++)
            {
                if (!maze[i][j].visited)
                {
                    unvisitedExist = true;
                }
            }
        }
    }
    //should empty the stack now
    mazeMaker.makeEmpty();
}


void saveMaze(blockm** const &maze, int mazeID, int M, int N)
{
    /* Function saves generated maze into a text file
     * M and N are the maze dimensions*/

    ofstream outfile;
    string filename = "maze_" + to_string(mazeID) + ".txt";

    outfile.open(filename.c_str());

    //handle error
    if(outfile.fail())
    {
        cout << "Couldn't create file for maze " << mazeID << endl;
    }
    else
    {
        //write maze into file

        //write dimensions
        outfile << M << " " << N << endl;
        for(int i=M-1; i>=0; i--)
        {
            for(int j=0; j<N; j++)
            {
                outfile << "x=" << maze[i][j].x << " y=" << maze[i][j].y << " l=" <<
                    maze[i][j].l << " r=" << maze[i][j].r << " u=" << maze[i][j].u <<
                    " d=" << maze[i][j].d << endl;
            }
        }
    }
}
//--------------------------   Pathfinder Functions  ----------------------------//

Stack<blockm*> findPath(blockm*** mazes, const int mazechoice, const int M, const int N,
    const int xent, const int yent, const int xext, const int yext)
{
    //create pathfinding stack
    Stack<blockm*> pathFinder;
    blockm** maze = mazes[mazechoice-1];

    //reset maze blocks to unvisited
    for (int i=0; i<M; i++)
    {
        for(int j=0;j<N;j++)
        {
            maze[i][j].visited = false;
        }
    }

    //push first block to stack
    pathFinder.push(&maze[M-yent-1][xent]); //TODO: potential index bug

    bool foundPath = false;
    while(!foundPath)
    {
        //set current block
        blockm* current = pathFinder.top();

        //mark block visited
        current->visited = true;

        //check if block is the target (change foundPath)
        if(current->x == xext && current->y == yext)
        {
            foundPath = true;
            return pathFinder;
        }

        //check movement options
        char options[4];
        int index=-1;
        //check up
        if (current->u == 0 && current->y != M-1 && !(maze[M-current->y - 2][current->x].visited))
        {
            index++;
            options[index] = 'u';
        }

        if (current->d == 0 && current->y != 0 && !(maze[M-current->y][current->x].visited))
        {
            index++;
            options[index] = 'd';
        }

        if (current->l == 0 && current->x != 0 && !(maze[M-current->y-1][current->x-1].visited))
        {
            index++;
            options[index] = 'l';
        }

        if (current->r == 0 && current->x != N-1 && !(maze[M-current->y-1][current->x+1].visited))
        {
            index++;
            options[index] = 'r';
        } //TODO: potential index bug

        //Case0: there's a valid option
        if(index != -1)
        {
            //choose random option
            RandGen rand;
            char choice = options[rand.RandInt(0, index)];

            //move to said option (push to stack)
            if (choice == 'u')
            {
                pathFinder.push(&maze[M-current->y-2][current->x]);
            }
            else if(choice == 'd')
            {
                pathFinder.push(&maze[M-current->y][current->x]);
            }
            else if(choice == 'l')
            {
                pathFinder.push(&maze[M-current->y-1][current->x-1]);
            }
            else if(choice == 'r')
            {
                pathFinder.push(&maze[M-current->y-1][current->x+1]);
            }
        }
        //Case1: there's no valid option
        else
        {
            //pop from stack and continue
            pathFinder.pop();
        }
    }
}


void savePath(Stack<blockm*>& stack, const int mazeNum)
{
    //Function will turn the stack to a text file following a predetermined format
    /* we know that the top is the exit and bottom is entry */
    int xext, yext;
    xext = stack.top()->x;
    yext = stack.top()->y;

    Stack<blockm*> reverseStack;
    while(!stack.isEmpty())
    {
        reverseStack.push(stack.topAndPop());
    }

    //now the top of reverse stack is the entry
    int xent, yent;
    xent = reverseStack.top()->x;
    yent = reverseStack.top()->y;

    //save to file
    ofstream outfile;
    string filename = "maze_" + to_string(mazeNum) + "_path_" + to_string(xent) + "_"
        + to_string(yent) + "_" + to_string(xext) + "_" + to_string(yext) + ".txt";

    outfile.open(filename.c_str());

    //handle error
    if(outfile.fail())
    {
        cout << "Couldn't create file for path" << endl;
    }
    else
    {
        //write path into file
        while(!reverseStack.isEmpty())
        {
            blockm* current = reverseStack.topAndPop();
            outfile << current->x << " " << current->y << endl;
        }
    }
}




//--------------------------           Main          ----------------------------//

int main()
{
    //maze generation: getting inputs
    int K, M, N;    //K is the number of mazes, M and N are the dimensions of each maze
    cout << "Enter the number of mazes: ";
    cin >> K;

    cout << "Enter then number of rows and columns (M and N): ";
    cin >> M >> N;

    //maze generation: generating mazes
    blockm*** mazes = new blockm**[K];

    for(int i=0; i<K; i++)
    //loop for maze generation
    {

        //Generate maze 2D matrix
        blockm** maze = new blockm*[M];

        for (int j=0; j<M; j++)
        {
            maze[j] = new blockm[N];
        }

        //Generate maze

        makeMaze(maze, M, N);

        //Save generated maze to .txt file and to mazes array

        saveMaze(maze, i+1, M, N);
        mazes[i] = maze;
    }

    cout << "All mazes are generated.\n\n";

    //path finding: getting inputs
    int maze, xentry, yentry, xexit, yexit;
    cout << "Enter a maze ID between 1 to " << K << " inclusive to find a path: ";
    cin >> maze;

    cout << "Enter x and y coordinates of the entry points (x,y) or (column, row): ";
    cin >> xentry >> yentry;

    cout << "Enter x and y coordinates of the exit points (x,y) or (column, row): ";
    cin >> xexit >> yexit;

    //path finding: finding the path

    //create pathfinding stack
    Stack<blockm*> path;
    //find path
    path = findPath(mazes, maze, M, N, xentry, yentry, xexit, yexit);

    //save path to .txt file

    savePath(path, maze);

    //delete dynamically allocated memory
    for(int i=0; i<K; i++)
    {
        for (int j=0; j<M; j++)
        {
            delete [] mazes[i][j];
        }
        delete [] mazes[i];
    }
    delete [] mazes;


    return 0;
}