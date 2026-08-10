# cpp-middle-project-sprint-2-dynamic <!-- omit in toc -->

- [Начало работы](#начало-работы)
- [Сборка проекта и запуск тестов](#сборка-проекта-и-запуск-тестов)
  - [Команды для сборки проекта](#команды-для-сборки-проекта)
  - [Команда для запуска тестов](#команда-для-запуска-тестов)
- [Сборка под Windows](#сборка-под-windows)
  - [Что нужно установить](#что-нужно-установить)
  - [Как собрать](#как-собрать)
  - [Пресеты CMake](#пресеты-cmake)


Шаблон репозитория для практического задания «Динамическая версия `scan`: интерпретация данных в runtime» 2-го спринта «Мидл разработчик С++»

## Начало работы

1. Нажмите зелёную кнопку `Use this template`, затем `Create a new repository`.
2. Назовите свой репозиторий.
3. Склонируйте созданный репозиторий командой `git clone your-repository-name`.
4. Создайте новую ветку командой `git switch -c development`.
5. Откройте проект в `Visual Studio Code`.
6. Нажмите `F1` и откройте проект в dev-контейнере командой `Dev Containers: Reopen in Container`.

## Сборка проекта и запуск тестов

Данный репозиторий использует **cmake** — генератор систем сборки для C и C++. Позволяет создавать проекты, которые могут компилироваться на различных платформах и с различными компиляторами. Подробнее о cmake:
  - https://dzen.ru/a/ZzZGUm-4o0u-IQlb
  - https://neerc.ifmo.ru/wiki/index.php?title=CMake_Tutorial
  - https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html

### Команды для сборки проекта

```bash
mkdir build ; cd build

# Вызывается один раз перед сборкой проекта, чтобы скачать и установить все необходимые зависимости
conan install --settings=build_type=Debug --build=missing ..

# Вызывается каждый раз, когда необходимо собрать проект
conan build --settings=build_type=Debug ..
```

### Команда для запуска тестов

```bash
cd build
ctest --verbose
```

## Сборка под Windows

Dev-контейнер остаётся основным способом сборки, но проект собирается и нативно —
компилятором MSVC, без Docker и без WSL. Точка входа одна: `scripts\build_windows.ps1`.

### Что нужно установить

- **Visual Studio 2022** (Community достаточно) с рабочей нагрузкой
  «Разработка классических приложений на C++». Нужен компилятор MSVC v143;
  проверялось на `cl.exe` 19.44. Более ранние версии могут не потянуть C++23:
  проект опирается на `std::expected`, `std::println` и `std::string_view::contains`.
- **CMake ≥ 3.25** и **Ninja** на `PATH`. Оба приезжают вместе с компонентом
  Visual Studio «C++ CMake tools for Windows», если ставить их отдельно не хочется.
- Conan **не нужен**: внешних зависимостей у проекта нет, все заголовки свои.
  Скрипт вызывает Conan только если в корне появится `conanfile.py`/`conanfile.txt`.

Отдельно запускать «Developer Command Prompt» не требуется — скрипт сам находит
`vcvars64.bat` (через `vswhere`, с запасными путями) и вносит окружение MSVC в
свой процесс.

### Как собрать

```powershell
powershell -ExecutionPolicy Bypass -File scripts\build_windows.ps1
```

Скрипт настраивает CMake, собирает всё и прогоняет `ctest`, а в конце печатает
пути к получившимся `.exe`. Результат лежит в `build\windows`, основной
исполняемый файл — `build\windows\scan_tests.exe`.

Полезные ключи:

| Ключ | Зачем |
| --- | --- |
| `-BuildType Debug` | сборка с отладочной информацией в `build\windows-debug` |
| `-Clean` | удалить каталог сборки и собрать с нуля |
| `-SkipTests` | не запускать `ctest` |

Тесты, которые обязаны **не** компилироваться (`tests/fail_*.cpp`), в обычную
сборку не входят — как и на Linux, они собираются отдельной целью:

```powershell
cmake --build build\windows --target check-compile-fails
```

### Пресеты CMake

`CMakePresets.json` описывает обе платформы, поэтому IDE (Visual Studio, VS Code,
CLion) подхватывает конфигурацию сама:

| Пресет | Платформа | Генератор | Каталог сборки |
| --- | --- | --- | --- |
| `linux-default` | Linux / dev-контейнер | Unix Makefiles | `build/` |
| `windows-msvc-release` | Windows, MSVC x64 | Ninja | `build/windows/` |
| `windows-msvc-debug` | Windows, MSVC x64 | Ninja | `build/windows-debug/` |

Windows-пресеты рассчитаны на окружение MSVC: если запускать их руками, а не
через `scripts\build_windows.ps1`, делать это надо из «Developer PowerShell for
VS 2022», иначе CMake не найдёт `cl.exe`.
