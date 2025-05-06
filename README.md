# TapeSorter

Проект **TapeSorter** реализует алгоритм внешней сортировки с использованием эмуляции ленточных устройств хранения. Поддерживается ограничение по объёму памяти и задержки, характерные для реальных магнитных лент.

---

## 📁 Структура проекта

```

TapeSorter/
├── CMakeLists.txt               
├── include/
│   ├── TapeDevice.h             # Абстрактный интерфейс устройства ленты
│   ├── FileTapeDevice.h         # Класс для работы с файловой лентой
│   ├── TapeSorter.h             # Класс сортировщика ленты
│   └── config.h                 # Структура задержек
├── src/                         # Реализация заголовочных файлов
│   ├── FileTapeDevice.cpp       
│   ├── TapeSorter.cpp           
│   └── main.cpp                 
├── test/
│   ├── file\_tape\_device\_tests.cpp  # Тесты FileTapeDevice
│   └── tape\_sorter\_tests.cpp       # Тесты TapeSorter
├── tmp/                             # папка для хранения временных лент
├── input.bin, input.bin, output.bin, output2.bin      # Входная/выходная бинарные ленты
│
├── generate_bin.sh             # для генерации входного input.bin файла с большим количеством чисел
│     
├── external/
│   └── googletest/              # GoogleTest для написания unit-тестов
├── main.cpp                     # файл запуска программы
│
└── README.md                  # Текущий файл

```

---

## 📄 Описание основных файлов

- `TapeDevice.h` — интерфейс ленты: `read`, `write`, `moveForward`, `rewind`, `moveBackward`, `getSize()`, `updateSize()` .
- `FileTapeDevice.*` — реализация интерфейса, работающего с бинарным файлом.
- `TapeSorter.*` — алгоритм внешней сортировки: разбиение + слияние.
- `config.h` — структура `TapeDelays`, содержащая задержки в миллисекундах.
- `main.cpp` — пример использования сортировщика.
- `tests/` — модульные тесты с использованием GoogleTest.
- `external/googletest/` — исходный код библиотеки GTest взятый с данного [репозитория](https://github.com/google/googletest).

---

