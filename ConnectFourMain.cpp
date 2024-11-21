#include <iostream>
#include "cf.h"

using namespace std;

int main() {
    initBoard();
    char selection = showMenu();

    if (selection == 'A') {
        playAgainstAi();
    } else if (selection == 'H') {
        playAgainstHuman();
    }

    cout << " \n press enter to exit";
    cin.ignore();
    cin.get();

    return 0;
}