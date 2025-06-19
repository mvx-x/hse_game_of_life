#include <gtest/gtest.h>

#include "game_of_life.h"

// Тест для конструктора и getGridSize
TEST(GameOfLifeTest, ConstructorAndGridSize) {
    GameOfLife game(10, 20);
    std::pair<int, int> size = game.getGridSize();
    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 20);
}

// Тест для clear()
TEST(GameOfLifeTest, Clear) {
    GameOfLife game(5, 5);
    game.randomize();
    game.clear();
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            EXPECT_FALSE(game.isCellAlive(x, y));
        }
    }
}

// Тест для toggleCell
TEST(GameOfLifeTest, ToggleCell) {
    GameOfLife game(3, 3);
    EXPECT_FALSE(game.isCellAlive(0, 0));
    game.toggleCell(0, 0);
    EXPECT_TRUE(game.isCellAlive(0, 0));
    game.toggleCell(0, 0);
    EXPECT_FALSE(game.isCellAlive(0, 0));

    // Проверка границ
    game.toggleCell(-1, -1);
    EXPECT_FALSE(game.isCellAlive(-1, -1));
    game.toggleCell(100, 100);
    EXPECT_FALSE(game.isCellAlive(100, 100));
}

// Тест для update() - простой случай: вымирание от одиночества
TEST(GameOfLifeTest, UpdateLonelyCellDies) {
    GameOfLife game(3, 3);
    game.toggleCell(1, 1);
    game.update();
    EXPECT_FALSE(game.isCellAlive(1, 1));
}

// Тест для update() - простой случай: рождение клетки
TEST(GameOfLifeTest, UpdateCellBorn) {
    GameOfLife game(3, 3);
    game.toggleCell(0, 0);
    game.toggleCell(0, 1);
    game.toggleCell(0, 2);
    game.update();
    EXPECT_TRUE(game.isCellAlive(1, 1));
}

// Тест для update() - простой случай: выживание клетки
TEST(GameOfLifeTest, UpdateCellSurvives) {
    GameOfLife game(3, 3);
    game.toggleCell(0, 1);
    game.toggleCell(1, 1);
    game.toggleCell(2, 1);
    game.update();
    EXPECT_TRUE(game.isCellAlive(1, 1));
}

// Тест для счетчика поколений
TEST(GameOfLifeTest, GenerationCount) {
    GameOfLife game(3, 3);
    EXPECT_EQ(game.getGenerationCount(), 0);

    game.update();
    EXPECT_EQ(game.getGenerationCount(), 1);

    game.update();
    EXPECT_EQ(game.getGenerationCount(), 2);

    game.resetGenerationCount();
    EXPECT_EQ(game.getGenerationCount(), 0);
}

// Тест для подсчета живых клеток
TEST(GameOfLifeTest, LiveCellCount) {
    GameOfLife game(3, 3);
    EXPECT_EQ(game.getLiveCellCount(), 0);

    game.toggleCell(0, 0);
    EXPECT_EQ(game.getLiveCellCount(), 1);

    game.toggleCell(1, 1);
    EXPECT_EQ(game.getLiveCellCount(), 2);
}

// Тест для подсчета мертвых клеток
TEST(GameOfLifeTest, DeadCellCount) {
    GameOfLife game(2, 2);
    EXPECT_EQ(game.getDeadCellCount(), 4);

    game.toggleCell(0, 0);
    EXPECT_EQ(game.getDeadCellCount(), 3);

    game.toggleCell(1, 1);
    EXPECT_EQ(game.getDeadCellCount(), 2);
}

// Тест для процента живых клеток
TEST(GameOfLifeTest, LiveCellPercentage) {
    GameOfLife game(2, 2);
    EXPECT_DOUBLE_EQ(game.getLiveCellPercentage(), 0.0);

    game.toggleCell(0, 0);
    EXPECT_DOUBLE_EQ(game.getLiveCellPercentage(), 25.0);

    game.toggleCell(1, 1);
    EXPECT_DOUBLE_EQ(game.getLiveCellPercentage(), 50.0);
}

// Тест для бессмертных клеток: назначение и снятие бессмертия
TEST(GameOfLifeTest, ImmortalToggleAndQuery) {
    GameOfLife game(3, 3);
    game.toggleCell(1, 1);
    EXPECT_FALSE(game.isImmortal(1, 1));
    game.toggleImmortal(1, 1);
    EXPECT_TRUE(game.isImmortal(1, 1));
    game.toggleImmortal(1, 1);
    EXPECT_FALSE(game.isImmortal(1, 1));
}

// Тест: бессмертная клетка не умирает при update
TEST(GameOfLifeTest, ImmortalCellNeverDies) {
    GameOfLife game(3, 3);
    game.toggleCell(1, 1);
    game.toggleImmortal(1, 1);
    // Одиночная клетка должна умереть, но бессмертная — нет
    game.update();
    EXPECT_TRUE(game.isCellAlive(1, 1));
    EXPECT_TRUE(game.isImmortal(1, 1));
    // Снимаем бессмертие — теперь должна умереть
    game.toggleImmortal(1, 1);
    game.update();
    EXPECT_FALSE(game.isCellAlive(1, 1));
}

// Тест: бессмертие не назначается мёртвой клетке
TEST(GameOfLifeTest, ImmortalNotSetOnDeadCell) {
    GameOfLife game(3, 3);
    EXPECT_FALSE(game.isCellAlive(1, 1));
    game.toggleImmortal(1, 1);
    EXPECT_FALSE(game.isImmortal(1, 1));
}

// Тест: clear и randomize сбрасывают бессмертие
TEST(GameOfLifeTest, ImmortalResetOnClearAndRandomize) {
    GameOfLife game(3, 3);
    game.toggleCell(1, 1);
    game.toggleImmortal(1, 1);
    EXPECT_TRUE(game.isImmortal(1, 1));
    game.clear();
    EXPECT_FALSE(game.isImmortal(1, 1));
    game.toggleCell(1, 1);
    game.toggleImmortal(1, 1);
    EXPECT_TRUE(game.isImmortal(1, 1));
    game.randomize();
    EXPECT_FALSE(game.isImmortal(1, 1));
}