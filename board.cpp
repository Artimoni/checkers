#include "board.h"
#include <sstream>
#include <cctype>

Board::Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = nullptr;
}

bool Board::isInside(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

std::shared_ptr<Checker> Board::getChecker(int row, int col) const {
    if (!isInside(row, col)) return nullptr;
    return board[row][col];
}

void Board::moveChecker(int fromRow, int fromCol, int toRow, int toCol) {
    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = nullptr;
    board[toRow][toCol]->row = toRow;
    board[toRow][toCol]->col = toCol;
    promoteToKingIfNeeded(board[toRow][toCol]);
}

void Board::removeChecker(int row, int col) {
    board[row][col] = nullptr;
}

std::vector<std::shared_ptr<Checker>> Board::getAllCheckers(Color color) const {
    std::vector<std::shared_ptr<Checker>> result;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (board[r][c] && board[r][c]->color == color)
                result.push_back(board[r][c]);
    return result;
}

int letterToCol(char letter) {
    return std::toupper(letter) - 'A';
}

int numberToRow(char number) {
    return 8 - (number - '0');
}

bool Board::loadFromFile(std::istream& input) {
    std::string line;
    int section = 0; // 0 - White, 1 - Black

    while (std::getline(input, line)) {
        if (line.find("White") != std::string::npos) {
            section = 0;
            continue;
        }
        if (line.find("Black") != std::string::npos) {
            section = 1;
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        while (ss >> token) {
            bool isKing = false;
            if (token[0] == 'M') {
                isKing = true;
                token = token.substr(1);
            }

            if (token.size() != 2) continue;
            char colChar = token[0];
            char rowChar = token[1];

            int col = letterToCol(colChar);
            int row = numberToRow(rowChar);

            if (!isInside(row, col)) continue;

            Color color = (section == 0) ? Color::WHITE : Color::BLACK;
            board[row][col] = std::make_shared<Checker>(color, row, col, isKing);
        }
    }

    return true;
}

void Board::promoteToKingIfNeeded(std::shared_ptr<Checker>& checker) {
    if (checker->isKing) return;
    if ((checker->color == Color::WHITE && checker->row == 0) ||
        (checker->color == Color::BLACK && checker->row == 7)) {
        checker->isKing = true;
    }
}
