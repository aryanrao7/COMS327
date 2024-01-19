#include <string>
#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
#include "tictactoe.h"

using namespace std;

void displayHeader(const string& title) {
    // ASCII art for a simple header
    cout << "===================================" << endl;
    cout << "  " << title << " GAME" << endl;
    cout << "===================================" << endl;
}

void displayHeader2(const string& title) {
    // ASCII art for a simple header
    cout << "===================================" << endl;
    cout << "  " << title << endl;
    cout << "===================================" << endl;
}

int main(int argc, char* argv[])
{
    // Clear the terminal
    system("clear");

    displayHeader2("WELCOME TO THE HUNGER GAMES!");

    cout << "1. Tic Tac Toe (One Player)" << endl;
    cout << "2. Tic Tac Toe (Two Players)" << endl;
    cout << "3. Blackjack" << endl;
    cout << "4. Connect Four"<< endl;

    // Ask for the game selection
    cout << "Enter the game number to play (1, 2, 3 or 4): ";
    char gameChoice;
    cin >> gameChoice;

    switch (gameChoice)
    {
    case '1':{
        displayHeader("TIC TAC TOE (ONE PLAYER)");
        char s,x;
        cout << "Enter player name: ";
        string playerName;
        cin >> playerName;

        cout<<"ENTER PLAYER 1 SYMBOL('X' goes first or 'O' goes second)"<<endl;
        do{
            cin>>s;
        if(toupper(s)!='O' && toupper(s)!='X')
            {
              cout<<"Only 'O' or 'X' allow. Enter again:"<<endl;
            }
        }while(toupper(s)!='O' && toupper(s)!='X');

        if(toupper(s) == 'X'){
          x = 'O';
        }

        else{
          x = 'X';
        }
        
        Player player1(playerName, toupper(s), true);
        Player player2("CPU", x, false); // CPU opponent

        Game ticTacToe(&player1, &player2);
        ticTacToe.drawBoard(); 
        break;
    }
    case '2':{
        displayHeader("TIC TAC TOE (TWO PLAYERS)");

        cout << "Enter player 1 name: ";
        string player1Name;
        cin >> player1Name;

        cout << "Enter player 2 name: ";
        string player2Name;
        cin >> player2Name;

        Player player1(player1Name, 'X', true);
        Player player2(player2Name, 'O', true);

        Game ticTacToe(&player1, &player2);
        ticTacToe.drawBoard(); 
        break;
    }
    case '3':
        displayHeader("BLACKJACK");
        system("./blackjack");
        break;

    case '4':
        displayHeader("CONNECT 4");
        system("./connect");
        break;

    case 'Q':
    case 'q': break;

    default:
        cout << "Invalid game selection. Exiting." << endl;
        return 1;
    }

    return 0;
}
