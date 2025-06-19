/**
 * @file main.cpp
 * @brief Реализация игры "Жизнь" Конвея с использованием SFML
 *
 * Программа моделирует клеточный автомат по правилам:
 * 1. Мертвая клетка оживает, если рядом ровно 3 живых соседа
 * 2. Живая клетка выживает, если рядом 2 или 3 живых соседа
 * 3. Во всех остальных случаях клетка умирает (от одиночества или
 * перенаселения)
 *
 * Особенности реализации:
 * - Двойная буферизация (grid и nextGrid) для корректного обновления состояний
 * - Интерактивное управление: пауза, ручное редактирование, регулировка
 * скорости
 * - Динамическая настройка параметров симуляции при запуске
 * - Возможность выбора цвета клеток как при старте, так и во время симуляции
 *
 * Структура программы:
 * 1. Класс GameOfLife - ядро логики симуляции
 * 2. Функция main - обработка ввода, рендеринг и цикл приложения
 */

#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "game_of_life.h"

 /**
  * @brief Точка входа и основной игровой цикл
  *
  * Последовательность работы:
  * 1. Запрос параметров симуляции у пользователя:
  *    - Размеры сетки
  *    - Размер клетки
  *    - Интервал обновления
  *    - Начальный цвет клеток
  * 2. Инициализация графического окна
  * 3. Создание экземпляра GameOfLife
  * 4. Обработка событий (клавиатура/мышь)
  * 5. Цикл рендеринга с регулируемой частотой
  *
  * Управление:
  * - Пробел: Старт/пауза
  * - R: Случайная инициализация
  * - C: Очистка поля
  * - S: Вывод статистики в консоль
  * - ЛКМ: Переключение клетки
  * - Стрелки Вверх/Вниз: Изменение скорости
  * - Клавиши 1-5: Изменение цвета клеток
  * - Enter: Шаг вперед (когда стоит пауза)
  * - Escape: Выход
  */
int main() {
    std::cout << "Conway's Game of Life\n";
    int gridWidth = 0;
    int gridHeight = 0;
    int cellSize = 0;
    int initialIntervalMs = 0;
    int colorChoice = 0;

    std::cout << "Enter grid width (e.g., 100): ";
    std::cin >> gridWidth;
    std::cout << "Enter grid height (e.g., 100): ";
    std::cin >> gridHeight;
    std::cout << "Enter cell size in pixels (e.g., 8): ";
    std::cin >> cellSize;
    std::cout << "Enter initial simulation interval in ms (e.g., 100): ";
    std::cin >> initialIntervalMs;

    std::cout << "\nChoose initial cell color:\n";
    std::cout << "1. Green (default)\n";
    std::cout << "2. Red\n";
    std::cout << "3. Blue\n";
    std::cout << "4. Yellow\n";
    std::cout << "5. Cyan\n";
    std::cout << "Enter your choice (1-5): ";
    std::cin >> colorChoice;

    if (gridWidth <= 0 || gridHeight <= 0 || cellSize <= 0 ||
        initialIntervalMs <= 0) {
        std::cerr << "Invalid parameters. Exiting.\n";
        return 1;
    }
    if (colorChoice < 1 || colorChoice > 5) {
        std::cout << "Invalid color choice. Using default (Green).\n";
        colorChoice = 1;
    }

    unsigned int windowWidth = static_cast<unsigned int>(gridWidth * cellSize);
    unsigned int windowHeight = static_cast<unsigned int>(gridHeight * cellSize);

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)), "Game of Life");
    GameOfLife game(gridWidth, gridHeight);

    switch (colorChoice) {
    case 1:
        game.setCellColor(sf::Color::Green);
        break;
    case 2:
        game.setCellColor(sf::Color::Red);
        break;
    case 3:
        game.setCellColor(sf::Color::Blue);
        break;
    case 4:
        game.setCellColor(sf::Color::Yellow);
        break;
    case 5:
        game.setCellColor(sf::Color::Cyan);
        break;
    }

    bool isRunning = false;
    bool stepRequested = false;
    int intervalMs = initialIntervalMs;
    sf::Clock clock;
    sf::Time elapsed;

    std::cout << "Controls:\n";
    std::cout << "  Space: Start/Pause simulation\n";
    std::cout << "  R: Randomize grid\n";
    std::cout << "  C: Clear grid\n";
    std::cout << "  S: Show statistics in console\n";
    std::cout << "  Left Mouse Click: Toggle cell state\n";
    std::cout << "  Right Mouse Click: Toggle immortality for a live cell\n";
    std::cout
        << "  Up/Down arrows: Increase/Decrease simulation speed (interval ms)\n";
    std::cout << "  Color selection:\n";
    std::cout << "    1: Green\n";
    std::cout << "    2: Red\n";
    std::cout << "    3: Blue\n";
    std::cout << "    4: Yellow\n";
    std::cout << "    5: Cyan\n";
    std::cout << "  Enter: Step forward (when paused)\n";
    std::cout << "  Escape or Close window: Exit\n";

    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            const auto& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            else if (event.is<sf::Event::KeyPressed>()) {
                auto code = event.getIf<sf::Event::KeyPressed>()->code;
                if (code == sf::Keyboard::Key::Space) {
                    isRunning = !isRunning;
                }
                else if (code == sf::Keyboard::Key::R) {
                    game.randomize();
                }
                else if (code == sf::Keyboard::Key::C) {
                    game.clear();
                }
                else if (code == sf::Keyboard::Key::S) {
                    std::cout << "Generation: " << game.getGenerationCount() << std::endl;
                    std::cout << "Live cells: " << game.getLiveCellCount() << std::endl;
                    std::cout << "Dead cells: " << game.getDeadCellCount() << std::endl;
                    std::cout << "Live cell percentage: " << game.getLiveCellPercentage()
                        << "%" << std::endl;
                }
                else if (code == sf::Keyboard::Key::Up) {
                    intervalMs = std::max(1, intervalMs - 10);
                    std::cout << "Interval: " << intervalMs << " ms\n";
                }
                else if (code == sf::Keyboard::Key::Down) {
                    intervalMs += 10;
                    std::cout << "Interval: " << intervalMs << " ms\n";
                }
                else if (code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                else if (code == sf::Keyboard::Key::Num1) {
                    game.setCellColor(sf::Color::Green);
                }
                else if (code == sf::Keyboard::Key::Num2) {
                    game.setCellColor(sf::Color::Red);
                }
                else if (code == sf::Keyboard::Key::Num3) {
                    game.setCellColor(sf::Color::Blue);
                }
                else if (code == sf::Keyboard::Key::Num4) {
                    game.setCellColor(sf::Color::Yellow);
                }
                else if (code == sf::Keyboard::Key::Num5) {
                    game.setCellColor(sf::Color::Cyan);
                }
                else if (code == sf::Keyboard::Key::Enter) {
                    stepRequested = true;
                }
            }
            else if (event.is<sf::Event::MouseButtonPressed>()) {
                auto mb = event.getIf<sf::Event::MouseButtonPressed>();
                if (mb->button == sf::Mouse::Button::Left) {
                    int x = mb->position.x / cellSize;
                    int y = mb->position.y / cellSize;
                    game.toggleCell(x, y);
                }
                else if (mb->button == sf::Mouse::Button::Right) {
                    int x = mb->position.x / cellSize;
                    int y = mb->position.y / cellSize;
                    game.toggleImmortal(x, y);
                }
            }
        }

        if (isRunning) {
            elapsed = clock.getElapsedTime();
            if (elapsed.asMilliseconds() >= intervalMs) {
                game.update();
                clock.restart();
            }
        }
        else if (stepRequested) {
            game.update();
            stepRequested = false;
            clock.restart();
        }
        else {
            clock.restart();
        }

        window.clear(sf::Color::Black);
        game.draw(window, cellSize);
        window.display();
    }

    return 0;
}