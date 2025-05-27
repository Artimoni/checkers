#include "solver.h"
#include <algorithm>
#include <iostream>

GameSolver::GameSolver(const Board& initialBoard) : board(initialBoard) {}

bool GameSolver::isDirectionValidForChecker(const std::shared_ptr<Checker>& checker, int dr, int /*dc*/) {
    if (!checker) return false;
    if (checker->isKing) return true;
    if (checker->color == Color::WHITE) return dr < 0;
    else return dr > 0;
}

bool GameSolver::solve(std::vector<std::string>& resultMoves) {
    std::vector<Move> moves;
    if (dfs(board, 3, moves, Color::WHITE)) {
        resultMoves.clear();
        for (const auto& m : moves)
            resultMoves.push_back(m.toString());
        return true;
    }
    return false;
}

bool GameSolver::dfs(Board currentBoard, int depth, std::vector<Move>& moves, Color currentPlayer) {
    std::cout << "Глубина: " << depth << ", сторона: " << (currentPlayer == Color::WHITE ? "белые" : "чёрные") << "\n";
    if (depth == 0) {
        if (currentBoard.getAllCheckers(Color::BLACK).empty())
            return true;
        if (currentPlayer == Color::BLACK && generateAllMoves(currentBoard, Color::BLACK).empty())
            return true;
        return false;
    }

    auto possibleMoves = generateAllMoves(currentBoard, currentPlayer);
    if (possibleMoves.empty()) {
        if (currentPlayer == Color::BLACK)
            return true;
        else
            return false;
    }

    for (const auto& move : possibleMoves) {
        Board nextBoard = currentBoard;
        int fromR = move.fromRow, fromC = move.fromCol;
        if (!move.captured.empty()) {
            for (size_t i = 0; i < move.captured.size(); ++i) {
                if (i + 1 >= move.path.size()) break;
                int toR = move.path[i + 1].first;
                int toC = move.path[i + 1].second;
                nextBoard.moveChecker(fromR, fromC, toR, toC);
                nextBoard.removeChecker(move.captured[i].first, move.captured[i].second);
                fromR = toR; fromC = toC;
            }
        }
        else if (!move.path.empty()) {
            int toR = move.path[0].first;
            int toC = move.path[0].second;
            nextBoard.moveChecker(move.fromRow, move.fromCol, toR, toC);
        }

        moves.push_back(move);
        bool isWin = dfs(nextBoard, depth - 1, moves, currentPlayer == Color::WHITE ? Color::BLACK : Color::WHITE);
        if (isWin) return true;
        moves.pop_back();
    }
    return false;
}

std::vector<Move> GameSolver::generateAllMoves(const Board& currentBoard, Color color) {
    std::cout << "Генерация ходов для " << (color == Color::WHITE ? "белых" : "чёрных") << ":\n";
    std::vector<Move> captures;
    auto checkers = currentBoard.getAllCheckers(color);
    for (auto& checker : checkers) {
        if (checker) {
            std::cout << "  Шашка: " << (char)('A' + checker->col) << (8 - checker->row) << "\n";
            findCaptures(currentBoard, checker, captures);
        }
    }
    if (!captures.empty()) {
        std::cout << "Найдено взятий: " << captures.size() << "\n";
        for (const auto& m : captures) {
            std::cout << "  " << m.toString() << "\n";
        }
        return captures;
    }

    std::vector<Move> moves;
    int dr[] = { -1, -1, 1, 1 };
    int dc[] = { -1, 1, -1, 1 };

    for (auto& checker : checkers) {
        if (!checker) continue;
        for (int i = 0; i < 4; ++i) {
            if (!isDirectionValidForChecker(checker, dr[i], dc[i]) && !checker->isKing) continue;
            if (checker->isKing) {
                int nr = checker->row + dr[i];
                int nc = checker->col + dc[i];
                while (currentBoard.isInside(nr, nc) && !currentBoard.getChecker(nr, nc)) {
                    Move m;
                    m.fromRow = checker->row;
                    m.fromCol = checker->col;
                    m.path.push_back({ nr, nc });
                    moves.push_back(m);
                    nr += dr[i];
                    nc += dc[i];
                }
            }
            else {
                int nr = checker->row + dr[i];
                int nc = checker->col + dc[i];
                if (currentBoard.isInside(nr, nc) && !currentBoard.getChecker(nr, nc)) {
                    Move m;
                    m.fromRow = checker->row;
                    m.fromCol = checker->col;
                    m.path.push_back({ nr, nc });
                    moves.push_back(m);
                }
            }
        }
    }
    std::cout << "Найдено обычных ходов: " << moves.size() << "\n";
    for (const auto& m : moves) {
        std::cout << "  " << m.toString() << "\n";
    }
    return moves;
}

void GameSolver::findCaptures(const Board& board, std::shared_ptr<Checker> checker,
    std::vector<Move>& moves,
    std::vector<std::pair<int, int>> path,
    std::vector<std::pair<int, int>> captured,
    std::vector<std::vector<bool>> visited) {
    if (!checker) return;
    bool found = false;
    int dr[] = { -1, -1, 1, 1 };
    int dc[] = { -1, 1, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        if (!isDirectionValidForChecker(checker, dr[i], dc[i]) && !checker->isKing) continue;
        if (checker->isKing) {
            int nr = checker->row + dr[i];
            int nc = checker->col + dc[i];
            while (board.isInside(nr, nc) && !board.getChecker(nr, nc)) {
                nr += dr[i]; nc += dc[i];
            }
            if (!board.isInside(nr, nc)) continue;
            auto enemy = board.getChecker(nr, nc);
            if (enemy && enemy->color != checker->color) {
                int toR = nr + dr[i];
                int toC = nc + dc[i];
                while (board.isInside(toR, toC) && !board.getChecker(toR, toC)) {
                    if (!visited[nr][nc]) {
                        Board newBoard = board;
                        newBoard.moveChecker(checker->row, checker->col, toR, toC);
                        newBoard.removeChecker(nr, nc);
                        auto newChecker = newBoard.getChecker(toR, toC);

                        auto newPath = path;
                        if (newPath.empty())
                            newPath.push_back({ checker->row, checker->col });
                        newPath.push_back({ toR, toC });
                        auto newCaptured = captured;
                        newCaptured.push_back({ nr, nc });
                        auto newVisited = visited;
                        newVisited[nr][nc] = true;

                        findCaptures(newBoard, newChecker, moves, newPath, newCaptured, newVisited);
                        found = true;
                    }
                    toR += dr[i]; toC += dc[i];
                }
            }
        }
        else {
            int midR = checker->row + dr[i];
            int midC = checker->col + dc[i];
            int toR = checker->row + 2 * dr[i];
            int toC = checker->col + 2 * dc[i];
            if (!board.isInside(toR, toC)) continue;
            auto midChecker = board.getChecker(midR, midC);
            if (midChecker && midChecker->color != checker->color && !board.getChecker(toR, toC) && !visited[midR][midC]) {
                Board newBoard = board;
                newBoard.moveChecker(checker->row, checker->col, toR, toC);
                newBoard.removeChecker(midR, midC);
                auto newChecker = newBoard.getChecker(toR, toC);

                auto newPath = path;
                if (newPath.empty())
                    newPath.push_back({ checker->row, checker->col });
                newPath.push_back({ toR, toC });
                auto newCaptured = captured;
                newCaptured.push_back({ midR, midC });
                auto newVisited = visited;
                newVisited[midR][midC] = true;

                findCaptures(newBoard, newChecker, moves, newPath, newCaptured, newVisited);
                found = true;
            }
        }
    }
    if (!found && !captured.empty()) {
        moves.push_back({ path[0].first, path[0].second, path, captured });
        std::cout << "Найдено взятие: " << moves.back().toString() << "\n";
    }
}
