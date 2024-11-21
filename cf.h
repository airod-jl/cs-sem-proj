#ifndef CF_SFML_H
#define CF_SFML_H

#include "cf.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>


using namespace std;

// constants declaration
unsigned const int COL = 7; // dimensions of the board
unsigned const int ROW = 6;
unsigned const int PLAYER_ONE = 1; 
unsigned const int PLAYER_TWO = 2; 
unsigned const int MAX_DEPTH = 7; 
const string YELLOW = "\033[1;33mO\033[1;0m";
const string RED = "\033[1;31mO\033[1;0m";

const int MAX_INT = 2147483647;
const int MIN_INT = -2147483647;

const int CELL_SIZE = 100;
const int WINDOW_WIDTH = COL * CELL_SIZE;
const int WINDOW_HEIGHT = ROW * CELL_SIZE;

extern bool gameFinished;     //extern modifier used so the same variable is used and updated when the header file included in any source file
extern int turns;
extern int thisPlayer;
extern array<array<int, COL>, ROW> board;

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
void displayBoardHuman(array<array<int, COL>, ROW>& board);
void displayBoardAI(array<array<int, COL>, ROW>& board);
void displayPlayerMessage(int player);

bool gameFinished = false; // flag for if game is over
int turns = 1; // count for number of turns
int thisPlayer = PLAYER_ONE; // current player

array<array<int, COL>, ROW> board; // board declaration

/**
 * game playing function"
 * loops between player and ai as they take turns
 */
void playAgainstAi() {
	displayBoardAI(board); // open gui window and take input from there
	
}


/**
 * game playing function
 * loops between player and player as they take turns
 */
void playAgainstHuman() {
	displayBoardHuman(board); // open gui window and take input from there
	
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

		
		cout << ((thisPlayer == PLAYER_ONE) ? RED + "'s Turn: " : YELLOW + "'s Turn: "); //checl which player's turn it is
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

	//if move winning then go for it
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
 * @param b - the board to perform minimax on
 * @param d - the current depth
 * @param alf - alpha contains maximum score for maximizing ai
 * @param bet - beta contains minimum score for minimizing player
 * @param p - current player
 * @return - returns an array of two elements those being the column number at miniMax[1] and the score corresponding to the column in [0]
 */
array<int, 2> miniMax(array<array<int, COL>, ROW>& b, int d, int alf, int bet, int p) { 
	/*
	 * when minimum depth is reached exit the recursive function returning the calculated value
	 *
	 * reduces the depth 
	 */
	if (d == 0 || d >= (COL * ROW) - turns) {
		// get current score to return
		return array<int, 2>{tabScore(b, PLAYER_TWO), -1};
	}

	if (p == PLAYER_TWO) { // if AI player
		array<int, 2> moveSoFar = {MIN_INT, -1}; //contains the score and column needed for the score

		if (winningMove(b, PLAYER_ONE)) { 
			return {MIN_INT + 1, -1}; 
		} 
		for (unsigned int c = 0; c < COL; c++) { 
			if (b[ROW - 1][c] == 0) { 
				array<array<int, COL>, ROW> newBoard = copyBoard(b); 
				makeMove(newBoard, c, p); 
				int score = miniMax(newBoard, d - 1, alf, bet, PLAYER_ONE)[0]; // find move based on that new board state
				if (score > moveSoFar[0]) { //maximizing the score
					moveSoFar = {score, (int)c};
				}
				alf = max(alf, moveSoFar[0]);
				if (alf >= bet) { break; } // for pruning the remaining branches by breaking out of the loop as the optimum move already found
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
				if (alf >= bet) { break; } // for pruning the remaining branches by breaking out of the loop as the optimum move already found
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
	 * horizontal checks of a combinatiion of any 4 AI, Human or empty
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
	unsigned int good = 0; // points of p
	unsigned int bad = 0; // points against p
	unsigned int empty = 0; // empty spots
	for (unsigned int i = 0; i < v.size(); i++) { // find how many of each
		good += (v[i] == p) ? 1 : 0;
		bad += (v[i] == PLAYER_ONE || v[i] == PLAYER_TWO) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}
	// bad was calculated as (bad + good), so remove good
	bad -= good;
	return heurFunction(good, bad, empty);
}

/**
 * heuristic function helps ai pick a node
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
    for (auto& row : board) {
        row.fill(0);
    }
}

/**
 * function to copy board state to another 2D vector
 * ie. make a duplicate board
 * 
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

char showMenu() {
    sf::RenderWindow window(sf::VideoMode(400, 200), "Connect4 - Choose Game Mode");

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text computerButton;
    computerButton.setFont(font);
    computerButton.setString("AI");
    computerButton.setCharacterSize(24);
    computerButton.setFillColor(sf::Color::Black);
    computerButton.setOutlineColor(sf::Color::White);
    computerButton.setOutlineThickness(2);
    computerButton.setPosition(50, 100);

    sf::Text humanButton;
    humanButton.setFont(font);
    humanButton.setString("Human");
    humanButton.setCharacterSize(24);
    humanButton.setFillColor(sf::Color::Black);
    humanButton.setOutlineColor(sf::Color::White);
    humanButton.setOutlineThickness(2);
    humanButton.setPosition(250, 100);

    sf::RectangleShape computerButtonBorder(sf::Vector2f(120, computerButton.getLocalBounds().height + 20));
    computerButtonBorder.setFillColor(sf::Color(255, 255, 190));
    computerButtonBorder.setOutlineColor(sf::Color::White);
    computerButtonBorder.setOutlineThickness(2);
    computerButtonBorder.setPosition(computerButton.getPosition().x - 40, computerButton.getPosition().y - 10);

    sf::RectangleShape humanButtonBorder(sf::Vector2f(120 ,humanButton.getLocalBounds().height + 20));
    humanButtonBorder.setFillColor(sf::Color(255, 255, 190));
    humanButtonBorder.setOutlineColor(sf::Color::White);
    humanButtonBorder.setOutlineThickness(2);
    humanButtonBorder.setPosition(humanButton.getPosition().x - 20, humanButton.getPosition().y - 10);

    char selection = '\0';

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (computerButtonBorder.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selection = 'A';
                        window.close();
                    } else if (humanButtonBorder.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selection = 'H';
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color(128, 128, 128));
        window.draw(computerButtonBorder);
        window.draw(computerButton);
        window.draw(humanButtonBorder);
        window.draw(humanButton);
        window.display();
    }

    return selection;
}



void displayBoardHuman(array<array<int, COL>, ROW>& board) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Connect4");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && !gameFinished) {
                    int col = event.mouseButton.x / CELL_SIZE;
                    if (col >= 0 && col < COL && board[ROW - 1][col] == 0) {
                        makeMove(board, col, thisPlayer);
						for (int row = 0; row < ROW; row++) {
                            for (int col = 0; col < COL; col++) {
                                sf::CircleShape circle(CELL_SIZE / 2 - 5);
                                circle.setPosition(col * CELL_SIZE + 5, (ROW - row - 1) * CELL_SIZE + 5);

                                if (board[row][col] == PLAYER_ONE) {
                                    circle.setFillColor(sf::Color::Yellow);
                                } else if (board[row][col] == PLAYER_TWO) {
                                    circle.setFillColor(sf::Color::Red);
                                } else {
                                    circle.setFillColor(sf::Color::White);
                                }

                                window.draw(circle);
                            }
                        }

						window.display();

                        gameFinished = winningMove(board, thisPlayer);
                        if (gameFinished) {
                            displayPlayerMessage(thisPlayer); //player win message
                        }
						else if (turns >= ROW * COL) {
                			gameFinished = true;
               				displayPlayerMessage(0); // Draw message
            				}
                        thisPlayer = (thisPlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
                        cout << turns++ << endl;
                    }
                }
            }
        }

        window.clear(sf::Color::Blue);

        for (int row = 0; row < ROW; row++) {
            for (int col = 0; col < COL; col++) {
                sf::CircleShape circle(CELL_SIZE / 2 - 5);
                circle.setPosition(col * CELL_SIZE + 5, (ROW - row - 1) * CELL_SIZE + 5);

                if (board[row][col] == PLAYER_ONE) {
                    circle.setFillColor(sf::Color::Yellow);
                } else if (board[row][col] == PLAYER_TWO) {
                    circle.setFillColor(sf::Color::Red);
                } else {
                    circle.setFillColor(sf::Color::White);
                }

                window.draw(circle);
            }
        }

        window.display();
    }
}


void displayBoardAI(array<array<int, COL>, ROW>& board) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Connect4");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left && !gameFinished) {
                    int col = event.mouseButton.x / CELL_SIZE;
                    if (col >= 0 && col < COL && board[ROW-1][col] == 0) {
                        makeMove(board, col, thisPlayer);
						for (int row = 0; row < ROW; row++) {
                            for (int col = 0; col < COL; col++) {
                                sf::CircleShape circle(CELL_SIZE / 2 - 5);
                                circle.setPosition(col * CELL_SIZE + 5, (ROW - row - 1) * CELL_SIZE + 5);

                                if (board[row][col] == PLAYER_ONE) {
                                    circle.setFillColor(sf::Color::Yellow);
                                } else if (board[row][col] == PLAYER_TWO) {
                                    circle.setFillColor(sf::Color::Red);
                                } else {
                                    circle.setFillColor(sf::Color::White);
                                }

                                window.draw(circle);
                            }
                        }

						window.display();

                        gameFinished = winningMove(board, thisPlayer);
                        if (gameFinished) {
                            displayPlayerMessage(thisPlayer);
                        }
                        thisPlayer = (thisPlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
                        turns++;
                    }
                }
            }
        }

		if (!gameFinished && thisPlayer == PLAYER_TWO) {
            int col = aiMove();
            makeMove(board, col, PLAYER_TWO);
				window.clear(sf::Color::Blue);

        	for (int row = 0; row < ROW; row++) {
            	for (int col = 0; col < COL; col++) {
                	sf::CircleShape circle(CELL_SIZE / 2 - 5);
                	circle.setPosition(col * CELL_SIZE + 5, (ROW - row - 1) * CELL_SIZE + 5);

                	if (board[row][col] == PLAYER_ONE) {
                    	circle.setFillColor(sf::Color::Yellow);
                	} else if (board[row][col] == PLAYER_TWO) {
                    	circle.setFillColor(sf::Color::Red);
                	} else {
                    	circle.setFillColor(sf::Color::White);
                	}

               		 window.draw(circle);
            	}
        	}

        	window.display();
            gameFinished = winningMove(board, PLAYER_TWO);
            if (gameFinished) {
                displayPlayerMessage(PLAYER_TWO);
            } else if (turns >= ROW * COL) {
                gameFinished = true;
                displayPlayerMessage(0); // Draw message
            }
            thisPlayer = PLAYER_ONE;
            cout << turns++;
        }

	}
}


void displayPlayerMessage(int player) {
    sf::RenderWindow window(sf::VideoMode(300, 100), "Player Message");

    sf::Font font;
	font.loadFromFile("arial.ttf");

    sf::Text message;
    message.setFont(font);
    message.setCharacterSize(24);
    message.setFillColor(sf::Color::Black);
    message.setOutlineColor(sf::Color::White);
    message.setOutlineThickness(2);
    message.setPosition(50, 30);

    if (player == PLAYER_ONE) {
        message.setString("Player Yellow Wins!");
    } else if (player == PLAYER_TWO) {
        message.setString("Player Red Wins!");
    } else {
        message.setString("It's a Draw!");
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(128, 128, 128));
        window.draw(message);
        window.display();
    }
}

#endif // CF_SFML_H