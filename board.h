#pragma once
#include <vector>
#include <string>
#include <memory>
#include <istream>

enum class Color { WHITE, BLACK };

struct Checker {
    Color color;
    int row, col;
    bool isKing;

    Checker(Color c, int r, int col, bool king = false) : color(c), row(r), col(col), isKing(king) {}
};

class Board {
public:
    Board();

    bool loadFromFile(std::istream& input);
    std::shared_ptr<Checker> getChecker(int row, int col) const;
    void moveChecker(int fromRow, int fromCol, int toRow, int toCol);
    void removeChecker(int row, int col);
    std::vector<std::shared_ptr<Checker>> getAllCheckers(Color color) const;
    bool isInside(int row, int col) const;
    void promoteToKingIfNeeded(std::shared_ptr<Checker>& checker);
    bool isBlackCell(int row, int col) const;
    bool checkPositionCorrectness() const;

private:
    std::shared_ptr<Checker> board[8][8];
};
