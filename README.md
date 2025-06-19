# Conway's Game of Life

Интерактивная кроссплатформенная реализация "Жизни" Конвея с графическим интерфейсом на SFML.

## Функционал
- Классические правила клеточного автомата "Жизнь".
- Графика на SFML (цвета, управление мышью и клавиатурой).
- Пошаговый и автоматический режимы.
- Возможность делать клетки бессмертными (правый клик по живой клетке, бессмертные клетки белые).
- Статистика: поколение, число живых/мертвых клеток, процент живых.
- Unit-тесты на Google Test.

## Установка и сборка

### 1. Установка vcpkg
```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg 
./bootstrap-vcpkg.bat
# или на Linux: bootstrap-vcpkg.sh
```

### 2. Установка зависимостей
```sh
./vcpkg install sfml gtest
```

### 3. Сборка проекта

**Linux/macOS:**
```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

**Windows (Visual Studio 2022):**
```sh
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```

### 4. Запуск
```sh
# Windows
./build/Release/GameOfLife.exe

# Linux/macOS
./build/GameOfLife
``` 