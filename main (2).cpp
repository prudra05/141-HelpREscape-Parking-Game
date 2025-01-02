#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <set>

using namespace std;

bool CheckMove(char board[6][6], char tempBoard[6][6], char color, int position, char dir) {//checks whether move is valid on tempBoard
  for (int row = 0; row < 6; row++) {
    for (int col = 0; col <6; col++) {
      if (board[row][col] == color) {
        if (dir == 'D') {
          if (tempBoard[row+position][col] != '.') {
            return false;
            break;
          }
        }
        if (dir == 'U') {
          if (tempBoard[row-position][col] != '.') {
            return false;
            break;
          }
        }
        if (dir == 'L') {
          if (tempBoard[row][col-position] != '.') {
            return false;
            break;
          }
        } 
        if (dir == 'R') {
          if (tempBoard[row][col+position] != '.') {
            return false;
            break;
          }
        } 
      }
    }
  }
  return true;
}

bool isFileValid(string FileName) { //Checks whether file is valid and opens without error
  ifstream gameBoard;
  gameBoard.open(FileName);
  if (gameBoard.is_open()) {
    return true;
  }
  else {
    return false;
  }
  gameBoard.close();
}

bool Quit(string userInput) { //Allows user to continue playing until they quit
  if (userInput != "Q") {
    return true;
  }
  else {
    return false;
  }
}

void setBoard(char board[6][6], string FileName) { //contents of gameBoard(file) are put in to board
  ifstream gameBoard;
  gameBoard.open(FileName);
  if (gameBoard.is_open()) {
    for (int row = 0; row < 6; row++) {
      gameBoard >> board[row];
    }
  }
  gameBoard.close();
}

set<char> Alpha;
void findUniqueAlphas(char board[6][6]) { //checks whether all the cars on the board are valid, cars on board are letters A-Z.
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      char element = board[i][j];
      if (element >= 'A' && element <= 'Z') {
        Alpha.insert(element);
      }
    }
  }
}

void showBoard(char board[6][6]) { //Displayes the board to user and sets the frame of the board
  cout << "--------" << endl;
  for (int row = 0; row < 6; row++) {
    cout << "|";
    for (int col = 0; col < 6; col++) {
      if (board[row][col] != '.' && isalpha(board[row][col]) == false) {
        board[row][col] = '.';
      }
    cout << board[row][col];
    }
    
    if (row == 2) {
      cout << "=" << endl;
    }
    else {
      cout << "|" << endl;
    }
  }
  cout << "--------" << endl;
}

bool checkDistance(string userInput) { //Function checks whether userInput specifically distance is accurate
  if(isdigit(userInput.at(1))) {
    return true;
  }
  else {
    cout << "That is not a valid numeric move" << endl;
    return false;
  }
}

bool checkDirection(char dir) { //Function checks whether userInput specifically direction is accurate
  if (dir == 'L' || dir == 'R' || dir == 'U' || dir == 'D') {
    return true;
  }
  else {
    cout << "That direction is not valid" << endl;
    return false;
  }
}


int main() {
  //Main Board
  char board[6][6] = {
  {'.', '.', '.', '.', '.', '.'}, {'.', '.', '.', '.', '.', '.'},
  {'.', '.', '.', '.', '.', '.'}, {'.', '.', '.', '.', '.', '.'},
  {'.', '.', '.', '.', '.', '.'}, {'.', '.', '.', '.', '.', '.'}
  };
  //This is equivalent to board.txt
  string FileName;
  string userInput; //Color-Position-Direction 

  cout << "Enter filename: " << endl;
  cin >> FileName;

  if (isFileValid(FileName)) { //Checks if file exists and opens without error
    setBoard(board,FileName); //sets board with contents of board.txt(or other file)
    showBoard(board);//then displays board
  }

  while (isFileValid(FileName)) {
    cout << "Enter next move (or Q to quit): "; //Asks user for next move until File exists and opens without error
    cin >> userInput;

    while (Quit(userInput)) { //This takes place while user has not Quit
      char color = toupper(userInput.at(0)); //color of Car
      int position = (userInput.at(1)-48); //How many steps car is supposed to take
      char dir = toupper(userInput.at(2)); //Direction car goes

      if(!checkDistance(userInput)) { //breaks if userInput specifically Distance is invalid
        break;
      }

      if(!checkDirection(dir)) { //breaks if userInput specifically Direction is invalid
        break;
      }
      
      bool carMove = true;
      char tempBoard[6][6]; //temporaryboard to check whether moves are valid

      //removes color (car) from board and replaces with '.'
      for (int row = 0; row < 6; row ++) {
        for (int col = 0; col < 6; col ++) {
          if (color == board[row][col]) {
            tempBoard[row][col] = '.';
          }
          else {
            tempBoard[row][col] = board[row][col];
          }
        }
      }
      
    //checks if move is valid using tempBoard
      int minSteps = -1;
      for (int row = 0; row < 6; row ++) {
        for (int col = 0; col < 6; col ++) {
          if (color == board[row][col]) {
            int possibleStep = 0;
            int tempRow = row;
            int tempCol = col;

            while (true) {
              if (dir == 'L') {
                tempCol -= 1;
              }
              else if (dir == 'R') {
                tempCol += 1;
              }
              else if (dir == 'U') {
                tempRow -= 1;
              }
              else if (dir == 'D') {
                tempRow += 1;
              }
              if (tempBoard[tempRow][tempCol] != '.') { //doesn't allow car to move if another colored car is blocking its path
                break;
              }
              possibleStep++;
            }
            int count = min(position,possibleStep);
            
            if (minSteps == -1 || count < minSteps) {
              minSteps = count;
            }
          }
        }
      }
      //moves car minimum number of steps. For Ex. if user inputs 5 steps but there is only enough space for 2 steps, car moves two steps
      if (carMove) {
        for (int row = 0; row < 6; row ++) {
          for (int col = 0; col < 6; col ++) {
            if (board[row][col]==color) {
              if (dir == 'D') {
                tempBoard[row+minSteps][col] = color;
              }
              if (dir == 'U') {
                tempBoard[row-minSteps][col] = color;
              }
              if (dir == 'L') {
                tempBoard[row][col-minSteps] = color;
              }
              if (dir == 'R') {
                tempBoard[row][col+minSteps] = color;
              }
            }
          }
        }
        for (int row = 0; row < 6; row++) {
          for (int col = 0; col < 6; col++) {
            board[row][col] = tempBoard[row][col];
          } // transfers tempBoard in the original board
        }
      }

      showBoard(board); //displays board to user

      //Displays winning message if R is next to = and then exists userInput loop
      for (int row = 0; row < 6; row++) {
          for (int col = 0; col < 6; col++) {
            if (board[2][5] == 'R') {
              cout << "You win! Congratulation!" << endl;
              return 0;
              break;
            }
          }
        }
      break;
    }
    //Quits loop if user inputs Q
    if (userInput == "Q") {
      break;
    }
  }
  return 0;
}
