#pragma once
#include "board.h"
#include <vector>
#include <string>
#include <memory>
#include <sstream>

struct Move {
    int fromRow, fromCol;
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> captured;

    std::string toString() const {
        std::stringstream ss;
        if (!captured.empty()) {
            for (size_t i = 0; i < path.size(); ++i) {
                ss << (char)('A' + path[i].second) << (8 - path[i].first);
                if (i + 1 < path.size()) ss << "x";
            }
        }
        else if (!path.empty()) {
            ss << (char)('A' + fromCol) << (8 - fromRow) << "-"
                << (char)('A' + path[0].second) << (8 - path[0].first);
        }
        return ss.str();
    }
};

class GameSolver {
public:
    GameSolver(const Board& initialBoard);
    bool solve(std::vector<std::string>& resultMoves);

private:
    Board board;
    bool dfs(Board currentBoard, int depth, std::vector<Move>& moves, Color currentPlayer);
    std::vector<Move> generateAllMoves(const Board& currentBoard, Color color);
    void findCaptures(const Board& board, std::shared_ptr<Checker> checker,
        std::vector<Move>& moves,
        std::vector<std::pair<int, int>> path = {},
        std::vector<std::pair<int, int>> captured = {},
        std::vector<std::vector<bool>> visited = std::vector<std::vector<bool>>(8, std::vector<bool>(8, false)));
    bool isDirectionValidForChecker(const std::shared_ptr<Checker>& checker, int dr, int dc);
};
