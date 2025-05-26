#pragma once
#include "board.h"
#include <vector>
#include <string>

class GameSolver {
public:
    GameSolver(const Board& initialBoard);
    bool solve(std::vector<std::string>& resultMoves);

private:
    Board board;
    bool dfs(Board currentBoard, int depth, std::vector<std::string>& moves);
    std::vector<std::string> generatePossibleMoves(const Board& currentBoard, Color color);
};
