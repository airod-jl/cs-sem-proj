#include <iostream>
#include "cf_array.h"

using namespace std;

int main() {
	
	char selection;
	while(true)
	{
		cout << "Play against human or AI? (H/A): ";
		cin >> selection;
		
		if (cin.fail()) 
		{ 
			cin.clear();
			cin.ignore();
			cout << "enter a single character H or A" << endl; 
		}
		else if(selection == 'A' || selection == 'H')
		{
			break;
		}
	}

	initBoard();
	if(selection == 'A')
	{
		playAgainstAi(); 
	}
	else if (selection == 'H')
	{
		playAgaintHuman();
	}

	return 0;
}
