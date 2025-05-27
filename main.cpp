#include "board.h"
#include "solver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Программа запущена\n";

    std::ifstream input("input.txt");
    if (!input) {
        std::cerr << "Не удалось открыть input.txt\n";
        return 1;
    }

    Board board;
    if (!board.loadFromFile(input)) {
        std::cerr << "Ошибка при загрузке позиции\n";
        return 1;
    }
    std::cout << "Позиция загружена\n";

    GameSolver solver(board);
    std::vector<std::string> resultMoves;

    if (solver.solve(resultMoves)) {
        std::ofstream output("output.txt");
        if (!output) {
            std::cerr << "Не удалось открыть output.txt\n";
            return 1;
        }
        for (const auto& moveStr : resultMoves) {
            output << moveStr << "\n";
        }
        std::cout << "Решение найдено и записано в output.txt\n";
    }
    else {
        std::cout << "Решение не найдено за 2-3 хода\n";
    }

    std::cout << "Программа завершена\n";
    return 0;
}
