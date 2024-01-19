#include <iostream>
#include <vector>

const int ROWS = 6;
const int COLS = 7;

class ConnectFour {
private:
    char board[ROWS][COLS];

public:
    ConnectFour() {
        // Initialize the board with empty spaces
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                board[i][j] = ' ';
            }
        }
    }

    void displayBoard() const {
        // Display the current state of the board
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                std::cout << "| " << board[i][j] << " ";
            }
            std::cout << "|\n";
        }
        std::cout << "-----------------------------\n";
    }

    bool isValidMove(int col) const {
        // Check if the selected column is valid for a move
        return col >= 0 && col < COLS && board[0][col] == ' ';
    }

    void makeMove(int col, char player) {
        // Make a move in the specified column for the given player
        for (int i = ROWS - 1; i >= 0; --i) {
            if (board[i][col] == ' ') {
                board[i][col] = player;
                break;
            }
        }
    }

    bool checkWin(char player) const {
        // Check for a win in rows, columns, and diagonals
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (checkDirection(i, j, 1, 0, player) ||
                    checkDirection(i, j, 0, 1, player) ||
                    checkDirection(i, j, 1, 1, player) ||
                    checkDirection(i, j, 1, -1, player)) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    bool checkDirection(int row, int col, int rowDir, int colDir, char player) const {
        // Check for a win in a specific direction
        for (int i = 0; i < 4; ++i) {
            int newRow = row + i * rowDir;
            int newCol = col + i * colDir;
            if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS || board[newRow][newCol] != player) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    ConnectFour game;
    char currentPlayer = 'X';

    std::cout << "Welcome to Connect Four!\n";
    std::cout << "Player 1: X, Player 2: O\n";

    int col;
    bool gameOver = false;

    while (!gameOver) {
        game.displayBoard();

        // Get player move
        std::cout << "Player " << currentPlayer << ", enter your move (column 0-6): ";
        std::cin >> col;

        // Check if the move is valid
        if (game.isValidMove(col)) {
            game.makeMove(col, currentPlayer);

            // Check for a win
            if (game.checkWin(currentPlayer)) {
                game.displayBoard();
                std::cout << "Player " << currentPlayer << " wins!\n";
                gameOver = true;
            } else {
                // Switch players
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        } else {
            std::cout << "Invalid move. Try again.\n";
        }
    }

    return 0;
}
