#include <iostream>
#include <cstdlib>
#include "bits/stdc++.h"
using namespace std;

class Grid{
private:
  int grid[9][9] ;
  //ph


public:
  void updateNum(int myM, int myN, int myNum){
    grid[myM][myN] = myNum;
  }
  int getNum(int myM, int myN){
    return grid[myM][myN];
  }

};

void inputGrid();

//checks for number in row
bool checkHorizontal(Grid& myGrid, int myM, int myNum){
  for(int i = 0; i < 9; i++){
    if(myGrid.getNum(myM, i) == myNum){
      return true;
    }
  }
  return false;
}
//checks for number in colemn
bool checkVertical(Grid& myGrid, int myN, int myNum){
  for(int i = 0; i < 9; i++){
    if(myGrid.getNum(i, myN) == myNum){
      return true;
    }
  }
  return false;
}

//combines checkVertical and checkHorizontal
bool checkPerp(Grid& myGrid, int myM, int myN, int myNum){
  if(checkHorizontal(myGrid, myM, myNum) || checkVertical(myGrid, myN, myNum)){
    return true;
  }
  return false;
}
//checks the 3x3 block for number
bool checkBlock(Grid& myGrid, int myM, int myN, int myNum){
  int m = myM/3;//makes sure to start on the first of the chunk
  int n = myN/3;
  for(int i = m*3; i < m*3+3; i++){
    for(int j = n*3; j < n*3+3; j++){
      if (myGrid.getNum(i,j) == myNum){
        return true;
      }
    }
  }
  return false;
}


//slighly harder method to do by hand
//checks if open square can only contain one number
bool noOtherNumber(Grid& myGrid){
  //cout <<"ran noOtherNum"<<endl;
  bool updated = false;
  bool finished = true;
  do{
    updated = false;
    for(int i = 0; i < 9; i++){
      for(int j = 0; j < 9; j++){
        //cout <<"("<<i<<","<<j<<")";

        if(myGrid.getNum(i,j)==0){
          int possNum = 0;
          int possI;
          int possJ;
          bool cont = true;
          for(int num = 1; num < 10 && cont == true; num++){
            if(!checkPerp(myGrid, i, j, num) && !checkBlock(myGrid, i, j, num) && possNum == 0 && cont == true){
              possNum = num;
              possI = i;
              possJ = j;
            }
            else if(!checkPerp(myGrid, i, j, num) && !checkBlock(myGrid, i, j, num) && possNum != 0 && cont == true){
              cont = false;
            }

          }
          if(cont == true){
            myGrid.updateNum(possI, possJ, possNum);
            updated = true;
            finished = false;
            //cout <<"("<<i<<","<<j<<")= "<<possNum<<endl;
          }
        }
      }
    }
  }while(updated);
  return finished;
}

//easiest method to do by hand
//checks 3x3 blocks for a single spot for a number
//if it finds 2 available spots, it moves on to a new number
bool IntuitiveSolve(Grid& myGrid){
  //cout <<"ran intuitive solve"<<endl;
  bool updated = false;
  bool finished = true;
  do{
    updated = false;
    for(int lowi = 0; lowi < 9; lowi+=3){//cycling between 3x3 blocks

      for(int lowj = 0; lowj < 9; lowj+=3){

          int highi = lowi + 3; //creating upper limit of 4loops
          int highj = lowj + 3;

          for(int num = 1; num < 10; num++){
            int possNum = 0;
            int possI = 0;
            int possJ = 0;
            bool cont = true;
            for(int i = lowi; i < highi && cont; i++){//cylcing within 3x3 blocks
              for(int j = lowj; j < highj && cont; j++){
                //cout<<"Loop Position = ("<<i<<", "<<j<<"), num = "<<num<<endl;
                if(!checkBlock(myGrid, i,j, num)){//not including it in the below if statement reduces amount of calls to checkBlock
                  if(myGrid.getNum(i,j) == 0 && !checkPerp(myGrid,i,j, num) && possNum == 0 && cont){
                    possNum = num;
                    possI = i;
                    possJ = j;
                  }
                  else if(myGrid.getNum(i,j) == 0 && !checkPerp(myGrid,i,j, num)){
                    cont = false;
                  }
                }else{
                  cont = false;

                }
              }
            }
            if(cont){
              myGrid.updateNum(possI, possJ, possNum);
              updated = true;
              finished = false;
              //out <<"PossNum: ("<<possI<<", "<<possJ<<") = "<<possNum<<endl;
            }
          }
      }
    }
  }while(updated);
 return finished;
}


void displayGrid(Grid& myGrid, int myI, int myJ){
  if (system("CLS")) system("clear");
  cout <<"--------------------------"<<endl;
  int cellCount = 0;
  for (int i = 0; i < 11; i++) {
    for (int j = 0; j < 13; j++) {
      if (i == 3 || i == 7) {
        cout<<"--";
      }else if (j == 0 || j == 4 || j == 8 || j == 12) {
        cout <<"| ";
      }else{
        int cellI = cellCount/9;
        int cellJ = cellCount%9;
        cellCount++;
        if (cellI == myI && cellJ == myJ){
          cout<<"█ ";
        }else{
          int out = myGrid.getNum(cellI,cellJ);
          if (out == 0){cout <<"* ";}
          else{
            cout << out << " ";
          }
        }
      }
    }
    cout <<endl;
  }
    cout <<"--------------------------"<<endl;
}

//SOlver found on https://www.geeksforgeeks.org/sudoku-backtracking-7/

#define UNASSIGNED 0
#define N 9
bool FindUnassignedLocation(int grid[N][N],
                            int& row, int& col);
bool isSafe(int grid[N][N], int row, int col, int num);
bool SolveSudoku(int grid[N][N])
{
  int row, col;
  if (!FindUnassignedLocation(grid, row, col))
        // success!
        return true;

    // Consider digits 1 to 9
    for (int num = 1; num <= 9; num++) {
        // if looks promising
        if (isSafe(grid, row, col, num)) {
            // make tentative assignment
            grid[row][col] = num;

            // return, if success, yay!
            if (SolveSudoku(grid))
                return true;

            // failure, unmake & try again
            grid[row][col] = UNASSIGNED;
        }
    }
    // this triggers backtracking
    return false;
}

bool FindUnassignedLocation(int grid[N][N],
                            int& row, int& col)
{
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == UNASSIGNED)
                return true;
    return false;
}

bool UsedInRow(int grid[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return true;
    return false;
}
bool UsedInCol(int grid[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return true;
    return false;
}
bool UsedInBox(int grid[N][N], int boxStartRow,
               int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + boxStartRow]
                    [col + boxStartCol]
                == num)
                return true;
    return false;
}

bool isSafe(int grid[N][N], int row,
            int col, int num)
{
    /* Check if 'num' is not already placed in
    current row, current column and current 3x3 box */
    return !UsedInRow(grid, row, num)
           && !UsedInCol(grid, col, num)
           && !UsedInBox(grid, row - row % 3,
                         col - col % 3, num)
           && grid[row][col] == UNASSIGNED;
}
void printGrid(int grid[N][N])
{
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++)
            cout << grid[row][col] << " ";
        cout << endl;
    }
}


int main() {

  Grid thegrid;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      thegrid.updateNum(i,j,0);
    }
  }
  int input;
  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {
      displayGrid(thegrid,i,j);
      cout << "Input 0 for empty cells, -1 for go back, then press return."<<endl;
      cout << "Num: ";
      cin >> input;
      if (input == -1){
        if(j!=0){
          j -= 2;
        }
        else if(j == 0){
          i-=1;
          j = 7;
        }
      }
      else{
        thegrid.updateNum(i,j, input);
      }
    }

  }



  //begin solving

  bool solved = false;

  while(!solved){
    bool no = noOtherNumber(thegrid);
    bool intu = IntuitiveSolve(thegrid);
    solved = no && intu;

  }

//stores my class grid in a 2x2 array, because thats how the bactracking alorithm was programed

  int grid[9][9];
  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {
      grid[i][j] = thegrid.getNum(i,j);
    }
  }

  if (SolveSudoku(grid))
          {}//printGrid(grid);
      else
          cout << "No solution exists";


//puts solved 2d array into grid class
//this is so i can use my displayGrid, which only works on the grid class 
  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {

      thegrid.updateNum(i,j,grid[i][j]);
    }
  }


  displayGrid(thegrid, -1,-1);
return 0;

}
