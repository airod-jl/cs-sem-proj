#include <iostream>
#include "connect4.h"

using namespace std;


int main()
{
    initialiseBoard();
    initializeGame();
    outputBoard();
    while(gameFinished == false)
    {
        dropChip();
        outputBoard();
        checkGameFinished();
        if(gameFinished == false)
        {
            swapPlayer();
        }
    }


}