#include <iostream>

using namespace std;

//------GLOBAL CONSTANT DECLARATION-------//
const string BLANK = " ";
const string P1 = "\033[1;31mO\033[1;0m"; //sets P1 to a red "O"
const string P2 = "\033[1;33mO\033[1;0m"; //sets P2 to a yellow "O"


//----GLOBAL VARIABLE DECLARATION---------//
string board[6][7];
short int thisPlayer;   // used as an index for player[], P1 will be 0(red) P2 comp or human will be 1(yellow) 
string player[2] = 
{
    "\033[1;31mO\033[1;0m", "\033[1;33mO\033[1;0m"
};


bool gameFinished, winnerFound;
int validColumn, validRow; // holds the chip being added 

//--------FUNCTION HEADERS-----------------//

void initialiseBoard(); //TO SET ALL VALUES IN THE ARRAY TO BLANK
void outputBoard(); //DISPLAYS THE BOARD
void initializeGame(); //SETS PLAYER TO 1 AND STARTS GAME
bool isColumnValid(int column); // CHECKS IF THE COLUMN CHOSEN BY THE PLAYER IS WITHIN THE BOARD AND HAS SPACE 
int chooseColumn(); // PROMPTS THE PLAYER FOR AN INPUT AND RETURNS IF VALID
int findFreeRow(); // CHECKS HOW FAR DOWN THE CHIP CAN FALL AND RETURNS THE ROW THE CHIP LANDS IN
void dropChip(); // CHANGES THE THE VALUE OF THE ARRAY ACCORDING TO THE ROW AND COLUMN

// CHECKS IF WIN CONDTIONS ARE MET
bool checkDiagonalLine(); 
bool checkVerticalLine();
bool checkHorizontal();

void checkIfPlayerHasWon(); // IF ANY WIN CONDITION MET PLAYER WINS
void swapPlayer(); // CHANGES PLAYER 1 TO 2 AD 2 TO 1
void checkForFullBoard(); //IF NO BLANK SPACES FOUND FINISHES GAME
void checkGameFinished(); // IF ANY PLAYER WON OR BOARD FILLED GAME ENDS






//-----FUNCTIONS FOR INTITIAL SETUP-------//
void initialiseBoard()
{
    for(int row = 0; row < 6; row++)
    {
        for(int column = 0; column <7; column++)
        {
            board[row][column] = BLANK;
        }
    }
}

void outputBoard()  //builds the board from top to bottom (standard cartesian co-ordinate system followed with origin at bottom left)
{
    for(int row = 5; row >= 0; row--)
    {
        for(int column = 0; column <7; column++)
        {
            cout << "[" << board[row][column] << "]";
        }
        cout << endl;
    }
}

void initializeGame()
{
    thisPlayer = 0; // P1(red) goes first
    gameFinished = false;

}


//----------FUNCTIONS FOR MAKING A MOVE--------//
bool isColumnValid(int column)
{
    if(column >= 0 && column <=6)
    {
        if(board[5][column] == BLANK)
        {
            return true;
        }
    }
    return false;
}

int chooseColumn() //Used to prompt an input, validates it and returns it.
{
    int col;
    string player = (thisPlayer == 0) ? "Red" : "Yellow";
    cout << player << " player's turn: " << endl;
    do
    {
        cout << "Please Enter a valid column number: ";
        try
        {
            cin >> col;
            col--; // the user will enter 1-7 but the array indexes go from 0-6 so 1 is subtracted
        }
        catch(const std::exception& e) //in case the user enters a non-integer the program wont terminate but rather display the error
        {
            cout<< "error: " << e.what() << endl;
        }
        
    }while(isColumnValid(col) == false);
    return col;
}

int findFreeRow()
{
    int freerow = 0;
    while(board[freerow][validColumn] != BLANK)
    {
        freerow++;
    }
    return freerow;
}

void dropChip()
{
    validColumn = chooseColumn();
    validRow = findFreeRow();

    board[validRow][validColumn] = player[thisPlayer]; //sets the value  of the chip corresponding to the player  
}


//-------------------Functions to check for victory conditions

bool checkHorizontal()
{
    for(int i = 0; i<=3; i++)
    {
        if //checks if 4 horizontal chips belong to the same player
        (
            board[validRow][i] == player[thisPlayer] &&
            board[validRow][i+1] == player[thisPlayer] &&
            board[validRow][i+2] == player[thisPlayer] &&
            board[validRow][i+3] == player[thisPlayer]
        )
        {
            cout << "horizontal win";
            return true;
        }
    }
    return false;
}

bool checkVerticalLine()
{
   for(int i = 3; i<=5; i++)
    {
        if //checks if 4 vertical chips belong to the same player
        (
            board[i][validColumn] == player[thisPlayer] &&
            board[i - 1][validColumn] == player[thisPlayer] &&
            board[i - 2][validColumn] == player[thisPlayer] &&
            board[i - 3][validColumn] == player[thisPlayer]
        )
        {
            cout << "vertical win";
            return true;
        }
    } 
    return false;

}

bool checkDiagonalLine() 
{
    for(int validColumn = 0; validColumn <=3; validColumn++) //for diagonals tilted to the right 
    {
        for(int validRow = 0; validRow <=2; validRow++)
        {   
            if
            (
                board[validRow][validColumn] == player[thisPlayer] &&
                board[validRow + 1][validColumn + 1] == player[thisPlayer] &&
                board[validRow + 2][validColumn + 2] == player[thisPlayer] &&
                board[validRow + 3][validColumn + 3] == player[thisPlayer]
            )
            {
                return true;
            }
        
        }
    }

    for(int validColumn = 0; validColumn <=3; validColumn++) //for diagonals tilted left
    {
        for(int validRow = 3; validRow <=5; validRow++)
        {   
            if
            (
                board[validRow][validColumn] == player[thisPlayer] &&
                board[validRow - 1][validColumn + 1] == player[thisPlayer] &&
                board[validRow - 2][validColumn + 2] == player[thisPlayer] &&
                board[validRow - 3][validColumn + 3] == player[thisPlayer]
            )
            {
                return true;
            }
        
        }
    }
    return false;
}

void checkIfPlayerHasWon() //if any win condition met player wins
{
    winnerFound = false;
    
    winnerFound = checkHorizontal() || checkVerticalLine() || checkDiagonalLine();  
}

void checkForFullBoard()
{
    bool blankFound = false;
    int column, row;

    do
    {   
        do
        {
            if(board[row][column] == BLANK)
            {
                blankFound = true;
            }

        } while (column < 6 && !blankFound);  

    } while (row < 5 && !blankFound);
    if (!blankFound)
    {
        cout << "It is a draw" << endl;
        gameFinished = true;
    }   
}

void checkGameFinished()
{
    winnerFound = false;
    checkIfPlayerHasWon();
    if (winnerFound)
    {
        gameFinished = true;
        cout << player[thisPlayer] << " has won!";
    }
    else
    {
        checkForFullBoard;
    }

}

void swapPlayer()
{
    if(thisPlayer == 0)
    {
        thisPlayer++;
    }
    else
    {
        thisPlayer--;
    }
}





