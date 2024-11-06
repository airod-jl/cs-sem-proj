#include <iostream>
#include <array>


using namespace std;

// constants declaration
unsigned const int COL = 7; // dimensions of the board
unsigned const int ROW = 6;
unsigned const int PLAYER_ONE = 1; 
unsigned const int PLAYER_TWO = 2; 
unsigned const int MAX_DEPTH = 5; 
const string YELLOW = "\033[1;33mO\033[1;0m";
const string RED = "\033[1;31mO\033[1;0m";

const int MAX_INT = 2147483647;
const int MIN_INT = -2147483647;


// function declarations
void outputBoard(array<array<int, COL>, ROW>&);
int humanMove();
void makeMove(array<array<int, COL>, ROW>&, int, int);
int aiMove();
array<array<int, COL>, ROW> copyBoard(array<array<int, COL>, ROW>&);
bool winningMove(array<array<int, COL>, ROW>&, int);
int scoreSet(array<int, 4>, int);
int tabScore(array<array<int, COL>, ROW>, int);
array<int, 2> miniMax(array<array<int, COL>, ROW>&, int, int, int, int);
int heurFunction(int, int, int);

bool gameFinished = false; // flag for if game is over
int turns = 0; // count for # turns
int thisPlayer = PLAYER_ONE; // current player

array<array<int, COL>, ROW> board; // the game board

/**
 * game playing function
 * loops between players while they take turns
 */
void playAgainstAi() {
	outputBoard(board); // print initial board
	while (!gameFinished) { // while no game over state
		
		if (turns >= ROW * COL) { // if max number of turns reached
			gameFinished = true;
			cout << "Draw!" << endl;
			return;
		}
		else if (thisPlayer == PLAYER_TWO) { // AI move
			makeMove(board, aiMove(), PLAYER_TWO);
		}
		else if (thisPlayer == PLAYER_ONE) { // player move
			makeMove(board, humanMove(), PLAYER_ONE);
		}
	

		gameFinished = winningMove(board, thisPlayer); // check if player won
		thisPlayer = (thisPlayer == 1) ? 2 : 1; // switch player
		turns++; // iterate number of turns
		cout <<  "turn: "<< turns << endl;
		outputBoard(board); // print board after successful move
	}
	cout << ((thisPlayer == PLAYER_ONE) ? YELLOW + " has won!" : RED + " has won!") << endl;
	
}


void playAgaintHuman() {
	outputBoard(board); // print initial board
	while (!gameFinished) { // while no game over state
		
		if (turns >= ROW * COL) { // if max number of turns reached
			gameFinished = true;
			cout << "Draw!" << endl;
			return;
		}
		else if (thisPlayer == PLAYER_TWO) { // AI move
			makeMove(board, humanMove(), PLAYER_TWO);
		}
		else if (thisPlayer == PLAYER_ONE) { // player move
			makeMove(board, humanMove(), PLAYER_ONE);
		}
		
		gameFinished = winningMove(board, thisPlayer); // check if player won
		thisPlayer = (thisPlayer == 1) ? 2 : 1; // switch player
		turns++; // iterate number of turns
		cout <<  "turn: "<< turns << endl;
		outputBoard(board); // print board after successful move
	}
	
	cout << ((thisPlayer == PLAYER_ONE) ? YELLOW + " has won!" : RED + " has won!") << endl;

}

/**
 * function that makes the move for the player
 * @param b - the board to make move on
 * @param c - column to drop piece into
 * @param p - the current player
 */
void makeMove(array<array<int, COL>, ROW>& b, int c, int p) {
	// start from bottom of board going up
	for (unsigned int r = 0; r < ROW; r++) {
		if (b[r][c] == 0) { // first available spot
			b[r][c] = p; // set piece
			break;
		}
	}
}

/**
 * prompts the user for their move
 * and ensures valid user input
 * @return the user chosen column
 */
int humanMove() {
	int move = -1; // temporary
	while (true) { // repeat until proper input given

		
		cout << ((thisPlayer == PLAYER_ONE) ? RED + "'s Turn: " : YELLOW + "'s Turn: ");
		cin >> move; // init move as input
		if (cin.fail()) { // if non-integer
			cin.clear();
			cin.ignore();
			cout << "enter a number (0-6)" << endl; // let user know
		}
		else if (!((unsigned int)move < COL && move >= 0)) { // if out of bounds
			cout << "enter a number (0-6)" << endl; // let user know
		}
		else if (board[ROW - 1][move] != 0) { // if full column
			cout << "column is full" << endl;; // let user know
		}
		else { // if it gets here, input valid
			break;
		}
		cout << endl << endl;
	}
	return move;
}

/**
 * AI "think" algorithm
 * uses minimax to find ideal move
 * @return - the column number for best move
 */
int aiMove() {
	cout << "AI is thinking about a move..." << endl;
	for (int col = 0; col < COL; col++) {
        if (board[ROW - 1][col] == 0) { // Check if the column is not full
            // Simulate the move
            for (unsigned int r = 0; r < ROW; r++) {
                if (board[r][col] == 0) { // Find the first available spot
                    board[r][col] = PLAYER_TWO; // Make the move
                    if (winningMove(board, PLAYER_TWO)) {
                        board[r][col] = 0; // Undo the move
                        return col; // Winning move found
                    }
                    board[r][col] = 0; // Undo the move
                    break; // Exit the loop after making the move
                }
            }
        }
    }

    // Check if the opponent can win in the next move and block it
    for (int col = 0; col < COL; col++) {
        if (board[ROW - 1][col] == 0) { // Check if the column is not full
            // Simulate the move for the opponent
            for (unsigned int r = 0; r < ROW; r++) {
                if (board[r][col] == 0) { // Find the first available spot
                    board[r][col] = PLAYER_ONE; // Make the opponent's move
                    if (winningMove(board, PLAYER_ONE)) {
                        board[r][col] = 0; // Undo the move
                        return col; // Block the opponent's winning move
                    }
                    board[r][col] = 0; // Undo the move
                    break; // Exit the loop after making the move
                }
            }
        }
    }
	return miniMax(board, MAX_DEPTH, 0 - MAX_INT, MAX_INT, PLAYER_TWO)[1];
}

/**
 * Minimax implementation using alpha-beta pruning
 * @param b - the board to perform MM on
 * @param d - the current depth
 * @param alf - alpha
 * @param bet - beta
 * @param p - current player
 */
array<int, 2> miniMax(array<array<int, COL>, ROW>& b, int d, int alf, int bet, int p) {
	/**
	 * if we've reached minimal depth allowed by the program
	 * we need to stop, so force it to return current values
	 * since a move will never (theoretically) get this deep,
	 * the column doesn't matter (-1) but we're more interested
	 * in the score
	 *
	 * as well, we need to take into consideration how many moves
	 * ie when the board is full
	 */
	if (d == 0 || d >= (COL * ROW) - turns) {
		// get current score to return
		return array<int, 2>{tabScore(b, PLAYER_TWO), -1};
	}
	if (p == PLAYER_TWO) { // if AI player
		array<int, 2> moveSoFar = {MIN_INT, -1};

		if (winningMove(b, PLAYER_ONE)) { 
			return {MIN_INT + 1, -1}; 
		} 
		for (unsigned int c = 0; c < COL; c++) { 
			if (b[ROW - 1][c] == 0) { 
				array<array<int, COL>, ROW> newBoard = copyBoard(b); 
				makeMove(newBoard, c, p); 
				int score = miniMax(newBoard, d - 1, alf, bet, PLAYER_ONE)[0]; // find move based on that new board state
				if (score > moveSoFar[0]) { // if better score, replace it, and consider that best move (for now)
					moveSoFar = {score, (int)c};
				}
				alf = max(alf, moveSoFar[0]);
				if (alf >= bet) { break; } // for pruning
			}
		}
		return moveSoFar; // return best possible move
	}
	else {
		array<int, 2> moveSoFar = {MAX_INT, -1}; 
		if (winningMove(b, PLAYER_TWO)) 
		{ 
			return {MAX_INT -1, -1}; 
		} 
		for (unsigned int c = 0; c < COL; c++) {
			if (b[ROW - 1][c] == 0) {
				array<array<int, COL>, ROW> newBoard = copyBoard(b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, alf, bet, PLAYER_TWO)[0];
				if (score < moveSoFar[0]) {
					moveSoFar = {score, (int)c};
				}
				bet = min(bet, moveSoFar[0]);
				if (alf >= bet) { break; }
			}
		}
		return moveSoFar;
	}
}

/**
 * function to tabulate current board "value"
 * @param b - the board to evaluate
 * @param p - the player to check score of
 * @return - the board score
 */
int tabScore(array<array<int, COL>, ROW> b, int p) {
	int score = 0;
	array<int, COL> rs;
	array<int, ROW> cs;
	array<int, 4> set;
	/**
	 * horizontal checks, we're looking for sequences of 4
	 * containing any combination of AI, PLAYER, and empty pieces
	 */
	for (int r = 0; r < ROW; r++) {
		for (int c = 0; c < COL; c++) {
			rs[c] = b[r][c]; // this is a distinct row alone
		}
		for (int c = 0; c < COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
			}
			score += scoreSet(set, p); // find score
		}
	}
	// vertical
	for (int c = 0; c < COL; c++) {
		for (int r = 0; r < ROW; r++) {
			cs[r] = b[r][c];
		}
		for (unsigned int r = 0; r < ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				set[i] = cs[r + i];
			}
			score += scoreSet(set, p);
		}
	}
	// diagonals
	for (unsigned int r = 0; r < ROW - 3; r++) {
		for (unsigned int c = 0; c < COL; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	for (unsigned int r = 3; r < ROW ; r++) {
		for (unsigned int c = 0; c < COL; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r - i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	return score;
}

/**
 * function to find the score of a set of 4 spots
 * @param v - the row/column to check
 * @param p - the player to check against
 * @return - the score of the row/column
 */
int scoreSet(array<int, 4> v, int p) {
	unsigned int good = 0; // points in favor of p
	unsigned int bad = 0; // points against p
	unsigned int empty = 0; // neutral spots
	for (unsigned int i = 0; i < v.size(); i++) { // just enumerate how many of each
		good += (v[i] == p) ? 1 : 0;
		bad += (v[i] == PLAYER_ONE || v[i] == PLAYER_TWO) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}
	// bad was calculated as (bad + good), so remove good
	bad -= good;
	return heurFunction(good, bad, empty);
}

/**
 * heuristic function helps ai decide what patterns to prioritizw
 * 
 * @param g - good points
 * @param b - bad points
 * @param z - empty spots
 * @return - the final score
 */
int heurFunction(int g, int b, int z) {
	int score = 0;
	if (g == 4) { score += 100000; } // preference to go for winning move vs. block
	else if (g == 3 && z == 1) { score += 5000; }
	else if (g == 2 && z ==1)	{score +=1000;}
	else if (b == 2 && z == 2) { score -= 100; } 
	else if (b == 3 && z == 1) { score -= 20000; }
	else if (g == 1) {score +=10;} 
	else if (b == 4) { score -= 200000; }
	
	return score;
}

/**
 * function to determine if move winning
 * @param b - the board to check
 * @param p - the player to check against
 * @return - whether that player can have a winning move
 */
bool winningMove(array<array<int, COL>, ROW>& b, int p) {
	unsigned int winSequence = 0; // to count adjacent pieces
	// for horizontal checks
	for (unsigned int c = 0; c < COL - 3; c++) { // for each column
		for (unsigned int r = 0; r < ROW; r++) { // each row
			for (int i = 0; i < 4; i++) { // recall you need 4 to win
				if ((unsigned int)b[r][c + i] == p) { // if not all pieces match
					winSequence++; // add sequence count
				}
				if (winSequence == 4) { return true; } // if 4 in row
			}
			winSequence = 0; // reset counter
		}
	}
	// vertical checks
	for (unsigned int c = 0; c < COL; c++) {
		for (unsigned int r = 0; r < ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r + i][c] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	// the below two are diagonal checks
	for (unsigned int c = 0; c < COL - 3; c++) {
		for (unsigned int r = 3; r < ROW; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r - i][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	for (unsigned int c = 0; c < COL - 3; c++) {
		for (unsigned int r = 0; r < ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r + i][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	return false; // otherwise no winning move
}

/**
 * sets up the board to be filled with empty spaces
 * also used to reset the board to this state
 */
void initBoard() {
	for (unsigned int r = 0; r < ROW; r++) {
		for (unsigned int c = 0; c < COL; c++) {
			board[r][c] = 0; // make sure board is empty initially
		}
	}
}

/**
 * function to copy board state to another 2D vector
 * ie. make a duplicate board; used for mutating copies rather
 * than the original
 * @param b - the board to copy
 * @return - said copy
 */
array<array<int, COL>, ROW> copyBoard(array<array<int, COL>, ROW>& b) {
	array<array<int, COL>, ROW> newBoard;
	for (unsigned int r = 0; r < ROW; r++) {
		for (unsigned int c = 0; c < COL; c++) {
			newBoard[r][c] = b[r][c]; // just straight copy
		}
	}
	return newBoard;
}

/**
 * prints the board to console out
 * @param b - the board to print
 */
void outputBoard(array<array<int, COL>, ROW>& b)
{
    for (unsigned int i = 0; i < COL; i++) {
		cout << "  " << i;
	}
    cout << endl << " ";
	
	for (unsigned int r = 0; r < ROW; r++) {
		for (unsigned int c = 0; c < COL; c++) {
			cout << "[";
			switch (b[ROW - r - 1][c]) {
			case 0: cout << " ]"; break; // no piece
			case 1: cout << RED + "]"; break; // one player's piece
			case 2: cout << YELLOW + "]"; break; // other player's piece
			}
			
		}
		cout << endl << " ";
    }
	cout << endl;
}
