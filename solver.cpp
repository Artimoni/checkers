#include "solver.h"
#include <sstream>

GameSolver::GameSolver(const Board& initialBoard) : board(initialBoard) {}

bool GameSolver::solve(std::vector<std::string>& resultMoves) {
    return dfs(board, 3, resultMoves);
}

bool GameSolver::dfs(Board currentBoard, int depth, std::vector<std::string>& moves) {
    if (depth == 0) {
        if (currentBoard.getAllCheckers(Color::BLACK).empty()) {
            return true;
        }
        return false;
    }

    auto whiteMoves = generatePossibleMoves(currentBoard, Color::WHITE);
    for (const auto& move : whiteMoves) {
        moves.push_back(move);
        return true; 
    }

    return false;
}

std::vector<std::string> GameSolver::generatePossibleMoves(const Board& currentBoard, Color color) {
    std::vector<std::string> moves;
    auto checkers = currentBoard.getAllCheckers(color);
    for (auto& checker : checkers) {
        int dr[] = { -1, -1, 1, 1 };
        int dc[] = { -1, 1, -1, 1 };

        for (int i = 0; i < 4; ++i) {
            int nr = checker->row + dr[i];
            int nc = checker->col + dc[i];

            if (!currentBoard.isInside(nr, nc)) continue;
            if (currentBoard.getChecker(nr, nc) == nullptr) {
                std::stringstream ss;
                ss << (char)('A' + checker->col) << (8 - checker->row)
                    << " -> " << (char)('A' + nc) << (8 - nr);
                moves.push_back(ss.str());
            }
        }
    }
    return moves;
}
